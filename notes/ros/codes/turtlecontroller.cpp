#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>
#include <cmath>
#include <vector>
#include <string>

// 一个目标点由三部分组成：
// 1) x/y：目标坐标
// 2) name：在 turtlesim 中对应的目标乌龟名（用于可视化与删除）
struct Goal {
  float x, y;
  std::string name;
};

// ========== 全局状态（由回调更新，在主循环读取） ==========
// turtle1 当前位姿
static turtlesim::Pose g_pose;
// 是否已经收到过至少一次 /turtle1/pose
static bool g_pose_ok = false;

// 是否启用“外部目标模式”：
// false：按 fixed_goals 顺序巡航
// true ：跟踪 /turtledriver/goal 提供的目标
static bool g_use_external_goal = false;
// 外部目标坐标（由 goalCb 更新）
static float g_goal_x = 6.0f;
static float g_goal_y = 6.0f;
// 外部目标在 turtlesim 中的可视化名称
static std::string g_goal_name = "goal";  // 动态目标对应的可视化乌龟名称
// 当前外部目标是否已经被 spawn 到画面中
static bool g_goal_spawned = false;

// 角度归一化函数：
// 将任意角度映射到 [-pi, pi]，用于计算最短转向误差。
// 例如 +3.5rad 会变成一个等价的负角，避免“绕大圈转向”。
static float normAngle(float a) {
  const float PI = 3.14159265f;
  while (a > PI)  a -= 2.0f * PI;
  while (a < -PI) a += 2.0f * PI;
  return a;
}

// /turtle1/pose 回调：更新当前乌龟位姿
static void poseCb(const turtlesim::Pose::ConstPtr& msg) {
  g_pose = *msg;
  g_pose_ok = true;
}

// /turtledriver/goal 回调：接收外部目标
// 约定消息类型用 turtlesim/Pose，但这里只使用 x 和 y。
static void goalCb(const turtlesim::Pose::ConstPtr& msg) {
  // 一旦收到外部目标，切到外部目标模式
  g_use_external_goal = true;
  g_goal_x = msg->x;
  g_goal_y = msg->y;
  // 标记为“未生成”，主循环下一次会先 kill 再 spawn 到新位置
  g_goal_spawned = false;
}

// 封装 /spawn 服务调用：在指定坐标创建目标乌龟
static bool spawnTurtle(ros::ServiceClient& spawn_client,
                        const std::string& name, float x, float y) {
  turtlesim::Spawn srv;
  srv.request.x = x;
  srv.request.y = y;
  srv.request.theta = 0.0;
  srv.request.name = name;
  if (spawn_client.call(srv)) {
    ROS_INFO("Spawned turtle: %s at (%.2f, %.2f)", name.c_str(), x, y);
    return true;
  }
  ROS_WARN("Failed to spawn turtle: %s", name.c_str());
  return false;
}

// 封装 /kill 服务调用：删除指定名称的乌龟
static void killTurtle(ros::ServiceClient& kill_client, const std::string& name) {
  turtlesim::Kill srv;
  srv.request.name = name;
  if (kill_client.call(srv)) {
    ROS_INFO("Killed turtle: %s", name.c_str());
  } else {
    ROS_WARN("Failed to kill turtle: %s (maybe doesn't exist)", name.c_str());
  }
}

int main(int argc, char** argv) {
  // 初始化 ROS 节点
  ros::init(argc, argv, "turtlecontroller");
  ros::NodeHandle nh;

  // ========== 通信对象 ==========
  // 发布：给 turtle1 发送速度指令
  ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
  // 订阅：获取 turtle1 实时位姿
  ros::Subscriber pose_sub = nh.subscribe("/turtle1/pose", 1, poseCb);
  // 订阅：接收外部目标（可动态改变目标点）
  ros::Subscriber goal_sub = nh.subscribe("/turtledriver/goal", 1, goalCb);

  // 服务：创建/删除目标乌龟
  ros::ServiceClient spawn_client = nh.serviceClient<turtlesim::Spawn>("/spawn");
  ros::ServiceClient kill_client  = nh.serviceClient<turtlesim::Kill>("/kill");

  // 等待服务可用，避免启动早期调用失败
  spawn_client.waitForExistence();
  kill_client.waitForExistence();

  // ========== 固定目标序列 ==========
  // 在没有外部目标时，turtle1 会按以下顺序逐个到达
  std::vector<Goal> fixed_goals = {
    {6.0f, 6.0f, "goal1"},
    {2.0f, 6.0f, "goal2"},
    {8.0f, 2.0f, "goal3"}
  };
  // 当前处理到第几个固定目标
  int idx = 0;

  // 启动时把固定目标都 spawn 出来，便于观察“目标点位置”
  for (auto& g : fixed_goals) {
    // 防止重复启动导致同名冲突：先尝试删，再创建
    killTurtle(kill_client, g.name);
    spawnTurtle(spawn_client, g.name, g.x, g.y);
  }

  // 控制循环频率
  ros::Rate r(10);          // 10Hz：较平稳，且响应足够快
  const float eps = 0.20f;  // 到达阈值：距离小于该值即视为到达

  // ========== 主控制循环 ==========
  while (ros::ok()) {
    // 处理回调（更新位姿、外部目标）
    ros::spinOnce();

    // 尚未拿到 turtle1 位姿时先等待
    if (!g_pose_ok) { r.sleep(); continue; }

    // 选择当前追踪目标
    float tx, ty;
    std::string tname;

    if (g_use_external_goal) {
      // 外部目标模式：
      // 使用 /turtledriver/goal 给出的坐标，并把目标显示为乌龟 "goal"
      tx = g_goal_x; ty = g_goal_y; tname = g_goal_name;

      if (!g_goal_spawned) {
        // 目标更新后在新位置重生目标乌龟
        killTurtle(kill_client, tname);
        spawnTurtle(spawn_client, tname, tx, ty);
        g_goal_spawned = true;
      }
    } else {
      // 固定目标模式：按顺序访问 fixed_goals
      if (idx >= (int)fixed_goals.size()) {
        // 全部固定目标完成：发布零速度并退出
        geometry_msgs::Twist stop;
        cmd_pub.publish(stop);
        ROS_INFO("All 3 objectives visited.");
        break;
      }
      tx = fixed_goals[idx].x;
      ty = fixed_goals[idx].y;
      tname = fixed_goals[idx].name;
    }

    // 计算相对位移与欧氏距离
    float dx = tx - g_pose.x;
    float dy = ty - g_pose.y;
    float dist = std::sqrt(dx*dx + dy*dy);

    geometry_msgs::Twist cmd;
    if (dist < eps) {
      // 距离足够小，认为“到达目标”，立即停车
      cmd.linear.x = 0.0;
      cmd.angular.z = 0.0;
      cmd_pub.publish(cmd);

      // 到达后删除该目标的可视化乌龟
      if (g_use_external_goal) {
        killTurtle(kill_client, tname);
        g_goal_spawned = false;
        // 外部模式继续监听，可随时接收下一目标
      } else {
        killTurtle(kill_client, tname);
        idx++; // 固定模式切换到下一个目标
      }

      r.sleep();
      continue;
    }

    // 目标朝向（由当前位置指向目标点）
    float target_theta = std::atan2(dy, dx);
    // 航向误差：当前朝向到目标朝向的最短角度差
    float err_theta = normAngle(target_theta - g_pose.theta);

    // 比例控制参数（可调）
    float k_lin = 1.5f;
    float k_ang = 6.0f;

    // P 控制：
    // 线速度与距离成正比，角速度与朝向误差成正比
    cmd.linear.x  = k_lin * dist;
    cmd.angular.z = k_ang * err_theta;

    // 可选策略：角度误差较大时先多转向、少前进
    if (std::fabs(err_theta) > 0.7f) cmd.linear.x *= 0.2f;

    // 速度限幅，避免过大指令导致不稳定
    if (cmd.linear.x > 2.0f) cmd.linear.x = 2.0f;
    if (cmd.angular.z > 4.0f) cmd.angular.z = 4.0f;
    if (cmd.angular.z < -4.0f) cmd.angular.z = -4.0f;

    cmd_pub.publish(cmd);
    r.sleep();
  }

  // 正常退出
  return 0;
}
