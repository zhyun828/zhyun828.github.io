## 1️⃣ Topic 通信以及 ROS 基础 API
```cpp
ros::init(argc, argv, "bonjour");// 初始化 ROS，命名节点为 bonjour，节点名必须唯一。
ros::NodeHandle nh;// 创建一个节点句柄，命名为 nh，和 ROS 通信的接口对象通过它：发布 topic，订阅 topic，调用 service，它必须在 ros::init() 之后创建
ros::Publisher pub = nh.advertise<std_msgs::string>("topic",10);// 发布一个 topic，topic 名为 topic，消息类型为 std_msgs::String，队列长度为 10，返回一个 Publisher 对象 pub
ros::Subscriber sub = nh.subscribe("chatter", 10, chatterCallback);// 订阅一个 topic，topic 名为 chatter，消息类型为 std_msgs::String，队列长度为 10，回调函数为 chatterCallback，返回一个 Subscriber 对象 sub
ros::Rate loop_rate(1);//1Hz 循环频率
ros::ok();// 判断节点是否该继续运行，如果Ctrl+C，节点冲突，ros::shutdown() 就会返回 false。
ros::spinOnce();// 处理回调函数，去消息队列里看一眼有新消息就调用对应 callback，没有就直接返回
ros::spin()  进入循环，处理回调函数，直到节点关闭
// 发布消息 hello i
std_msgs::String msg;
msg.data = "hello " + std::to_string(count);
pub.publish(msg);
count++;
// 或者用流式输出
std::stringstream ss
ss << "hello " << count++;
msg.data = ss.str();// 这样就可以发布 hello 0，hello 1，hello 2，...
pub.publish(msg);

loop_rate.sleep(); //让程序：如果这一轮执行太快就 sleep 一下保证循环频率稳定在 10Hz

ROS_INFO("I heard: %s", msg.data.c_str());;// c_str() 将 std::string 转换为 const char*，%s 不支持 std::string 直接输出
ROS_INFO_STREAM("I heard: " << msg.data);// 支持流式输出，且不需要 c_str() 转换
```
## 2️⃣ Service 通信
ROS 中 Service = 同步 RPC 调用 Remote Procedure Call
远程过程调用 调用一个远程节点里的函数，就像本地函数一样。
```cpp
ros::ServiceClient spawn_client = nh.serviceClient<turtlesim::Spawn>("/spawn"); 
// 创建一个 ServiceClient 对象 spawn_client，连接到 /spawn 服务，服务类型为 turtlesim::Spawn
spawn_client.waitForExistence(); // 等待服务存在，如果服务不存在就一直阻塞，直到服务出现
turtlesim::Spawn spawn_srv; // 创建一个服务请求对象 spawn_srv，类型为 turtlesim::Spawn
spawn_srv.request.x = 2.0; // 设置请求参数 x 为 2.
spawn_srv.request.y = 2.0; // 设置请求参数 y 为 2.0
spawn_srv.request.theta = 0.0; // 设置请求参数 theta 为 0.0
spawn_srv.request.name = "turtle2"; // 设置请求参数 name 为 turtle2
spawn_client.call(spawn_srv); // 调用服务，传入请求对象 spawn_srv，如果调用成功返回 true，调用失败返回 false
ros::ServiceServer service = nh.advertiseService("my_service",callback);
// 创建一个 ServiceServer 对象 service，提供一个名为 my_service 的服务，回调函数为 callback，当有客户端调用这个服务时就会调用 callback 函数处理请求
bool callback(std_srvs::Empty::Request &req,std_srvs::Empty::Response &res){   
    ROS_INFO("service called");
    return true;}
```
## 3️⃣ 参数服务器
ROS 中 Parameter Server = 全局共享的字典，存储参数，节点可以读写参数
```cpp
| API          | 作用         | 示例                         |
| ------------ | ---------- | -------------------------- |
| `getParam()` | 读取参数       | `nh.getParam("speed",v);`  |
| `setParam()` | 设置参数       | `nh.setParam("speed",2);`  |
| `param()`    | 读取参数（带默认值） | `nh.param("speed",v,1.0);` |
| `hasParam()` | 判断是否存在     | `nh.hasParam("speed");`    |

```
## 4️⃣ 日志系统
| API           | 级别 |
| ------------- | -- |
| `ROS_DEBUG()` | 调试 |
| `ROS_INFO()`  | 信息 |
| `ROS_WARN()`  | 警告 |
| `ROS_ERROR()` | 错误 |
| `ROS_FATAL()` | 致命 |
```cpp
ROS_INFO("Robot started");
ROS_WARN("Low battery");
ROS_ERROR("Sensor failed");
```
日志文件会存在~/.ros/log

## 5️⃣ 节点状态控制
| API                      | 作用       |
| ------------------------ | -------- |
| `ros::ok()`              | ROS 是否运行 |
| `ros::shutdown()`        | 关闭 ROS   |
| `ros::requestShutdown()` | 请求关闭     |

```cpp
while(ros::ok())
{
    ...
}
```