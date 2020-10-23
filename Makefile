COMPILER = g++
FLAGS = -c

all: copy

copy: copy.o 
	$(COMPILER) copy.o -o copy

copy.o: copy.cpp
	$(COMPILER) $(FLAGS) copy.cpp -o copy.o

clean:
	rm *.o copy
