CC := mpicc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean mpi_exh_1 mpi_exh_2 mpi_exh_3

mpi_exh_1: mpi_exhaustive_1.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

mpi_exh_2: mpi_exhaustive_2.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

mpi_exh_3: mpi_exhaustive_3.o $(OBJS)
	mpicc $(CFLAGS) $(LIBRARIES) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o
	rm -f mpi_exh_*
