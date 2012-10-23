# makefile programa P3

CC = g++
CFLAGS = -ansi -pedantic
CDEBUG = -g
OBJS = main.o
MAIN = pract3
EXEC = pract3

MAIN: $(OBJS)
	$(CC) $(CDEBUG) $(CFLAGS) -o $(EXEC) $^

main.o: main.cpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC) $@

