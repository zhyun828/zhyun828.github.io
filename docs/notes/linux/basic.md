Linux API速查手册网站 https://www.bookstack.cn/read/linuxapi/SUMMARY.md
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

| 符号 | 含义 |
| ---- | ---- |
| `.`  | 当前目录 |
| `..` | 上一级目录（父目录） |
drwxr-xr-x 29 yun  yun  4096 5月   7 07:29 .
drwxr-xr-x  3 root root 4096 2月   4 00:00 ..

## Linux系统IO
//ssize_t是有符号整数类型，表示读写的字节数，返回值为-1表示出错
//off_t是有符号整数类型，表示文件偏移量，单位是字节
//size_t是无符号整数类型，表示写入的字节数
### `open`、`creat`
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>//fcntl是file control的缩写
#include <unistd.h>
int open(const char* pathname,int flags);
int open(const char* pathname,int flags,mode_t mode);
int creat(const char* pathname,mode_t mode);
int close(int fd);
```
### `read`
```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count); 
// fd	文件描述符
// buf	读取的数据存放在buf指针指向的缓冲区
// count	读取的字节数
```
### `write`
```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
// 参数同read函数
```
### `lseek`
```c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
// fd是文件描述符
// offset是偏移量
// whence是偏移量的基准位置。它的取值有三个
// SEEK_SET: 开始位置
// SEEK_CUR: 当前位置
// SEEK_END: 末尾位置
```

## C标准库IO
### `fopen`
```c
#include <stdio.h>
FILE *fopen(const char *pathname, const char *mode);
// pathname是文件路径
// mode是打开文件的模式，常见的有：
// "r"：只读模式，文件必须存在，否则返回NULL
// "w"：写入模式，如果文件存在则清空内容，如果文件不存在则创建新文件
// "a"：追加模式，如果文件存在则在末尾添加内容，如果文件不存在则创建新文件
// "r+"：读写模式，文件必须存在，否则返回NULL
// "w+"：读写模式，如果文件存在则清空内容，如果文件不存在则创建新文件
// "a+"：读写模式，如果文件存在则在末尾添加内容，如果文件不存在则创建新文件
```
### `fclose`
```c
#include <stdio.h>
int fclose(FILE *stream);
// stream是指向FILE对象的指针，表示要关闭的文件流
```
### `fread`
```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
// ptr是指向一个内存块的指针，读取的数据将存储在此处
// size是每个元素的大小，单位是字节
// nmemb是要读取的元素数量
// stream是指向FILE对象的指针，表示要读取的文件流
```
### `fwrite`
```c
#include <stdio.h>
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
// 参数同fread函数
```
### `fseek`
```c
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);
// stream是指向FILE对象的指针，表示要操作的文件流
// offset是偏移量，单位是字节
// whence是偏移量的基准位置。它的取值有三个
// SEEK_SET: 开始位置
// SEEK_CUR: 当前位置
// SEEK_END: 末尾位置
```
### `fclose`
```c
#include <stdio.h>
int fclose(FILE *stream);
// stream是指向FILE对象的指针，表示要关闭的文件流
```

## 进程
Linux 启动后会创建 PID 为 1 的初始进程，后续大多数进程都由它直接或间接创建，整体形成一棵进程树，可用 `pstree` 查看。

- 父进程：创建子进程，可以发送信号、等待子进程结束并获取退出状态。
- 子进程：由父进程创建，初始时相当于父进程的副本，拥有独立地址空间。
- 孤儿进程：父进程先结束，子进程仍在运行，会被 PID 为 1 的进程收养。
- 僵尸进程：子进程已退出，但父进程未调用 `wait` / `waitpid` 回收退出状态。
### `fork`
```c
#include <unistd.h>
pid_t fork(void);
```
`fork` 用于创建子进程。调用后父子进程都会从 `fork` 后继续执行，可通过返回值区分：
- 父进程中返回子进程 PID。
- 子进程中返回 `0`。
- 失败返回 `-1`。
```c
pid_t pid = fork();
if (pid < 0) {
    perror("fork");
} else if (pid == 0) {
    printf("child pid = %d\n", getpid());
} else {
    printf("parent pid = %d, child pid = %d\n", getpid(), pid);
}
```
### 进程终止
进程正常终止：
- `main` 函数中 `return`。
- 调用 `exit(status)`，`status` 是传给父进程的退出状态码。
进程异常终止：收到信号后退出，例如 `SIGKILL`、`SIGSEGV`。
### `wait`、`waitpid`
```c
#include <sys/wait.h>
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```
父进程通过 `wait` / `waitpid` 等待子进程结束，并回收子进程资源。
- `pid > 0`：等待指定 PID 的子进程。
- `pid = -1`：等待任意子进程，等同于 `wait`。
- `options = 0`：阻塞等待。
- `options = WNOHANG`：非阻塞检查，子进程未退出时返回 `0`。
```c
int status;
waitpid(pid, &status, 0);
printf("child exit status = %d\n", WEXITSTATUS(status));
```
### `exec`
`fork` 只会复制当前进程。如果希望子进程执行另一个程序，需要在子进程中调用 `exec` 系列函数。
```c
#include <unistd.h>
int execl(const char *path, const char *arg, ...);
```
`exec` 成功后不会返回，当前进程的代码、数据会被新程序替换；失败返回 `-1`。
```c
pid_t pid = fork();
if (pid == 0) {
    execl("/bin/ls", "ls", "-l", "/etc", (char *)NULL);
    perror("execl");
    return 1;
} else {
    int status;
    waitpid(pid, &status, 0);
}
```
常见 `exec` 函数区别：
- `l`：参数用列表传递，如 `execl`、`execlp`。
- `v`：参数用数组传递，如 `execv`、`execvp`。
- `p`：按 `PATH` 搜索程序，如 `execlp`、`execvp`。
- `e`：可额外指定环境变量，如 `execle`、`execve`。
### 保活进程思路
大型项目中可以用一个监控进程启动并管理业务进程：先 `fork`，子进程 `exec` 启动目标程序；父进程循环调用 `waitpid(pid, &status, WNOHANG)` 检查状态，发现子进程退出后重新启动。
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define PROGRAM "./hello"
pid_t start_program(void) {
    pid_t pid = fork();
    if (pid == 0) {
        execl(PROGRAM, PROGRAM, (char *)NULL);
        perror("execl");
        exit(1);
    }
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    return pid;
}

int main(void) {
    pid_t pid = start_program();

    while (1) {
        int status;
        pid_t ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            printf("process exit, restart\n");
            pid = start_program();
        } else if (ret < 0) {
            perror("waitpid");
            exit(1);
        }
        usleep(10 * 1000);
    }
}
```

## 进程间通信
### 匿名管道
```c
#include <unistd.h>
int pipe(int pipefd[2]);
// pipefd是一个长度为2的整数数组，pipefd[0]用于读取数据，pipefd[1]用于写入数据，成功返回0，失败返回-1
```
### 命名管道
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int mkfifo(const char *pathname, mode_t mode);
// pathname是命名管道的路径
// mode是命名管道的权限，常见的有：
// S_IRUSR: 读权限，文件所有者
// S_IWUSR: 写权限，文件所有者
// S_IRGRP: 读权限，文件所属组
// S_IWGRP: 写权限，文件所属组
// S_IROTH: 读权限，其他用户
// S_IWOTH: 写权限，其他用户
```
### 消息队列
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
// key是消息队列的键值，可以使用ftok函数生成
// msgflg是消息队列的标志，常见的有：
// IPC_CREAT: 如果消息队列不存在则创建
// IPC_EXCL: 与IPC_CREAT一起使用，如果消息队列已存在则返回错误
```
### 共享内存
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
// key是共享内存的键值，可以使用ftok函数生成
// size是共享内存的大小，单位是字节
// shmflg是共享内存的标志，常见的有：
// IPC_CREAT: 如果共享内存不存在则创建
// IPC_EXCL: 与IPC_CREAT一起使用，如果共享内存已存在则返回错误
```
### 信号量
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semget(key_t key, int nsems, int semflg);
// key是信号量集合的键值，可以使用ftok函数生成
// nsems是信号量集合中的信号量数量
// semflg是信号量集合的标志，常见的有：
// IPC_CREAT: 如果信号量集合不存在则创建
// IPC_EXCL: 与IPC_CREAT一起使用，如果信号量集合已存在则返回错误
```
