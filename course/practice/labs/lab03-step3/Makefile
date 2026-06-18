CC = gcc
CFLAGS = -Wall -Wextra -g -O2

TARGET = lab03_step3
SRCS = framework/student.c framework/verify.c
OBJS = $(SRCS:.c=.o)

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
