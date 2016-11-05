#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "optimal_sort.h"
#include "optimal_sort_struct.h"
#include "optimal_sort_qsortlike.h"

//#define LOG2NUM 20
#define NUM (1<<LOG2NUM)

static double difftime_ms(struct timespec gstart, struct timespec gend) {
    unsigned long long nsec = 1000ULL*1000*1000*(gend.tv_sec - gstart.tv_sec) + gend.tv_nsec - gstart.tv_nsec;
    return nsec/1000./1000.; // msec
}

static int qsort_compare(const void *ain, const void *bin) {
    const int *a=(const int *)ain;
    const int *b=(const int *)bin;
    if (*a<*b) return -1;
    return *a>*b;
}

static int qsort_compare_struct(const void *ain, const void *bin) {
    const struct _SORTTYPE *a=(const struct _SORTTYPE *)ain;
    const struct _SORTTYPE *b=(const struct _SORTTYPE *)bin;
    if (a->key < b->key) return -1;
    return a->key > b->key;
}

int main() {
    struct timespec t1, t2, t3, t4, t5, t6, t7, t8, t9;

    int *mi_orig = malloc(NUM*sizeof(int));
    int *mi_qsort = malloc(NUM*sizeof(int));
    int *mi_optimal = malloc(NUM*sizeof(int));
    int *mi_qsortlike = malloc(NUM*sizeof(int));

    int *ms_optimal_outptr = malloc(NUM*sizeof(int));

    struct _SORTTYPE *ms_qsort = malloc(NUM*sizeof(struct _SORTTYPE));
    struct _SORTTYPE *ms_optimal_struct = malloc(NUM*sizeof(struct _SORTTYPE));
    struct _SORTTYPE *ms_optimal_in = malloc(NUM*sizeof(struct _SORTTYPE));
    struct _SORTTYPE *ms_optimal_outdata = malloc(NUM*sizeof(struct _SORTTYPE));
    struct _SORTTYPE *ms_qsortlike = malloc(NUM*sizeof(struct _SORTTYPE));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1); srandom(t1.tv_nsec);
    for (int i=0; i<NUM; i++) {
	mi_orig[i] =
	mi_qsort[i] =
	mi_optimal[i] =
	mi_qsortlike[i] =
	ms_qsort[i].key =
	ms_optimal_struct[i].key =
	ms_optimal_in[i].key =
	ms_qsortlike[i].key =
	random();
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

    qsort(mi_qsort, NUM, sizeof(int), qsort_compare);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    optimal_sort(mi_optimal, LOG2NUM);                   // optimal sort
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t3);
    optimal_qsortlike(mi_qsortlike, LOG2NUM, sizeof(int), qsort_compare); // optimal qsort-like
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t4);

    // struct
    qsort(ms_qsort, NUM, sizeof(struct _SORTTYPE), qsort_compare_struct);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t5);

    optimal_sort_struct(ms_optimal_struct, LOG2NUM);        // optimal sort struct
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t6);

    optimal_sort_struct_ptr(NULL, ms_optimal_outptr, ms_optimal_in, LOG2NUM);    // optimal sort struct - sort pointer
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t7);
    optimal_sort_struct_ptr(ms_optimal_outdata, NULL, ms_optimal_in, LOG2NUM);    // optimal sort struct - sort pointer
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t8);

    optimal_qsortlike(ms_qsortlike, LOG2NUM, sizeof(struct _SORTTYPE), qsort_compare_struct); // optimal qsort-like
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t9);

    puts("orig. data : glibc qsort, optimal sort, qsort-like : str. qsort, optimal, opt. ptr, optimal pdata, qsort-like");
    puts("First 10:");
    for (int i=0; i<10 && i<NUM; i++) {
	printf(" %10d : %10d %10d %10d : %10d %10d %10d %10d %10d\n", mi_orig[i],
	    mi_qsort[i], mi_optimal[i], mi_qsortlike[i],
	    ms_qsort[i].key, ms_optimal_struct[i].key, ms_optimal_in[ms_optimal_outptr[i]].key,
	    ms_optimal_outdata[i].key, ms_qsortlike[i].key);
    }

    puts("Last 5:");
    for (int i=NUM-5; i<NUM; i++) {
	printf(" %10d : %10d %10d %10d : %10d %10d %10d %10d %10d\n", mi_orig[i],
	    mi_qsort[i], mi_optimal[i], mi_qsortlike[i],
	    ms_qsort[i].key, ms_optimal_struct[i].key, ms_optimal_in[ms_optimal_outptr[i]].key,
	    ms_optimal_outdata[i].key, ms_qsortlike[i].key);
    }

    printf("\n%d elements - basic variable types (for example: int):\n", NUM);
    printf("  glibc qsort:        %.2f ms\n", difftime_ms(t1, t2));
    printf("  optimal sort:       %.2f ms\n", difftime_ms(t2, t3));
    printf("  qsort-like generic: %.2f ms\n", difftime_ms(t3, t4));

    printf("\n%d elements, sizeof(struct _SORTTYPE) = %d bytes\n", NUM, sizeof(struct _SORTTYPE));
    printf("  glibc qsort:        %.2f ms\n", difftime_ms(t4, t5));
    printf("  optimal struct:     %.2f ms\n", difftime_ms(t5, t6));
    printf("  optimal struct ptr: %.2f ms - pointer only\n", difftime_ms(t6, t7));
    printf("  optimal struct ptr: %.2f ms - data\n", difftime_ms(t7, t8));
    printf("  qsort-like struct:  %.2f ms\n", difftime_ms(t8, t9));
    puts("");

    return 0;
}