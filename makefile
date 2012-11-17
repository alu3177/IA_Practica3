# makefile programa P3

CC = g++
CFLAGS = -ansi -pedantic
CDEBUG = -g
OBJS = main.o instancia.o problema.o genetics.o constructivas.o
MAIN = pract3
EXEC = pract3

MAIN: $(OBJS)
	$(CC) $(CDEBUG) $(CFLAGS) -o $(EXEC) $^

main.o: main.cpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

instancia.o: lib/instancia.cpp lib/instancia.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

problema.o: lib/problema.cpp lib/problema.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

genetics.o: lib/genetics.cpp lib/genetics.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

constructivas.o: lib/constructivas.cpp lib/constructivas.hpp
	$(CC) $(CDEBUG) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC) $@

