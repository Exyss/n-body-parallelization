CC := mpicc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean mpi_bh

mpi_bh: mpi_barnes_hut.o bh_octree.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f $(COMMON_BH_DIR)/*.o
	rm -f *.o
