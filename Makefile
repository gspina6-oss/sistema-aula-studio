CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

PROGRAMMA = aula_studio
TEST = test_aula_studio

SRC = src/main.c src/aula_studio.c
SRC_TEST = test/test_aula_studio.c src/aula_studio.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAMMA)

run: all
	./$(PROGRAMMA)

test:
	$(CC) $(CFLAGS) $(SRC_TEST) -o $(TEST)
	./$(TEST)

clean:
	rm -f $(PROGRAMMA) $(TEST) aula_studio.exe test_aula_studio.exe
