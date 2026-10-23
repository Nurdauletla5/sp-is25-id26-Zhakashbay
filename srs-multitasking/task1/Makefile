CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread
TARGETS = procinfo myps

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
