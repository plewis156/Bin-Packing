CFLAGS = -Wall 
CC = gcc

objects = binPacking.o bin.o

genetic: $(objects)
	$(CC) $(CFLAGS) -o binPacking $(objects)

project4.o: binPacking.c binPacking.h bin.h
bin.o: bin.c bin.h

.PHONY : clean
clean: 
	rm binPacking $(objects)
