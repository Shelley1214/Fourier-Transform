CFLAGS = -O3
CC = g++

esse: esse.o complex.o filter.o
	$(CC) $(CFLAGS) -o output esse.o complex.o filter.o
	time ./output A1Image__2020-07-09__09-32-50.bmp

esse.o: esse.cpp 
	$(CC) $(CFLAGS) -c esse.cpp 

complex.o: complex.cpp 
	$(CC) $(CFLAGS) -c complex.cpp

filter.o: filter.cpp 
	$(CC) $(CFLAGS) -c filter.cpp

.PHONY: all
all: esse


.PHONY: clean
clean:
	rm -f *.o  

