```cpp
ros::init(argc, argv, "bonjour");// 初始化 ROS，命名节点为 bonjour，节点名必须唯一。
ros::NodeHandle node;// 创建一个节点句柄，命名为 nh，和 ROS 通信的接口对象通过它：发布 topic，订阅 topic，调用 service，它必须在 ros::init() 之后创建
ros::Rate loop_rate(1);//1Hz 循环频率
loop_rate.sleep(); //让程序：如果这一轮执行太快就 sleep 一下保证循环频率稳定在 10Hz
ros::ok();// 判断节点是否该继续运行，如果Ctrl+C，节点冲突，ros::shutdown() 就会返回 false。
ros::spinOnce();// 处理回调函数，去消息队列里看一眼有新消息就调用对应 callback，没有就直接返回
ROS_INFO("value = %f", val);;// ROS 风格日志输出
ROS_INFO_STREAM("value = " << val);// ROS 风格日志输出，支持流式输出
```
## ROS 基础代码结构
### talker
```cpp 
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");//初始化 ROS，命名节点为 talker，节点名必须唯一。
  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 10);//创建一个发布者对象，发布 std_msgs::String 类型的消息 "chatter" →topic名称 10 → 队列大小
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    std_msgs::String msg;//std_msgs::String 是 ROS 自带的标准消息类型。它的结构非常简：string data，也就是说，它内部只有一个成员：msg.data
    std::stringstream ss;//这是 C++ 标准库里的字符串流。作用：像 cout 一样拼接字符串，但最后能转成 string
    ss << "hello " << count++;
    msg.data = ss.str();//ss.str() 是把 stringstream 转成 string。等价于：msg.data = "hello 0";现在 msg 里装着字符串。

    ROS_INFO_STREAM("Publishing: " << msg.data);//C++ 风格（流式输出）。
    pub.publish(msg);//这里的 pub 是一个 Publisher 对象。

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
```
### listener
```cpp 
#include <ros/ros.h>//ros/ros.h → ROS 核心 API
#include <std_msgs/String.h>//std_msgs/String.h → 字符串消息类型

void chatterCallback(const std_msgs::String::ConstPtr &msg)//std_msgs::String → 消息类型，ConstPtr → 常量智能指针，& → 引用
{
  ROS_INFO_STREAM("I heard: " << msg->data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("chatter", 10, chatterCallback);
//队列长度 10 是什么意思？如果消息来得太快：callback 来不及处理，超过 10 条，老消息会被丢弃防止内存爆炸
  ros::spin();  
  return 0;
}
```
#