#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

int main(int argc, char **argv)
{
    // 初始化 ROS 节点，节点名为 SubscriberNode
    ros::init(argc, argv, "SubscriberNode");
    // 创建节点句柄，用于与 ROS 系统通信
    ros::NodeHandle nh;

    // 创建发布器：向 chatter 话题发布 std_msgs::String 类型消息，队列长度为 10
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 10);

    // 设置循环频率为 10Hz
    ros::Rate rate(10);

    // 当 ROS 系统正常运行时持续循环
    while (ros::ok())
    {
        // 定义要发布的字符串消息对象
        std_msgs::String msg;

        // 组织消息内容：hello world + 当前时间戳（秒）
        std::stringstream ss;
        ss << "hello world " << ros::Time::now().toSec();
        msg.data = ss.str();

        // 在终端打印当前消息内容
        ROS_INFO("%s", msg.data.c_str());

        // 发布消息到 chatter 话题
        pub.publish(msg);

        // 按设定频率休眠，保证循环以 10Hz 运行
        rate.sleep();
    }

    // 程序正常退出
    return 0;
}
