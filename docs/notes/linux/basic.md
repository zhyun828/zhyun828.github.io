# mkdir -p ~/catkin_ws/src
mkdir = make directory（创建文件夹）
-p = parents（父目录）
含义是：
如果中间的目录不存在，就一起帮我创建
没有 -p
mkdir catkin_ws/src
如果 catkin_ws 不存在，会直接报错
~ = 你的 家目录等价于：/home/你的用户名mkdir -p ~/catkin_ws/src等价于：
mkdir -p /home/你的用户名/catkin_ws/src
# nano test.cpp
用来打开编辑文本，如果没有，则创建新文件

# ls列出当前目录里的文件和文件夹

#
| 符号   | 含义         |
| ---- | ---------- |
| .  | 当前目录       |
| .. | 上一级目录（父目录） |
