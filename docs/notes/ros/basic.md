# catkin 是什么？
catkin 是 ROS 的构建系统
 用来生成：
可执行程序
库
接口（message / service）

# workspace 是什么？
一个 workspace = 一个 ROS 工作区
里面放的是一整套 相关 ROS 代码

# workspace 的标准结构
catkin_ws/
├── src/        ← 你写代码的地方（最重要）
│   ├── CMakeLists.txt
│   ├── package_1/
│   ├── package_2/
│
├── build/      ← 编译中间文件（不用碰）
├── devel/      ← 生成的可执行程序、环境
└── install/   ← 可选，安装空间

# 什么是 ROS package？
package 是 ROS 中最小的构建单位
一个 package = 一个 ROS 小项目

# 一个 package 里一般有什么？
my_pkg/
├── src/        ← C++ 源文件
├── include/    ← 头文件
├── msg/        ← 自定义消息（以后）
├── srv/        ← 自定义服务（以后）
├── launch/     ← 启动文件（以后）
├── package.xml ← 身份证 + 依赖声明(包名、版本、作者、许可证、依赖等)
└── CMakeLists.txt ← 编译说明书
# 创建 package
<catkin_create_pkg first_pkg std_msgs rospy roscpp>创建一个叫 first_pkg 的包
它依赖：

roscpp（C++）

rospy（Python）

std_msgs（标准消息）

# 
| 名称                | 一句话理解         |
| ----------------- | ------------- |
| `ros::init`       | 启动 ROS 节点     |
| `ros::NodeHandle` | 节点与 ROS 通信的入口 |
| `ros::Rate`       | 控制循环频率        |
| `ROS_INFO`        | ROS 风格日志输出    |
| `ros::ok`         | 判断节点是否该继续运行   |
NodeHandle 是访问 topic/service/param 的入口
它必须在 ros::init() 之后创建

# source ~/catkin_ws/devel/setup.bash
让当前终端环境认识新建的 workspace