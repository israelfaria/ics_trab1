OBJ= utils.o oscil.o wav.o
OBJ_TEST= oscil_test.o all_tests.o
CFLAGS= -Wall -ansi -pedantic-errors -g

all:$(OBJ)
	$(CC) -o main $^ -lm main_linux.c $(CFLAGS)
check:$(OBJ) $(OBJ_TEST)
	$(CC) -o all_tests $^ -lm -lcunit $(CFLAGS)

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< 

clean:
	rm -rf *.o
