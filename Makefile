CC=gcc
CFLAGS=-Wall -Wextra -g -ggdb3 -Wno-unused-variable

EXEC=main
TESTS=tests

all: $(EXEC) $(TESTS)

$(EXEC): my_malloc.c utils.h main.c
	$(CC) $(CFLAGS) -o $@ $^

$(TESTS): my_malloc.c utils.h tests.c
	$(CC) $(CFLAGS) -o $@ $^

run:
	make && ./$(EXEC)

test:
	make && ./$(TESTS)

clean:
	find . -type f -executable -not -path "./.git/*" -not -name "*.sh" | xargs -r rm

.PHONY: all run test debug clean
