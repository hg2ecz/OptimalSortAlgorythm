CC=gcc

CFLAGS =-Wall -O2

# ARM and x86
CFLAGS+=-mcpu=cortex-a5 -mfpu=neon-fp16
#CFLAGS+=-march=native

CFLAGS+=-DLOG2NUM=20

LDFLAGS=-lm -s
OBJS=optimal_sort.o benchmark.o

TARGET=benchmark

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)
