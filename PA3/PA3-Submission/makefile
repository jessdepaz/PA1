CC=gcc
CFLAGS=-I. -lm -w
DEPS= allocator.h
OBJ = allocator.o first_fit.o next_fit.o best_fit.o worst_fit.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
pa3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
