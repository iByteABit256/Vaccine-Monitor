# the compiler to use
CC = gcc

# compiler flags:
#  -ggdb3 adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -ggdb3 -Wall
  
# linker flags:
LFLAGS =

# libraries to link with
LIBS =
  
# the name to use for both the target source file, and the output file:
TARGET = vaccineMonitor

# source files
SRC = src/vaccineMonitor.c src/main.c lib/hash/hash.c lib/bloomfilter/bloomfilter.c\
	  lib/lists/lists.c lib/skiplist/skiplist.c lib/hashtable/hashtable.c src/parser.c

# object files
OBJ = vaccineMonitor.o main.o hash.o bloomfilter.o lists.o skiplist.o hashtable.o parser.o

# make all by default
default: all

all: $(TARGET)

# make target
$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

# make object files
bloomfilter.o: lib/bloomfilter/bloomfilter.c lib/hash/hash.h
	$(CC) $(CFLAGS) -c lib/bloomfilter/bloomfilter.c
vaccineMonitor.o: src/vaccineMonitor.c lib/bloomfilter/bloomfilter.h
	$(CC) $(CFLAGS) -c src/vaccineMonitor.c
hash.o: lib/hash/hash.c
	$(CC) $(CFLAGS) -c lib/hash/hash.c
main.o: src/main.c lib/skiplist/skiplist.h\
	lib/bloomfilter/bloomfilter.h lib/hashtable/htInterface.h src/vaccineMonitor.h
	$(CC) $(CFLAGS) -c src/main.c
lists.o: lib/lists/lists.c
	$(CC) $(CFLAGS) -c lib/lists/lists.c
skiplist.o: lib/skiplist/skiplist.c
	$(CC) $(CFLAGS) -c lib/skiplist/skiplist.c
hashtable.o: lib/hashtable/hashtable.c lib/lists/lists.h
	$(CC) $(CFLAGS) -c lib/hashtable/hashtable.c
parser.o: src/parser.c lib/hashtable/hashtable.c
	$(CC) $(CFLAGS) -c src/parser.c

# clean up
clean:
	-rm -f $(TARGET)
	-rm -f $(OBJ)
