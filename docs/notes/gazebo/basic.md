Gazebo 分成两个进程
1. gzserver
负责：
物理引擎
碰撞
传感器数据生成
2. gzclient
负责：
图形界面
用户交互
gzserver = 后端计算
gzclient = 前端显示

启动gazebo
```bash 
rosrun gazebo_ros gazebo
``` 