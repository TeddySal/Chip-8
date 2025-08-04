CFLAGS = -Wall -Wextra -pedantic $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

main: main.c chip_8.c instructions.c
	$(CC) -o main main.c chip_8.c instructions.c $(CFLAGS) $(LDFLAGS)
