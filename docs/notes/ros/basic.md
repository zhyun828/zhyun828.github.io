## catkin 是什么？
catkin 是 ROS 的构建系统
 用来生成：
可执行程序
库
接口（message / service）
catkin_make 必须在 catkin_ws 工作空间根目录下运行

## source ~/catkin_ws/devel/setup.bash
让当前终端环境认识新建的 workspace
每开一个新终端都要 source 一下
否则找不到包，无法 rosrun

## 基本命令
### roscore（最重要）
📌 作用
它会启动：
ROS Master
Parameter Server
rosout（日志系统）
📌 原理
当一个节点启动时，它会：
向 Master 注册
说明自己发布什么 topic
说明自己订阅什么 topic
如果你不启动 roscore：
❌ 所有节点都会报错
❌ 无法建立通信

### rosrun（运行节点）
📌 语法
rosrun <package> <executable>
例如：
rosrun turtlesim turtlesim_node
意思是：
在 turtlesim 包里
运行 turtlesim_node 这个可执行程序
📌 它做了什么？
找到这个 package
找到可执行文件
启动为一个 ROS Node
📌 你要注意
它只能运行：
✔ 已经编译过的节点
✔ 已经 source 过的工作空间
否则找不到包。

### rosnode（管理节点）
这是调试神器。
🔹 查看所有节点
rosnode list
会显示：
/rosout
/turtlesim
/teleop
🔹 查看某个节点详情
rosnode info /turtlesim
你会看到：
发布哪些 topics
订阅哪些 topics
提供哪些 services
这在调试通信时非常重要。
🔹 杀死节点
rosnode kill /node_name
等价于 ctrl+c，但更干净。
🔹 ping 测试
rosnode ping /node_name
测试是否还能连通。

### rostopic（查看话题）
Topic 是 ROS 核心。
这个命令你必须熟练。
🔹 查看当前所有 topic
rostopic list
会看到：
/cmd_vel
/scan
/rosout
🔹 查看某个 topic 的数据
rostopic echo /topic_name
例如：
rostopic echo /turtle1/pose
会不断打印位置信息。
这相当于：
抓包监听
🔹 查看 topic 详细信息
rostopic info /topic_name
会告诉你：
谁在发布
谁在订阅
消息类型
🔹 查看消息类型
rostopic type /topic_name
例如：
geometry_msgs/Twist
🔹 手动发布消息（非常重要）
rostopic pub /topic_name message_type "data"
例如：
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist '{linear: {x: 0.2}}'
这相当于：
你自己模拟一个控制器
🔹 持续发布
rostopic pub -r 10 ...
表示 10Hz 发布。
否则默认只发一次。

### 常见完整实战流程
打开三个终端：
终端1
roscore
终端2
rosrun turtlesim turtlesim_node
终端3
rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist '{angular: {z: 0.5}}'
小乌龟会持续转圈。

## 完整流程：

1️⃣ 写代码
src/hello.cpp
2️⃣ 修改 CMakeLists.txt

添加 add_executable

3️⃣ 编译
cd ~/catkin_ws
catkin_make

4️⃣ source 环境
source devel/setup.bash

5️⃣ 运行
rosrun first_pkg hello

Debug 模式（第25页）
如果你想调试：
cd build
cmake ../src -DCMAKE_BUILD_TYPE=Debug
然后可以用 Eclipse 或 GDB 调试。

## workspace 是什么？
一个 workspace = 一个 ROS 工作区
里面放的是一整套 相关 ROS 代码

## CMakeLists.txt
必须添加
```cmake
add_executable(SubscriberNode src/SubscriberNode.cpp) 
把 src/SubscriberNode.cpp 编译成一个可执行程序把 src/SubscriberNode.cpp 编译成一个可执行程序,名字叫 SubscriberNode,编译后会生成：devel/lib/tp2/SubscriberNode,所以你才能运行：rosrun tp2 SubscriberNode

target_link_libraries(SubscriberNode ${catkin_LIBRARIES}) 
把 ROS 的库链接进你的程序例如：
roscpp、std_msgs、geometry_msgs、turtlesim,否则编译会报错：undefined reference to ros::init,因为程序找不到 ROS 的函数实现。

add_dependencies(SubscriberNode ${catkin_EXPORTED_TARGETS}) 
保证 SubscriberNode 在 ROS 生成代码之后再编译
在 ROS 中，.msg、.srv 和 .action 这类接口文件对应的代码不是手写出来的，而是在编译阶段自动生成的，生成结果包括 C++ 头文件和 Python 模块，比如 std_msgs/String.h。因此，只要你的节点依赖这些自动生成的接口代码，就必须先完成消息生成流程，再编译节点本身；否则编译器在构建节点时找不到对应头文件，就会报 fatal error: std_msgs/String.h: No such file 这类错误。
```


## workspace 的标准结构
catkin_ws/
├── src/        ← 你写代码的地方（最重要）
│   ├── CMakeLists.txt
│   ├── package_1/
│   ├── package_2/
│
├── build/      ← 编译中间文件（不用碰）
├── devel/      ← 生成的可执行程序、环境
└── install/   ← 可选，安装空间

## 什么是 ROS package？
package 是 ROS 中最小的构建单位
一个 package = 一个 ROS 小项目

## 一个 package 里一般有什么？

my_pkg/
├── src/        ← C++ 源文件
├── include/    ← 头文件
├── msg/        ← 自定义消息（以后）
├── srv/        ← 自定义服务（以后）
├── launch/     ← 启动文件（以后）
├── package.xml ← 身份证 + 依赖声明(包名、版本、作者、许可证、依赖等)
└── CMakeLists.txt ← 编译说明书

## 创建 package
<catkin_create_pkg first_pkg std_msgs rospy roscpp>创建一个叫 first_pkg 的包
它依赖：

roscpp（C++）

rospy（Python）

std_msgs（标准消息）



## ROS 三种通信方式
| 类型      | 适合场景            |
| ------- | --------------- |
| Topic   | 单向、连续数据流（传感器数据） |
| Service | 一问一答（即时）        |
| Action  | 长时间任务（可反馈、可取消）  |
1️⃣ Topic
发布 / 订阅
异步
可多个订阅者
👉 典型：激光雷达、相机
2️⃣ Service
请求 / 响应
同步
阻塞式
👉 典型：请求当前状态
3️⃣ Action
长时间任务
可反馈进度
可取消
👉 典型：导航到目标点

## roslaunch
```xml
<launch>
  <node name="talker"
        pkg="first_pkg"
        type="talker"
        output="screen"/>
  <node name="listener"
        pkg="first_pkg"
        type="listener"
        output="screen"/>
</launch>
```
运行
```bash
roslaunch first_pkg talker_listener.launch
```
不用开多个终端，可以统一管理系统
一个 launch 文件可以启动：10 个节点、RViz、Gazebo、参数服务器、重映射
可以设置参数
<param name="rate" value="10"/>
可以重映射 topic
<remap from="chatter" to="my_topic"/>
可以指定 namespace，适合多机器人系统。

## 调试
| 类型    | 示例        | 用什么命令    |
| ----- | --------- | -------- |
| 节点    | /talker   | rosnode  |
| 节点    | /listener | rosnode  |
| topic | /chatter  | rostopic |

```bash
source /opt/ros/noetic/setup.bash #把官方安装的 ROS 加入当前终端的环境变量
source ~/catkin_ws/devel/setup.bash

rosnode list #看有哪些节点在运行
rosnode info /listener 
rosnode info /talker #看某个节点更详细信息（订阅/发布了什么）

rostopic list #看有哪些topic
rostopic echo /chatter #看 “chatter” 正在发布什么内容（实时打印）
rostopic hz /chatter #看 “chatter” 发布频率
rostopic bw /chatter #看 “chatter” 发布带宽
rostopic info /chatter #看 “chatter” 的发布者和订阅者

rosmsg show std_msgs/String #看 std_msgs/String 这个消息类型的结构
rqt_graph #可视化节点和 topic 关系,图形化调试你应该看到：/talker -> /chatter -> /listener
rqt_console #图形化看 topic 内容：rqt_console / rqt_logger_level,看 ROS_INFO/ROS_WARN/ROS_ERROR 输出，并能过滤,还可以动态改日志等级

#录制与回放,录下来之后慢放、回放、复现 bug。
rosbag record -O test_chatter.bag /chatter #录制 chatter：
rosbag play test_chatter.bag #回放
rosbag info test_chatter.bag #查看 bag 文件信息

#看参数
rosparam list #列出所有参数
rosparam get /rosdistro #查看某个参数
```

## 自定义消息

① 创建消息包--catkin_create_pkg custom_messages std_msgs message_generation message_runtime
② 编写 .msg 文件--std_msgs/Header header  int32 robot_id...
③ 修改消息包--package.xml-<build_depend>message_generation</build_depend><exec_depend>message_runtime</exec_depend>
④ 修改消息包--CMakeLists.txt-find_package、add_message_files、generate_messages 、catkin_package
⑤ 编译生成消息代码--catkin_make  source devel/setup.bash 验证：rosmsg show custom_messages/RobotStatus
⑥ 在其他包中声明依赖<build_depend>custom_messages</build_depend><exec_depend>custom_messages</exec_depend> 修改 first_pkg/CMakeLists.txt--find_package
⑦ 在节点中 include 并使用--#include "custom_messages/RobotStatus.h"
⑧ 再次编译并运行--catkin_make source devel/setup.bash

把**自定义消息单独放在一个新包里**（比如 `custom_messages`）
### 目标结构
* `custom_messages`：只负责定义 `.msg`（生成 C++/Python 代码）
* `first_pkg`：你的 `talker.cpp / listener.cpp` 使用 `custom_messages/RobotStatus` 来发布/订阅

### 1）新建消息包 custom_messages
```bash
cd ~/catkin_ws/src
catkin_create_pkg custom_messages std_msgs message_generation message_runtime ##创建一个包叫custom_messages，并且这个包依赖：、std_msgs、message_generation、message_runtime
```
> 这里我直接把 message_generation/runtime 也写进依赖列表里，后面你在 package.xml 里仍要确认存在。

### 2）创建 msg 文件并写 4 种字段类型
```bash
cd ~/catkin_ws/src/custom_messages
mkdir -p msg
code msg/RobotStatus.msg
```
把 `msg/RobotStatus.msg` 写成下面这样（4 种类型 + Header）：
```text
std_msgs/Header header
int32 robot_id
bool is_ready
float32 battery
string note
```
这里包含：
* `int32`（整型）
* `bool`（布尔）
* `float32`（浮点）
* `string`（你要的字符串）
* 以及常用的 `std_msgs/Header`（带时间戳和 frame_id）

### 3）修改 custom_messages/package.xml
打开 `~/catkin_ws/src/custom_messages/package.xml`，确保有这些依赖（重点是 generation/runtime）：
```xml
<buildtool_depend>catkin</buildtool_depend> 意思：是这个包用 catkin 构建系统，几乎所有 ROS1 包都有

<build_depend>std_msgs</build_depend>
<build_depend>message_generation</build_depend>意思：编译时需要 message_generation因为你要从 .msg 文件生成 C++ 代码。

<exec_depend>std_msgs</exec_depend>
<exec_depend>message_runtime</exec_depend>意思：运行时需要 message_runtime也就是说：生成的消息在运行时要用这个库。
```
> 编译期要 `message_generation`，运行期要 `message_runtime`。

### 4）修改 custom_messages/CMakeLists.txt
打开 `~/catkin_ws/src/custom_messages/CMakeLists.txt`，按下面写（或对照改）：
1. find_package 加 message_generation
```cmake
find_package(catkin REQUIRED COMPONENTS
  std_msgs
  message_generation
) 意思：编译这个包需要 std_msgs 和 message_generation
```
2. 声明 msg 文件
```cmake
add_message_files(
  FILES
  RobotStatus.msg
) 我有一个消息文件叫 RobotStatus.msg，请把它加入生成流程
```
3. generate_messages 依赖 std_msgs
```cmake
generate_messages(
  DEPENDENCIES
  std_msgs
)根据 msg 文件生成 C++ / Python 代码，并且它依赖 std_msgs（因为 RobotStatus.msg 里用到了 std_msgs/Header）
```
4. catkin_package 导出 message_runtime
```cmake
catkin_package(
  CATKIN_DEPENDS std_msgs message_runtime
)意思：其他包如果依赖我，就自动继承 std_msgs 和 message_runtime，这叫：依赖传播
```

### 5）编译并验证消息生成
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```
验证消息 ROS 能看到：
```bash
rosmsg show custom_messages/RobotStatus
```
你应该能看到字段列表（header/robot_id/is_ready/battery/note）。

### 6）让 first_pkg 依赖 custom_messages（关键）
你现在的 talker/listener 在 `first_pkg` 里，所以 `first_pkg` 必须“依赖” `custom_messages`。
1. 修改 first_pkg/package.xml
在 `first_pkg/package.xml` 加：
```xml
<build_depend>custom_messages</build_depend>
<exec_depend>custom_messages</exec_depend>
```
（保留你原来的 roscpp/std_msgs 等依赖）

2. 修改 first_pkg/CMakeLists.txt
在 find_package 里加入 custom_messages：
```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  custom_messages
)
```
include 路径（通常已有）：
```cmake
include_directories(
  ${catkin_INCLUDE_DIRS}
)
```
> 只要 first_pkg 正确依赖了 custom_messages，`${catkin_INCLUDE_DIRS}` 里就会包含自定义消息生成的头文件路径。

### 7）改 talker.cpp / listener.cpp 使用新消息
1. talker.cpp（发布 custom_messages/RobotStatus）
```cpp
#include <ros/ros.h>
#include "custom_messages/RobotStatus.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<custom_messages::RobotStatus>("robot_status", 10);
    ros::Rate rate(10);

    int count = 0;
    while (ros::ok())
    {
        custom_messages::RobotStatus msg;

        msg.header.stamp = ros::Time::now();
        msg.header.frame_id = "base_link";

        msg.robot_id = 1;
        msg.is_ready = (count % 2 == 0);
        msg.battery = 12.3f - 0.01f * count;      // 演示用
        msg.note = std::string("hello #") + std::to_string(count);

        ROS_INFO("id=%d ready=%s battery=%.2f note=%s",
                 msg.robot_id,
                 msg.is_ready ? "true" : "false",
                 msg.battery,
                 msg.note.c_str());

        pub.publish(msg);

        ros::spinOnce();
        rate.sleep();
        ++count;
    }
    return 0;
}
```
2. listener.cpp（订阅并打印）
```cpp
#include <ros/ros.h>
#include "custom_messages/RobotStatus.h"

void cb(const custom_messages::RobotStatus::ConstPtr& msg)
{
    ROS_INFO("I heard: id=%d ready=%s battery=%.2f note=%s stamp=%.3f frame=%s",
             msg->robot_id,
             msg->is_ready ? "true" : "false",
             msg->battery,
             msg->note.c_str(),
             msg->header.stamp.toSec(),
             msg->header.frame_id.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("robot_status", 10, cb);
    ros::spin();
    return 0;
}
```

### 8）重新编译并运行（对应 PPT 的运行/调试思路）
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```
终端1：
```bash
roscore
```
终端2：
```bash
rosrun first_pkg talker
```
终端3：
```bash
rosrun first_pkg listener
```
或者直接看 topic：
```bash
rostopic echo /robot_status
```
### 最常见的两个坑（提前避雷）

1. **忘了 source**
   新开终端一定要：

```bash
source ~/catkin_ws/devel/setup.bash
```

2. **first_pkg 没把 custom_messages 加进依赖**
   会出现：找不到 `custom_messages/RobotStatus.h` 或 rosrun 找不到类型。

## TF--Transform
### 1）什么是TF
TF是一个“变换系统”，让你可以在任意时刻把某个坐标系下的数据转换到另一个坐标系下。
假设你有一个机器人：
世界坐标 world/map
机器人本体 base_link / base_footprint
传感器坐标 laser/camera
末端执行器 gripper
头部 head
它们的位置关系是：
world
   ↓
base_link
   ↓
laser
如果激光雷达检测到一个点：(1.0, 0.5)，这个坐标是相对于 laser 坐标系，但你可能需要：转换到 world 坐标系
TF 维护：一个坐标树（Frame Tree）
它不断发布：坐标变换（Transform）
比如：world → base_link  base_link → laser
然后自动帮你计算：world → laser
TF 的本质就是：4×4 齐次变换矩阵
T = [ R  t ]
    [ 0  1 ]
### 2）TF 的优势
分布式：没有单点故障（不是只有一个节点维护全部变换）
链式变换不丢精度/不丢信息：你不用自己一段段拼矩阵
不需要中间坐标系的数据转换成本：tf 能自动沿树找到路径并算合成变换
用户不用纠结“我的数据在哪个 frame”：你只要给出 frame_id，tf 帮你转换到需要的坐标系
能存历史（缓存）：你可以查“过去某时刻”对应的变换
### 3）TF 里有两类节点：Broadcaster / Listener
1. Broadcaster（发布者）
负责发布“坐标系之间的变换”发布到 topic：/tf（动态变换）
2. Listener（监听者）
订阅 /tf，把听到的变换缓存起来（直到缓存长度上限）然后提供查询：lookupTransform(...)
### 4）tf 的核心结构：变换树
tf 会把所有 frame 组织成一棵或多棵树：树上每条边：一个 transform（父→子）
树的任意两点：只要在同一棵树里，就能找到路径并计算变换
可以有多个不相连的树（多个 TF 子系统）
变换只能在同一棵树内生效（不同树无法互相转换）
### 5）怎么用 tf 树来“跨机器人/跨传感器”转换
用 robot1 的激光数据给 robot2 导航
这时你需要把点从：/robot1/laser → 转到 → /robot2/base 或 /map
在 tf 树里，从 source frame 往上走到公共祖先（例如 /map）再从公共祖先往下走到 target frame，中间会包含正向变换和逆变换
### 6）tf 常用命令行工具
view_frames：把 tf 树画成 PDF -- rosrun tf view_frames   /n  evince frames.pdf
tf_echo：实时打印两个 frame 的变换--rosrun tf tf_echo <reference> <target>
tf_monitor：监控 tf 发布频率/延迟--rosrun tf tf_monitor
static_transform_publisher：发布静态变换，用于固定安装的传感器（比如 base_link 到 laser_link 是固定的）或临时补一条 map→odom
### 7）rviz 里看 tf
rosrun rviz rviz -d `rospack find turtle_tf`/rviz/turtle_rviz.rviz
### 8）TF 里常见 frame 的含义
odom：只用里程计积分得到的坐标系（连续但会漂）
base_footprint：机器人底座在地面投影（z=0），常用于地面移动机器人
base_link：机器人本体坐标系（通常在质心或旋转中心附近）
base_laser_link：激光雷达安装坐标系
map
 └── odom
      └── base_footprint
            └── base_link
                  └── base_laser_link
### SLAM -- Simultaneous Localization and Mapping 同时定位与建图
机器人在一个未知环境中，一边画地图，一边确定自己在地图上的位置。
map -> odom：由 SLAM/定位（AMCL）发布（会慢慢调整，纠正漂移）
odom -> base_link：由里程计发布（连续但会漂移）
base_link -> sensor：传感器安装位姿（通常固定）
只要 tf 树连通，就能查：lookupTransform("map","base_link")
或命令行：tf_echo map base_link
得到机器人的全局位置。