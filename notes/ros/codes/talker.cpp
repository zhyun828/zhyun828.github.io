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