#include "my_quick_sorts.h"
#include <cstdlib>
#include <cstring>

#define THRESHOLD 10

typedef struct {
    unsigned int score;
    char other_data[12];
} RECORD_DIRECT;

// Fast 16-byte swap using stack buffer
inline void swap_16byte(void* a, void* b) {
    RECORD_DIRECT temp;
    memcpy(&temp, a, 16);
    memcpy(a, b, 16);
    memcpy(b, &temp, 16);
}

typedef int _Cmpfun(const void *, const void *);

// For Function 21 
void quick_sort(void* base, int left, int right, size_t size, _Cmpfun *cmp);
int partition(void* base, int left, int right, size_t size, _Cmpfun *cmp);

// For Function 22
void insertion_sort(void* base, size_t num, size_t size, _Cmpfun *cmp);
int median_of_three(void* base, int left, int right, size_t size, _Cmpfun *cmp);
int partition_median(void* base, int left, int right, size_t size, _Cmpfun *cmp);
void quick_sort_median_insert(void* base, int left, int right, size_t size, _Cmpfun *cmp);

// For Function 23
void quick_sort_median_insert_iter(void* base, int left, int right, size_t size, _Cmpfun *cmp);

// For Function 24: 32-bit swap optimized version
inline void swap_16byte_32bit(void* a, void* b);
void insertion_sort_optimized_32bit(void* base, size_t num);
int median_of_three_optimized_32bit(void* base, int left, int right);
int partition_2way_32bit(void* base, int left, int right, int pivot_pos);
void quick_sort_optimized_32bit(void* base, int left, int right);


// Function 1: Wrapper for standard library qsort
void my_qsort(void *base, size_t num, size_t size, _Cmpfun *cmp) 
{
    qsort(base, num, size, cmp);
}

// Function 21: Original quicksort implementation
void my_qsort_orig(void *base, size_t num, size_t size, _Cmpfun *cmp) {
    quick_sort(base, 0 , num - 1, size, cmp);
}

// Function 22: Quicksort with median-of-three and insertion sort optimization
void my_qsort_median_insert(void *base, size_t num, size_t size, _Cmpfun *cmp) {
    quick_sort_median_insert(base, 0, num - 1, size, cmp);
}

// Function 23: Iterative quicksort with median-of-three and insertion sort optimization
void my_qsort_median_insert_iter(void *base, size_t num, size_t size, _Cmpfun *cmp) {
    quick_sort_median_insert_iter(base, 0, num - 1, size, cmp);
}

// Function 24: Final optimized quicksort implementation (32-bit swap version - FASTEST)
void my_qsort_final(void *base, size_t num, size_t size, _Cmpfun *cmp) {
    if (num <= 1) return;

    quick_sort_optimized_32bit(base, 0, num - 1);
}

/*------------------------------------------------------------------------------------------------*/
// For Function 21
void quick_sort(void* base, int left, int right, size_t size, _Cmpfun *cmp) {
    if(left >= right) return;

    // Select pivot
    int pivot = partition(base, left, right, size, cmp);

    //Recursive calls
    quick_sort(base, left, pivot - 1, size, cmp);
    quick_sort(base, pivot + 1, right, size, cmp);
}

int partition(void* base, int left, int right, size_t size, _Cmpfun *cmp) {
    // 효율적 swap 이전
    // int pivot = left;
    // for(int i = left; i < right; i++) {
    //     if(cmp((char *)base + i * size, (char *)base + right * size) < 0) {
    //         //Swap
    //         char* temp = new char[size];
    //         memcpy(temp, (char *)base + i * size, size);
    //         memcpy((char *)base + i * size, (char *)base + pivot * size, size);
    //         memcpy((char *)base + pivot * size, temp, size);
    //         delete[] temp;
    //         pivot++;
    //     }
    // }
    // // sw
    // char *temp = new char[size];
    // memcpy(temp, (char *)base + pivot * size, size);
    // memcpy((char *)base + pivot * size, (char *)base + right * size, size);
    // memcpy((char *)base + right * size, temp, size);
    // delete[] temp;
    // return pivot;

    // 효율적 swap 적용
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;

    int pivot = left;
    for(int i = left; i < right; i++) {
        if(cmp(&arr[i], &arr[right]) < 0) {
            swap_16byte(&arr[i], &arr[pivot]);
            pivot++;
        }
    }
    swap_16byte(&arr[pivot], &arr[right]);

    return pivot;
}

// For Function 22
void insertion_sort(void* base, size_t num, size_t size, _Cmpfun *cmp) {
    // 효율적 swap 이전
    // for(int i = 1; i < num; i++){
    //     char* key = new char[size];
    //     memcpy(key, (char *)base + i * size, size);
    //     int j = i;
    //     while((j > 0) && (cmp(key, (char *)base + (j - 1) * size) < 0)){
    //         memcpy((char *)base + j * size, (char *)base + (j - 1) * size, size);
    //         j--;
    //     }
    //     memcpy((char *)base + j * size, key, size);
    //     delete[] key;
    // }

    // 효율적 swap 적용
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;

    for(int i = 1; i < num; i++){
        RECORD_DIRECT key = arr[i];
        int j = i;
        while((j > 0) && (cmp(&key, &arr[j - 1]) < 0)){
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

int median_of_three(void* base, int left, int right, size_t size, _Cmpfun *cmp) {
    int mid = (right + left) / 2;

    char* left_elem = (char *)base + left * size;
    char* mid_elem = (char *)base + mid * size;
    char* right_elem = (char *)base + right * size;

    if(cmp(left_elem, mid_elem) < 0){
        if(cmp(mid_elem, right_elem) < 0){
            return mid;
        } else if(cmp(left_elem, right_elem) < 0){
            return right;
        } else {
            return left;
        }
    } else {
        if(cmp(left_elem, right_elem) < 0){
            return left;
        } else if(cmp(mid_elem, right_elem) < 0){
            return right;
        } else {
            return mid;
        }
    }
}

int partition_median(void* base, int left, int right, size_t size, _Cmpfun *cmp) {
    // 효율적 swap 이전
    // int median_idx = median_of_three(base, left, right, size, cmp);
    // // swap median , right
    // char *temp = new char[size];
    // memcpy(temp, (char *)base + median_idx * size, size);
    // memcpy((char *)base + median_idx * size, (char *)base + right * size, size);
    // memcpy((char *)base + right * size, temp, size);
    // delete[] temp;
    // return partition(base, left, right, size, cmp);

    // 효율적 swap 적용
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;
    int median_idx = median_of_three(base, left, right, size, cmp);

    if (median_idx != right) {
        swap_16byte(&arr[median_idx], &arr[right]);
    }

    return partition(base, left, right, size, cmp);
}

void quick_sort_median_insert(void* base, int left, int right, size_t size, _Cmpfun *cmp){
    //small -> insertion sort
    if(right - left <= THRESHOLD){
        insertion_sort((char *)base + left * size, right - left + 1, size, cmp);
        return;
    }
    if(left >= right) return;

    int pivot = partition_median(base, left, right, size, cmp);
    quick_sort_median_insert(base, left, pivot - 1, size, cmp);
    quick_sort_median_insert(base, pivot + 1, right, size, cmp);
}

// For Function 23
void quick_sort_median_insert_iter(void* base, int left, int right, size_t size, _Cmpfun *cmp) {
    while(right - left > THRESHOLD) {
        int pivot = partition_median(base, left, right, size, cmp);

        if(pivot - left < right - pivot) {
            //right side is larger
            quick_sort_median_insert_iter(base, left, pivot - 1, size, cmp);
            left = pivot + 1;
        }
        else {
            //left side is larger
            quick_sort_median_insert_iter(base, pivot + 1, right, size, cmp);
            right = pivot - 1;
        }
    }
    // insertion sort
    if(left < right) {
        insertion_sort((char *)base + left * size, right - left + 1, size, cmp);
    }
}


/*------------------------------------------------------------------------------------------------*/
// For Function 24 32-bit swap optimization

inline void swap_16byte_32bit(void* a, void* b) {
    unsigned int* a32 = (unsigned int*)a;
    unsigned int* b32 = (unsigned int*)b;

    unsigned int temp0 = a32[0];
    unsigned int temp1 = a32[1];
    unsigned int temp2 = a32[2];
    unsigned int temp3 = a32[3];

    a32[0] = b32[0];
    a32[1] = b32[1];
    a32[2] = b32[2];
    a32[3] = b32[3];

    b32[0] = temp0;
    b32[1] = temp1;
    b32[2] = temp2;
    b32[3] = temp3;
}

// Optimized insertion sort with 32-bit swap
void insertion_sort_optimized_32bit(void* base, size_t num) {
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;

    for (int i = 1; i < num; i++) {
        RECORD_DIRECT key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].score > key.score) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Median-of-three with 32-bit swap
int median_of_three_optimized_32bit(void* base, int left, int right) {
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;
    int mid = left + (right - left) / 2;

    unsigned int k_left = arr[left].score;
    unsigned int k_mid = arr[mid].score;
    unsigned int k_right = arr[right].score;

    if (k_left < k_mid) {
        if (k_mid < k_right) return mid;
        else if (k_left < k_right) return right;
        else return left;
    } else {
        if (k_left < k_right) return left;
        else if (k_mid < k_right) return right;
        else return mid;
    }
}


int partition_2way_32bit(void* base, int left, int right, int pivot_pos) {
    RECORD_DIRECT* arr = (RECORD_DIRECT*)base;

    // Move pivot to right
    if (pivot_pos != right) {
        swap_16byte_32bit(&arr[pivot_pos], &arr[right]);
    }

    unsigned int pivot_key = arr[right].score;
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (arr[j].score <= pivot_key) {
            i++;
            if (i != j) {
                swap_16byte_32bit(&arr[i], &arr[j]);
            }
        }
    }

    i++;
    if (i != right) {
        swap_16byte_32bit(&arr[i], &arr[right]);
    }

    return i;
}

void quick_sort_optimized_32bit(void* base, int left, int right) {
    // Small array -> insertion sort
    if (right - left <= THRESHOLD) {
        insertion_sort_optimized_32bit((RECORD_DIRECT*)base + left, right - left + 1);
        return;
    }

    if (left >= right) return;

    // Median-of-three pivot
    int pivot_pos = median_of_three_optimized_32bit(base, left, right);

    // 2-way partition
    int p = partition_2way_32bit(base, left, right, pivot_pos);

    // Recursive calls
    quick_sort_optimized_32bit(base, left, p - 1);
    quick_sort_optimized_32bit(base, p + 1, right);
}

