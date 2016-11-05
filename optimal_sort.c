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
		SORTTYPE t = m[i];
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
