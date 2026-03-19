# ROS 基础笔记

## catkin 是什么？

`catkin` 是 ROS 的构建系统，主要用来生成：

- 可执行程序
- 库
- 接口（`message / service`）

`catkin_make` 必须在 `catkin_ws` 工作空间根目录下运行。

## `source ~/catkin_ws/devel/setup.bash`

这条命令的作用是让当前终端“认识”新建的 workspace。

- 每开一个新终端都要 `source` 一次
- 否则终端找不到包，也无法 `rosrun`

## 基本命令

### `roscore`（最重要）

作用：

- 启动 ROS Master
- 启动 Parameter Server
- 启动 `rosout`（日志系统）

原理上，当一个节点启动时，它会向 Master 注册，并说明：

- 自己发布什么 topic
- 自己订阅什么 topic

如果不启动 `roscore`：

- 所有节点都会报错
- 节点之间无法建立通信

### `rosrun`（运行节点）

语法：

```bash
rosrun <package> <executable>
```

例如：

```bash
rosrun turtlesim turtlesim_node
```

意思是：在 `turtlesim` 包里，运行 `turtlesim_node` 这个可执行程序。

它做的事包括：

- 找到对应 package
- 找到可执行文件
- 把它启动成一个 ROS Node

注意：

- 只能运行已经编译过的节点
- 也必须先 `source` 过对应工作空间

否则会出现“找不到包”的问题。

### `rosnode`（管理节点）

这是非常常用的调试工具。

查看所有节点：

```bash
rosnode list
```

常见输出类似：

```text
/rosout
/turtlesim
/teleop
```

查看某个节点详情：

```bash
rosnode info /turtlesim
```

可以看到：

- 发布哪些 topics
- 订阅哪些 topics
- 提供哪些 services

其他常见命令：

```bash
rosnode kill /node_name   # 杀掉节点
rosnode ping /node_name   # 测试节点是否还能连通
```

### `rostopic`（查看话题）

Topic 是 ROS 的核心通信方式，这组命令必须熟练。

查看当前所有 topic：

```bash
rostopic list
```

查看某个 topic 的实时数据：

```bash
rostopic echo /topic_name
```

例如：

```bash
rostopic echo /turtle1/pose
```

这可以理解成一种“抓包监听”。

其他常见命令：

```bash
rostopic info /topic_name
rostopic type /topic_name
rostopic pub /topic_name message_type "data"
rostopic pub -r 10 /topic_name message_type "data"
```

例子：

```bash
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist '{linear: {x: 0.2}}'
```

其中：

- `info` 会告诉你谁在发布、谁在订阅、消息类型是什么
- `type` 用来查看消息类型，比如 `geometry_msgs/Twist`
- `pub` 可以手动发布消息，相当于自己模拟一个控制器
- `-r 10` 表示以 `10 Hz` 持续发布；不写时默认只发一次

### 常见完整实战流程

打开三个终端：

1. 终端 1：
   ```bash
   roscore
   ```
2. 终端 2：
   ```bash
   rosrun turtlesim turtlesim_node
   ```
3. 终端 3：
   ```bash
   rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist '{angular: {z: 0.5}}'
   ```

这样小乌龟就会持续转圈。

## 完整流程

1. 写代码，例如 `src/hello.cpp`
2. 修改 `CMakeLists.txt`，添加 `add_executable`
3. 编译

   ```bash
   cd ~/catkin_ws
   catkin_make
   ```

4. `source` 环境

   ```bash
   source devel/setup.bash
   ```

5. 运行

   ```bash
   rosrun first_pkg hello
   ```

如果想用 Debug 模式，可以：

```bash
cd build
cmake ../src -DCMAKE_BUILD_TYPE=Debug
```

然后再用 Eclipse 或 GDB 调试。

## workspace 是什么？

一个 `workspace` 可以理解成一个 ROS 工作区，里面放的是一整套相关 ROS 代码。

## `CMakeLists.txt`

常见节点至少要加下面三项：

```cmake
add_executable(SubscriberNode src/SubscriberNode.cpp)
target_link_libraries(SubscriberNode ${catkin_LIBRARIES})
add_dependencies(SubscriberNode ${catkin_EXPORTED_TARGETS})
```

- `add_executable(...)`：把 `src/SubscriberNode.cpp` 编译成可执行程序 `SubscriberNode`，之后才能 `rosrun tp2 SubscriberNode`
- `target_link_libraries(...)`：把 `roscpp`、`std_msgs`、`geometry_msgs` 等 ROS 库链接进来，否则会报 `undefined reference to ros::init` 之类的链接错误
- `add_dependencies(...)`：保证消息 / 服务等自动生成的代码先准备好，再编译节点；否则可能找不到 `std_msgs/String.h` 这类头文件

## workspace 的标准结构

```text
catkin_ws/
├── src/        ← 你写代码的地方（最重要）
│   ├── CMakeLists.txt
│   ├── package_1/
│   └── package_2/
├── build/      ← 编译中间文件（一般不用手动改）
├── devel/      ← 生成的可执行程序和环境脚本
└── install/    ← 可选，安装空间
```

## 什么是 ROS package？

`package` 是 ROS 中最小的构建单位，可以把它理解成一个独立的 ROS 小项目。

## 一个 package 里一般有什么？

```text
my_pkg/
├── src/        ← C++ 源文件
├── include/    ← 头文件
├── msg/        ← 自定义消息
├── srv/        ← 自定义服务
├── launch/     ← 启动文件
├── package.xml ← 包信息与依赖声明
└── CMakeLists.txt ← 编译说明书
```

## 创建 package

```bash
catkin_create_pkg first_pkg std_msgs rospy roscpp
```

这会创建一个叫 `first_pkg` 的包，并声明这些依赖：

- `roscpp`：C++ 接口
- `rospy`：Python 接口
- `std_msgs`：标准消息类型

## ROS 三种通信方式

| 类型 | 适合场景 |
| ---- | -------- |
| Topic | 单向、连续数据流（传感器数据） |
| Service | 一问一答（即时） |
| Action | 长时间任务（可反馈、可取消） |

1. Topic
   发布 / 订阅、异步、可以有多个订阅者
   典型场景：激光雷达、相机
2. Service
   请求 / 响应、同步、阻塞式
   典型场景：请求当前状态
3. Action
   长时间任务、可反馈进度、可取消
   典型场景：导航到目标点

## `roslaunch`

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

运行：

```bash
roslaunch first_pkg talker_listener.launch
```

优点：

- 不用开多个终端，可以统一管理系统
- 一个 launch 文件可以同时启动多个节点、RViz、Gazebo、参数服务器等
- 可以设置参数：`<param name="rate" value="10" />`
- 可以重映射 topic：`<remap from="chatter" to="my_topic" />`
- 可以指定 namespace，适合多机器人系统

## 调试

| 类型 | 示例 | 用什么命令 |
| ---- | ---- | ---------- |
| 节点 | `/talker` | `rosnode` |
| 节点 | `/listener` | `rosnode` |
| topic | `/chatter` | `rostopic` |

常用调试命令：

```bash
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash

rosnode list
rosnode info /listener
rosnode info /talker

rostopic list
rostopic echo /chatter
rostopic hz /chatter
rostopic bw /chatter
rostopic info /chatter

rosmsg show std_msgs/String
rqt_graph
rqt_console
```

录制与回放（录下来之后慢放、回放、复现 bug）：

```bash
rosbag record -O test_chatter.bag /chatter
rosbag play test_chatter.bag
rosbag info test_chatter.bag
```

查看参数：

```bash
rosparam list
rosparam get /rosdistro
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
