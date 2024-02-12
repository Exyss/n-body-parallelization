CC := mpicc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean mpi_ext_1 mpi_ext_2 mpi_ext_3

mpi_ext_1: mpi_extensive_1.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

mpi_ext_2: mpi_extensive_2.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

mpi_ext_3: mpi_extensive_3.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o
	rm -f mpi_ext_*
