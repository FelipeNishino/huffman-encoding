DIR := ${CURDIR}

exec = huffenc
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -fPIC -std=c99

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm $(exec)
	-rm src/*.o
