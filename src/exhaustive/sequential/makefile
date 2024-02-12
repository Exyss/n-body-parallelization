CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean seq_exh_1 seq_exh_2

seq_exh_1: seq_exhaustive_1.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@

seq_exh_2: seq_exhaustive_2.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o
	rm -f seq_exh_*