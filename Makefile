CC=gcc
CFLAGS=-Wall -Wextra -g -ggdb3

EXEC=main
TESTS=tests

all: $(EXEC) $(TESTS)

$(EXEC): my_malloc.c main.c
	$(CC) $(CFLAGS) -o $@ $^

$(TESTS): my_malloc.c tests.c
	$(CC) $(CFLAGS) -o $@ $^

run:
	make && ./$(EXEC)

test:
	make && ./$(TESTS)

clean:
	find . -type f -executable -not -path "./.git/*" -not -name "*.sh" | xargs -r rm

.PHONY: all run test debug clean
