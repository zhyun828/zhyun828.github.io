CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -pthread
TARGET = slide_ctrl

SRCS = main.c mode.c gpio.c pwm.c eqep.c lis331hh.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
