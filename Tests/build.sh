cc -o main main.c chip_8.c instructions.c $(pkg-config --libs --cflags raylib)
