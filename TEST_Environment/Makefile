CC = gcc
CFLAGS = -Wall -g -O0
CTHREAD = -pthread

SRC = ${wildcard *.c}
OBJS = ${patsubst %.c,%,$(SRC)}


all:$(OBJS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $^ $(CTHREAD)
clean:
	$(RM) $(OBJS) .*.sw?
