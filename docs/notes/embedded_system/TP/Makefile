# 编译器和编译选项
CC = gcc                           # C编译器
CFLAGS = -Wall -Wextra -O2        # 编译标志: 所有警告, 额外警告, 优化等级2

# 链接选项
LDFLAGS = -pthread                # 链接选项: 启用线程库

# 目标可执行文件名
TARGET = slide_ctrl

# 源代码文件列表(*.c文件)
SRCS = main.c mode.c gpio.c pwm.c eqep.c lis331hh.c

# 目标文件列表(自动从SRCS生成)
OBJS = $(SRCS:.c=.o)

# 默认目标: 编译可执行文件
all: $(TARGET)

# 链接规则: 从目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# 编译规则: 从源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# 清理规则: 删除编译生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
