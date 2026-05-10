# Linux 基础命令速记

## `mkdir -p ~/catkin_ws/src`

```bash
mkdir -p ~/catkin_ws/src
```

- `mkdir` = make directory，创建文件夹。
- `-p` = parents，表示中间目录不存在时也一起创建。
- 如果不加 `-p`，直接执行 `mkdir catkin_ws/src`，而 `catkin_ws` 又不存在，就会直接报错。
- `~` 表示当前用户的家目录。

等价写法：

```bash
mkdir -p /home/你的用户名/catkin_ws/src
```

## `nano test.cpp`

```bash
nano test.cpp
```

用于打开并编辑文本文件；如果文件不存在，则会新建该文件。

## `ls`

```bash
ls
```

用于列出当前目录中的文件和文件夹。

## 常见路径符号
| 符号 | 含义 |
| ---- | ---- |
| `.`  | 当前目录 |
| `..` | 上一级目录（父目录） |


drwxr-xr-x 29 yun  yun  4096 5月   7 07:29 .
drwxr-xr-x  3 root root 4096 2月   4 00:00 ..
写上注释

