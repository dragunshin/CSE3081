#ifndef MY_QUICK_SORTS_H
#define MY_QUICK_SORTS_H

#include <cstddef>

// Comparison function type
typedef int _Cmpfun(const void *, const void *);

// Function 1: Standard library qsort wrapper
void my_qsort(void *base, size_t num, size_t size, _Cmpfun *cmp);

// Function 21: Original quicksort implementation
void my_qsort_orig(void *base, size_t num, size_t size, _Cmpfun *cmp);

// Function 22: Quicksort with median-of-three and insertion sort optimization
void my_qsort_median_insert(void *base, size_t num, size_t size, _Cmpfun *cmp);

// Function 23: Iterative quicksort with median-of-three and insertion sort optimization
void my_qsort_median_insert_iter(void *base, size_t num, size_t size, _Cmpfun *cmp);

// Function 24: Final optimized quicksort implementation
void my_qsort_final(void *base, size_t num, size_t size, _Cmpfun *cmp);

// Function 24 variants
void my_qsort_final_32bit(void *base, size_t num, size_t size, _Cmpfun *cmp);
void my_qsort_final_pseudo(void *base, size_t num, size_t size, _Cmpfun *cmp);


#endif // MY_QUICK_SORTS_H
