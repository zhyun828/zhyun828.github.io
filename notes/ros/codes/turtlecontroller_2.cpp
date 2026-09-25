#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>
#include <cmath>
#include <vector>
#include <string>

// 这个项目要完成的事情：
// 1. 控制 turtle1 移动
// 2. 读取 turtle1 当前的位置和朝向
// 3. 让 turtle1 自动走到目标点
// 4. 如果有多个目标点，就按顺序一个一个走
// 5. 可以把目标点在画面里显示出来（spawn）
// 6. 到达目标后可以把目标删掉（kill）

// 输入：
//   - /turtle1/pose      当前乌龟位置
//   - /turtledriver/goal 外部给的新目标
//
// 输出：
//   - /turtle1/cmd_vel   给乌龟的速度命令
//
// 服务：
//   - /spawn             生成目标乌龟
//   - /kill              删除目标乌龟

struct Goal{
    float x,y;
    std::string name;
};//对应乌龟的坐标和乌龟的名称；

//全局变量：保存当前位姿
static turtlesim::Pose g_pose;
//标志位：表示是否已经收到过pose
static bool g_pose_ok = false;
//pose回调函数
static void poseCb(const turtlesim::Pose::ConstPtr& msg){
    g_pose=*msg;//将受到的位姿保存到全局变量
    g_pose_ok=true;
}

//角度归一化函数
static float normAngle(float a){
    const float PI=3.14159265f;
    while(a>PI){
        a-=2.0f*PI;
    }
    while(a<-PI){
        a+=2.0f*PI;
    }
    return a;
}
//服务调用封装
static bool spawnTurtle(ros::ServiceClient& spawn_client,
                        const std::string& name,
                        float x,float y){
    turtlesim::Spawn srv;
    srv.request.x=x;
    srv.request.y=y;
    srv.request.theta=0.0;
    srv.request.name=name;
    if(spawn_client.call(srv)){
        ROS_INFO("Spawn turtle: %s",name.c_str());
        return true;
    }
    ROS_WARN("Failed to spawn turtle: %s",name.c_str());
    return false;
}
//杀死乌龟封装
static void killTurtle(ros::ServiceClient& kill_client,
                       const std::string& name) {
    turtlesim::Kill srv;
    srv.request.name=name;

    if (kill_client.call(srv)) {
        ROS_INFO("Killed turtle: %s",name.c_str());
    } else {
        ROS_WARN("Failed to kill turtle: %s",name.c_str());
    }
}

int main(int argc,char **argv){
    //初始化节点
    ros::init(argc,argv,"turtlecontroller");
    ros::NodeHandle nh;
    //创建速度发布者
    ros::Publisher cmd_pub =
        nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);//cmd_vel 速度控制命令
    //订阅位姿
    ros::Subscriber pose_sub = 
        nh.subscribe("/turtle1/pose",1,poseCb);
    //循环频率
    ros::Rate r(10);
    //创建service client
    ros::ServiceClient spawn_client=
        nh.serviceClient<turtlesim::Spawn>("/spawn");
    ros::ServiceClient kill_client=
        nh.serviceClient<turtlesim::Kill>("/kill");
    //等待服务可用
    spawn_client.waitForExistence();
    kill_client.waitForExistence();

    //目标列表
    std::vector<Goal> fixed_goals={
        {6.0f,6.0f,"goal1"},
        {2.0f,6.0f,"goal2"},
        {8.0f,2.0f,"goal3"}
    };
    
    //比例系数；
    float k_lin=1.5f;
    float k_ang=6.0f;
    //加到达判断
    const float eps=0.20f;
    //当前正在追第几个目标
    int idx=0;
    for (auto& g : fixed_goals) {
            // 先删一下，避免同名残留
            killTurtle(kill_client, g.name);

            // 再在目标位置生成一个小乌龟
            spawnTurtle(spawn_client, g.name, g.x, g.y);
        }
    while(ros::ok()){
        ros::spinOnce();
        // 先简单让乌龟直线走
        //geometry_msgs::Twist cmd;
        // cmd.linear.x = 1.0;
        // cmd.angular.z = 0.0;
        // cmd_pub.publish(cmd);
        if(!g_pose_ok){
            r.sleep();
            continue;
        }
        //查看是否越界
        if(idx>=(int)fixed_goals.size()){
            geometry_msgs::Twist stop;
            cmd_pub.publish(stop);
            ROS_INFO_STREAM("ALL FIXED GOALS REACHED!");
            break;
        }
        //先手动设定一个目标点
        float tx=fixed_goals[idx].x;
        float ty=fixed_goals[idx].y;
        //设置当前乌龟名称
        std::string tname = fixed_goals[idx].name;
        //计算当前位置到目标点的差
        float dx=tx-g_pose.x;
        float dy=ty-g_pose.y;
        //计算距离
        float dist=std::sqrt(dx*dx+dy*dy);
        //计算方向
        float target_theta=std::atan2(dy,dx);
        //计算朝向误差
        float err_theta=target_theta-g_pose.theta;
        //归一化
        err_theta = normAngle(target_theta-g_pose.theta);
        //定义速度命令
        geometry_msgs::Twist cmd;
        //到达判断
        if(dist<eps){
            cmd.linear.x = 0.0f;
            cmd.angular.z = 0.0f;
            cmd_pub.publish(cmd);
            ROS_INFO_STREAM("GOAL "<<idx+1<<" REACHED!");
            killTurtle(kill_client,tname);
            idx++;
            r.sleep();
            continue;
        }
        
        //线速度和角速度
        cmd.linear.x=k_lin*dist;
        cmd.angular.z=k_ang*err_theta;
        //最好先旋转再前进,避免大幅度前进转向
        if(std::fabs(err_theta)>0.7f){
            cmd.linear.x*=0.2f;
        }
        //加个限制幅度
        if(cmd.linear.x>2.0f)cmd.linear.x=2.0f;
        if(cmd.angular.z>4.0f)cmd.angular.z=4.0f;
        if(cmd.angular.z<-4.0f)cmd.angular.z=-4.0f;
        //发布速度命令
        cmd_pub.publish(cmd);
        ROS_INFO("x=%.2f y=%.2f theta=%.2f target=%s dist=%.2f ",g_pose.x,g_pose.y,g_pose.theta,tname.c_str(),dist);
        r.sleep();
    }
    return 0;
}