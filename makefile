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
TARGET = main

# source files
SRC = main.c hash/hash.c bloomfilter/bloomfilter.c

# object files
OBJ = main.o hash.o bloomfilter.o

# make all by default
default: all

all: $(TARGET)

# make target
$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

# make object files
bloomfilter.o: bloomfilter/bloomfilter.c hash/hash.h
	$(CC) $(CFLAGS) -c bloomfilter/bloomfilter.c
main.o: main.c bloomfilter/bloomfilter.h
	$(CC) $(CFLAGS) -c main.c
hash.o: hash/hash.c
	$(CC) $(CFLAGS) -c hash/hash.c

# clean up
clean:
	-rm -f $(TARGET)
	-rm -f $(OBJ)

