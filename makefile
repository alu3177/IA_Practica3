# makefile programa P3

CC = g++
CFLAGS = -ansi -pedantic
CDEBUG = -g
OBJS = main.o baseClass.o constructivas.o localSearches.o genetics.o
MAIN = pract3
EXEC = pract3

MAIN: $(OBJS)
	$(CC) $(CDEBUG) $(CFLAGS) -o $(EXEC) $^

main.o: main.cpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

baseClass.o: lib/baseClass.cpp lib/baseClass.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

constructivas.o: lib/constructivas.cpp lib/constructivas.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

localSearches.o: lib/localSearches.cpp lib/localSearches.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

genetics.o: lib/genetics.cpp lib/genetics.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC) $@

