struct _SORTTYPE {
    int key;
    char value[60];
};

void optimal_sort_struct(struct _SORTTYPE *m, unsigned int log2len);

// if dest    is NULL --> no copy
// if destptr is NULL --> no ptr
void optimal_sort_struct_ptr(struct _SORTTYPE *dest, int *destptr, const struct _SORTTYPE *src, unsigned int log2len);
