#include <stdio.h>
#include <stdlib.h>
#include "optimal_sort.h"

void optimal_sort(SORTTYPE *m, unsigned int log2len) {
    int len = 1<<log2len;
    SORTTYPE *src, *dst;
    SORTTYPE *tmp = malloc(len*sizeof(SORTTYPE));
    if (!tmp) {
	perror("Malloc error in optimal_sort\n");
	exit(0);
    }

    if (log2len & 1) {
	for (int i=0; i<len; i+=2) {
	    if (m[i] > m[i+1]) {
		int t = m[i];
		m[i  ] = m[i+1];
		m[i+1] = t;
	    }
	}
	src = m;
	dst = tmp;
    } else {
	for (int i=0; i<len; i+=2) {
	    if (m[i] > m[i+1]) {
		tmp[i  ] = m[i+1];
		tmp[i+1] = m[i  ];
	    } else {
		tmp[i  ] = m[i  ];
		tmp[i+1] = m[i+1];
	    }
	}
	src = tmp;
	dst = m;
    }

    for (int step=4; step<=len; step<<=1) {
	for (int i=0; i<len; i+=step) {
	    int j;
	    int xstop = i+step/2;
	    int ystop = i+step;
	    int x=i, y=xstop;

	    for (j=i; x < xstop && y < ystop; j++) {
		if (src[x] <= src[y]) dst[j] = src[x++];
		else                  dst[j] = src[y++];
	    }
	    while (x < xstop) dst[j++] = src[x++];
	    while (y < ystop) dst[j++] = src[y++];
	}
	SORTTYPE *tmpptr = dst;
	dst = src;
	src = tmpptr;
    }
    free(tmp);
}


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
    void *tmp_1word = malloc(size);
    if (!tmp) {
	perror("Malloc error in optimal_qsort\n");
	exit(0);
    }

    if (log2nmemb & 1) {
	for (int i=0; i<len; i+=2) {
	    if (compar(base + i*size, base + (i+1)*size) > 0) {
		cusmemcpy(tmp_1word, base + i*size, size);
		cusmemcpy(base + i*size, base + (i+1)*size, size);
		cusmemcpy(base + (i+1)*size, tmp_1word, size);
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

    free(tmp_1word);
    free(tmp);
}
