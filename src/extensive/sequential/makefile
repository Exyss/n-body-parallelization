CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean seq_ext_1 seq_ext_2

seq_ext_1: seq_extensive_1.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@

seq_ext_2: seq_extensive_2.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o