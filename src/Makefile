OBJ= utils.o oscil.o wav.o
CFLAGS= -Wall -ansi -pedantic-errors

all:$(OBJ)
	$(CC) -o main $^ -lm main_linux.c $(CFLAGS)

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< 

clean:
	rm -rf *.o
