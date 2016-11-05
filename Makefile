CC=gcc

CFLAGS =-Wall -O2

# ARM and x86 specific
#CFLAGS+=-mcpu=cortex-a5 -mfpu=neon-fp16
#CFLAGS+=-march=native

CFLAGS+=-DLOG2NUM=20

# call string.h memcpy in qsort-like function -- much slower(!)
#CFLAGS+=-DORIG_MEMCPY

LDFLAGS=-lm -s
OBJS=optimal_sort.o optimal_sort_struct.o optimal_sort_struct_ptr.o optimal_sort_qsortlike.o benchmark.o

TARGET=benchmark

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)
