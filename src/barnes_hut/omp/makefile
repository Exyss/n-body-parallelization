CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm
OMPFLAG := -fopenmp

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)
.PHONY: all 
all: clean omp_bh

omp_bh: omp_barnes_hut.o bh_octree.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) -c $< -o $@ 

clean:
	rm -f $(DIR)/*.o
	rm -f $(COMMON_BH_DIR)/*.o
	rm -f *.o