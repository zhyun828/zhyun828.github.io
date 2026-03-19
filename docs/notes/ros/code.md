# ROS 常用代码片段

## 1. Topic 通信与 ROS 基础 API

```cpp
ros::init(argc, argv, "bonjour");                      // 初始化 ROS，节点名必须唯一
ros::NodeHandle nh;                                    // 节点句柄，发布/订阅/调用 service 都靠它
ros::Publisher pub = nh.advertise<std_msgs::String>("topic", 10);
ros::Subscriber sub = nh.subscribe("chatter", 10, chatterCallback);
ros::Rate loop_rate(1);                                // 1 Hz 循环频率

ros::ok();                                             // 判断节点是否继续运行
ros::spinOnce();                                       // 处理一次回调
ros::spin();                                           // 持续处理回调直到节点关闭

std_msgs::String msg;
msg.data = "hello " + std::to_string(count);
pub.publish(msg);
count++;

std::stringstream ss;
ss << "hello " << count++;
msg.data = ss.str();                                   // 发布 hello 0, hello 1, hello 2...
pub.publish(msg);

loop_rate.sleep();                                     // 保持循环频率稳定

ROS_INFO("I heard: %s", msg.data.c_str());             // printf 风格输出
ROS_INFO_STREAM("I heard: " << msg.data);              // 流式输出
```

补充：

- `ros::NodeHandle` 必须在 `ros::init()` 之后创建。
- `ros::spinOnce()` 适合自己写主循环时使用。
- `ros::spin()` 适合纯回调式节点。

## 2. Service 通信

ROS 中的 Service 可以理解成同步 RPC（Remote Procedure Call），也就是“调用远程节点里的函数，但写法像本地调用”。

```cpp
ros::ServiceClient spawn_client =
    nh.serviceClient<turtlesim::Spawn>("/spawn");

spawn_client.waitForExistence();   // 等待服务存在

turtlesim::Spawn spawn_srv;
spawn_srv.request.x = 2.0;
spawn_srv.request.y = 2.0;
spawn_srv.request.theta = 0.0;
spawn_srv.request.name = "turtle2";

spawn_client.call(spawn_srv);      // 成功返回 true，失败返回 false

ros::ServiceServer service = nh.advertiseService("my_service", callback);

bool callback(std_srvs::Empty::Request &req,
              std_srvs::Empty::Response &res) {
    ROS_INFO("service called");
    return true;
}
```

## 3. 参数服务器

ROS Parameter Server 可以看成一个全局共享字典，节点可以读写参数。

| API | 作用 | 示例 |
| ---- | ---- | ---- |
| `getParam()` | 读取参数 | `nh.getParam("speed", v);` |
| `setParam()` | 设置参数 | `nh.setParam("speed", 2);` |
| `param()` | 读取参数（带默认值） | `nh.param("speed", v, 1.0);` |
| `hasParam()` | 判断是否存在 | `nh.hasParam("speed");` |

## 4. 日志系统

| API | 级别 |
| --- | --- |
| `ROS_DEBUG()` | 调试 |
| `ROS_INFO()` | 信息 |
| `ROS_WARN()` | 警告 |
| `ROS_ERROR()` | 错误 |
| `ROS_FATAL()` | 致命 |

```cpp
ROS_INFO("Robot started");
ROS_WARN("Low battery");
ROS_ERROR("Sensor failed");
```

日志文件通常会保存在 `~/.ros/log`。

## 5. 节点状态控制

| API | 作用 |
| --- | --- |
| `ros::ok()` | ROS 是否仍在运行 |
| `ros::shutdown()` | 关闭 ROS |
| `ros::requestShutdown()` | 请求关闭 |

```cpp
while (ros::ok()) {
    ...
}
```
