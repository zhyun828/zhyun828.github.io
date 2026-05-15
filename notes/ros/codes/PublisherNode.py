#!/usr/bin/env python

import rospy
from std_msgs.msg import String


def talker():
    # 创建 publisher
    pub = rospy.Publisher('chatter', String, queue_size=10)

    # 初始化节点
    rospy.init_node('PublisherNode', anonymous=True)

    # 10Hz
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        hello_str = "hello world %s" % rospy.get_time()

        rospy.loginfo(hello_str)

        pub.publish(hello_str)

        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass