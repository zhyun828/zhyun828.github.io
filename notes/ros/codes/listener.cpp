

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

