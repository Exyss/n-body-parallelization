CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean seq_bh_1 seq_bh_2

seq_bh_1: seq_barnes_hut_1.o bh_octree_1.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@ 

seq_bh_2: seq_barnes_hut_2.o bh_octree_2.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@ 

clean:
	rm -f $(DIR)/*.o
	rm -f $(COMMON_BH_DIR)/*.o
	rm -f *.o
	rm -f seq_bh_*