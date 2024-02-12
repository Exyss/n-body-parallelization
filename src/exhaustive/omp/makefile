CC := gcc
CFLAGS := -Wall -Wextra -O3 -ggdb3
LIBRARIES := -lm
OMPFLAG := -fopenmp

DIR := ../../core
SRCS := $(wildcard $(DIR)/*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all 
all: clean omp_exh_1 omp_exh_2 omp_exh_3 omp_exh_4

omp_exh_1: omp_exhaustive_1.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_exh_2: omp_exhaustive_2.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_exh_3: omp_exhaustive_3.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

omp_exh_4: omp_exhaustive_4.o $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) $(OMPFLAG) -c $< -o $@

clean:
	rm -f $(DIR)/*.o
	rm -f *.o
	rm -f omp_exh_*