CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm
OMPFLAG := -fopenmp

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean omp_ext_1 omp_ext_2 omp_ext_3 omp_ext_4

omp_ext_1: omp_extensive_1.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_ext_2: omp_extensive_2.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_ext_3: omp_extensive_3.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_ext_4: omp_extensive_4.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o