CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pthread

all: producer_consumer

producer_consumer: producer_consumer.c
	$(CC) $(CFLAGS) producer_consumer.c -o producer_consumer

clean:
	rm -f producer_consumer

