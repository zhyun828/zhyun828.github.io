# Gazebo 基础

## Gazebo 的两个核心进程

1. `gzserver`
   负责物理引擎、碰撞检测和传感器数据生成。
2. `gzclient`
   负责图形界面和用户交互。

可以简单理解为：

- `gzserver` = 后端计算
- `gzclient` = 前端显示

## 启动 Gazebo

```bash
rosrun gazebo_ros gazebo
```
