CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
MATRIZADY = src/matrizAdy.o
INPUTOUTPUT = src/inputOutput.o

main: main.c $(MATRIZADY) $(INPUTOUTPUT)
	$(CC) main.c $(MATRIZADY) $(INPUTOUTPUT) -o main.o $(CFLAGS)

$(MATRIZADY): src/matrizAdy.c
	$(CC) src/matrizAdy.c -c -o $(MATRIZADY) $(CFLAGS)

$(INPUTOUTPUT): src/inputOutput.c
	$(CC) src/inputOutput.c -c -o $(INPUTOUTPUT) $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf src/*.o
