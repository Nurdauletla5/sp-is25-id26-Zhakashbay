CC=gcc
CFLAGS=-Wall -Wextra -std=c11

all: parallel_sum

parallel_sum: parallel_sum.c
	$(CC) $(CFLAGS) parallel_sum.c -o parallel_sum

clean:
	rm -f parallel_sum
