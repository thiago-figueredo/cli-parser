CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -std=c11
EXE=cli_parser
SRCS=main.c cli.c arg.c helpers.c list.c

all: 
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) 

clean:
	rm -f $(EXE) *.o main cli