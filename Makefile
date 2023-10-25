CC=gcc
CFLAGS=-Wall -Wextra -g -ggdb3 -Wno-unused-variable

BINDIR=bin
SRCDIR=src
TESTDIR=test

TESTBIN=$(BINDIR)/tests

all: $(BINDIR) $(TESTBIN)

$(BINDIR):
	mkdir $@

$(TESTBIN): $(SRCDIR)/my_malloc.c $(TESTDIR)/utils.h $(TESTDIR)/framework.h $(TESTDIR)/tests.c
	$(CC) $(CFLAGS) -o $@ $^

# Commands
.PHONY: all run test debug clean

test: $(BINDIR) $(TESTBIN)
	./$(TESTBIN)

clean: 
	rm -rf $(BINDIR)	
