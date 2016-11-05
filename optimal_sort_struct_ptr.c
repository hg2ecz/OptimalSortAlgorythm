#include <stdio.h>
#include <stdlib.h>
#include "optimal_sort_struct.h"

static inline int elemcompare(const struct _SORTTYPE a, const struct _SORTTYPE b) {
    if (a.key < b.key) return -1;
    return a.key > b.key;
}


#ifdef ORIG_MEMCPY
#  include <string.h>
static inline void elemcopy(struct _SORTTYPE *dst, const struct _SORTTYPE *src) {
    memcpy(dst, src, sizeof(struct _SORTTYPE));
}
#else
static inline void elemcopy(struct _SORTTYPE *dst, const struct _SORTTYPE *src) {
    int len = sizeof(struct _SORTTYPE);
    for (int i=0; i < len>>2; i++) ((int *)dst)[i] = ((int *)src)[i];
    if (len & 3) {
	int lenbase=len & ~3u;
	for (int i=lenbase; i < lenbase + (len&3); i++) ((char *)dst)[i] = ((char *)src)[i];
    }
}
#endif

void optimal_sort_struct_ptr(struct _SORTTYPE *data_out, int *index_out, const struct _SORTTYPE *data_in, unsigned int log2len) {
    int len = 1<<log2len;
    int *src, *dst;
    int *indexmem;
    if (index_out) {
	indexmem = malloc(len*sizeof(int)); // dupla mem
	dst = index_out;
    } else {
	indexmem = malloc(2*len*sizeof(int)); // dupla mem
	dst = &indexmem[len];
    }
    if (!indexmem) {
	perror("Malloc error in optimal_sort\n");
	exit(0);
    }
    src = indexmem;

    if (log2len & 1) {
	int *tmpptr = dst;
	dst=src;
	src=tmpptr;
    }

    for (int i=0; i<len; i+=2) {
	if (elemcompare(data_in[i], data_in[i+1]) > 0) {
	    src[i  ] = i+1;
	    src[i+1] = i;
	} else {
	    src[i  ] = i;
	    src[i+1] = i+1;
	}
    }

    for (int step=4; step<=len; step<<=1) {
	for (int i=0; i<len; i+=step) {
	    int j;
	    int xstop = i+step/2;
	    int ystop = i+step;
	    int x=i, y=xstop;

	    for (j=i; x < xstop && y < ystop; j++) {
		if (elemcompare(data_in[src[x]], data_in[src[y]]) < 0) dst[j] = src[x++];
		else                                                   dst[j] = src[y++];
	    }
	    while (x < xstop) dst[j++] = src[x++];
	    while (y < ystop) dst[j++] = src[y++];
	}
	int *tmpptr = dst;
	dst = src;
	src = tmpptr;
    }

    if (data_out) {
	for (int i=0; i<len; i++) elemcopy(&data_out[i], &data_in[src[i]]);
    }
    free(indexmem);
}
