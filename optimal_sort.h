#define SORTTYPE int
void optimal_sort(SORTTYPE *m, unsigned int log2len);
void optimal_qsortlike(void *base, int log2nmemb, size_t size, int (*compar)(const void *, const void *));
