CC=gcc
CFLAGS=-I. -lm -w
DEPS= queue.h simulate.h pqueue.h
OBJ = simulate.o queue.o sjf.o srt.o priority.o fcfs.o pqueue.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
pa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
