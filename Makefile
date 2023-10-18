CC=gcc
CFLAGS=-Wall -Wextra -g -ggdb3
EXEC=main

$(EXEC): my_malloc.c main.c
	$(CC) $(CFLAGS) -o $@ $^

run:
	make && ./$(EXEC)

debug:
	make && gdb ./$(EXEC)

clean:
	rm -f $(EXEC) *.out 
