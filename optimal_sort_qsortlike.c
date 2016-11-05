#include <stdio.h>
#include <stdlib.h>
#include "optimal_sort_qsortlike.h"

#ifdef ORIG_MEMCPY
#  include <string.h>
static inline void cusmemcpy(void *dst, const void *src, int len) {
    memcpy(dst, src, len);
}
#else
static inline void cusmemcpy(void *dst, const void *src, int len) {
    for (int i=0; i < len>>2; i++) ((int *)dst)[i] = ((int *)src)[i];
    if (len & 3) {
	int lenbase=len & ~3u;
	for (int i=lenbase; i < lenbase + (len&3); i++) ((char *)dst)[i] = ((char *)src)[i];
    }
}
#endif


void optimal_qsortlike(void *base, int log2nmemb, size_t size, int (*compar)(const void *, const void *)) {
    int len = 1<<log2nmemb;
    void *src, *dst;
    void *tmp = malloc(len*size);
    if (!tmp) {
	perror("Malloc error in optimal_qsort\n");
	exit(0);
    }

    if (log2nmemb & 1) {
	for (int i=0; i<len; i+=2) {
	    if (compar(base + i*size, base + (i+1)*size) > 0) {
		cusmemcpy(tmp, base + i*size, size);
		cusmemcpy(base + i*size, base + (i+1)*size, size);
		cusmemcpy(base + (i+1)*size, tmp, size);
	    }
	}
	src = base;
	dst = tmp;
    } else {
	for (int i=0; i<len; i+=2) {
	    if (compar(base + i*size, base + (i+1)*size) > 0) {
		cusmemcpy(tmp + i*size, base + (i+1)*size, size);
		cusmemcpy(tmp + (i+1)*size, base + i*size, size);
	    } else { 
		cusmemcpy(tmp + i*size, base + i*size, 2*size);
	    }
	}
	src = tmp;
	dst = base;
    }

    for (int step=4; step<=len; step<<=1) {
	for (int i=0; i<len; i+=step) {
	    int j;
	    int xstop = i+step/2;
	    int ystop = i+step;
	    int x=i, y=xstop;

	    for (j=i; x < xstop && y < ystop; j++) {
		if (compar(src + x*size, src + y*size) < 0) cusmemcpy(dst + j*size, src + x++ * size, size);
		else                                        cusmemcpy(dst + j*size, src + y++ * size, size);
	    }
	    while (x < xstop) cusmemcpy(dst + j++ * size, src + x++ * size, size);
	    while (y < ystop) cusmemcpy(dst + j++ * size, src + y++ * size, size);
	}
	void *tmpptr = dst;
	dst = src;
	src = tmpptr;
    }
    free(tmp);
}
