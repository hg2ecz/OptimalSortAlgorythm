#include <stdio.h>
#include <stdlib.h>
#include "optimal_sort_struct.h"


static inline int elemcompare(const struct _SORTTYPE a, const struct _SORTTYPE b) {
    if (a.key < b.key) return -1;
    return a.key > b.key;
}


static inline void elemcopy(struct _SORTTYPE *dst, const struct _SORTTYPE *src) {
    int len = sizeof(struct _SORTTYPE);
    for (int i=0; i < len>>2; i++) ((int *)dst)[i] = ((int *)src)[i];
    if (len & 3) {
	int lenbase=len & ~3u;
	for (int i=lenbase; i < lenbase + (len&3); i++) ((char *)dst)[i] = ((char *)src)[i];
    }
}


void optimal_sort_struct(struct _SORTTYPE *m, unsigned int log2len) {
    int len = 1<<log2len;
    struct _SORTTYPE *src, *dst;
    struct _SORTTYPE *tmp = malloc(len*sizeof(struct _SORTTYPE));
    if (!tmp) {
	perror("Malloc error in optimal_sort\n");
	exit(0);
    }

    if (log2len & 1) {
	for (int i=0; i<len; i+=2) {
	    if (elemcompare(m[i], m[i+1]) > 0) {
		struct _SORTTYPE t;
		elemcopy(&t, &m[i]);
		elemcopy(&m[i], &m[i+1]);
		elemcopy(&m[i+1], &t);
	    }
	}
	src = m;
	dst = tmp;
    } else {
	for (int i=0; i<len; i+=2) {
	    if (elemcompare(m[i], m[i+1]) > 0) {
		elemcopy(&tmp[i+1], &m[i  ]);;
		elemcopy(&tmp[i  ], &m[i+1]);;
	    } else {
		elemcopy(&tmp[i  ], &m[i  ]);;
		elemcopy(&tmp[i+1], &m[i+1]);;
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
		if (elemcompare(src[x], src[y]) < 0) elemcopy(&dst[j], &src[x++]);
		else                                 elemcopy(&dst[j], &src[y++]);
	    }
	    while (x < xstop) elemcopy(&dst[j++], &src[x++]);
	    while (y < ystop) elemcopy(&dst[j++], &src[y++]);
	}
	struct _SORTTYPE *tmpptr = dst;
	dst = src;
	src = tmpptr;
    }
    free(tmp);
}
