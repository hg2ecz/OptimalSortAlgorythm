#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "optimal_sort.h"

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

int main() {
    struct timespec t1, t2, t3;

    int *m1=malloc(NUM*sizeof(int));
    int *m2=malloc(NUM*sizeof(int));

    for (int i=0; i<NUM; i++) m1[i] = m2[i] = random();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    qsort(m1, NUM, sizeof(int), qsort_compare);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    optimal_sort(m2, LOG2NUM); // optimal sort
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t3);

    puts("First 20: (glibc qsort, optimal sort):");
    for (int i=0; i<20; i++) {
	printf(" %10d %10d\n", m1[i], m2[i]);
    }
    
    puts("Last 10: (glibc qsort, optimal sort):");
    for (int i=NUM-10; i<NUM; i++) {
	printf(" %10d %10d\n", m1[i], m2[i]);
    }

    puts("\nTimes:");
    printf("  glibc qsort:  %.2f ms\n", difftime_ms(t1, t2));
    printf("  optimal sort: %.2f ms\n", difftime_ms(t2, t3));
    return 0;
}
