//
// Example Codes for Sogang CSE0381/AIE3051(1)
// 2025.10.27
//

#include <Windows.h>
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

#define N_MAX_RECORDS 1024 * 1024 * 8
// #define PRINT_DATA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _record {
	unsigned int score;
	char other_data[12];
} RECORD;

typedef unsigned int RECORD2;

int my_record_keys_compare(const void *a, const void *b) {
	RECORD *rec_a, *rec_b;

	rec_a = (RECORD *)a;
	rec_b = (RECORD *)b;

	if (rec_a->score == rec_b->score)
		return 0;
	else if (rec_a->score < rec_b->score)
		return -1;
	else return 1;
	
	// could be "return rec_a->score - rec_b->scire;"
}

int my_unsigned_int_keys_compare(const void *a, const void *b) {
	RECORD2 *ui_a, *ui_b;;

	ui_a = (RECORD2 *)a;
	ui_b = (RECORD2 *)b;

	if (*ui_a == *ui_b)
		return 0;
	else if (*ui_a < *ui_b)
		return -1;
	else return 1;
}

void init_RECORD_array(RECORD *data, int n) {
	// Generate an array with random key sequences.
	int i, j;
	
	for (i = 0; i < n; i++) {
		data[i].score = i;
		memset(data[i].other_data, 0, sizeof(data[i].other_data));
	}

	// Shuffle the key values using the Fisher-Yates shuffle algorithm.
	srand((unsigned int)time(NULL));

	for (i = n - 1; i >= 1; i--) {
		j = rand() % (i + 1);

		{
			// codes from qsort.c
			char buf[256];
			char *r_i = (char *)(data + i);
			char *r_j = (char *)(data + j);
			size_t m, ms;

			for (ms = sizeof(RECORD); ms > 0; ms -= m, r_i += m, r_j += m) {
				m = ms < sizeof(buf) ? ms : sizeof(buf);

				memcpy(buf, r_i, m);
				memcpy(r_i, r_j, m);
				memcpy(r_j, buf, m);
			}
		}
	}
}

void init_RECORD2_array(RECORD2 *data, int n) {
	// Generate an array with random key sequences.
	int i, j;
	RECORD2 tmp;

	for (i = 0; i < n; i++)
		data[i] = i;

	// Shuffle the key values using the Fisher-Yates shuffle algorithm.
	srand((unsigned int)time(NULL));

	for (i = n - 1; i >= 1; i--) {
		j = rand() % (i + 1);

		tmp = data[i]; data[i] = data[j]; data[j] = tmp;
	}
}

void main(void) {
	int n;
	RECORD Array[N_MAX_RECORDS];
	RECORD2 Array2[N_MAX_RECORDS];

	n = N_MAX_RECORDS;
	printf("*** Array size: %d\n", n);

	init_RECORD_array(Array, n);

#ifdef PRINT_DATA
	printf("* Data before sort: ");
	for (int i = 0; i < n; i++)
		printf(" [score = %d] ", Array[i].score); printf("\n\n");
	printf("\n\n");
#endif

	CHECK_TIME_START;
	qsort(Array, n, sizeof(RECORD), my_record_keys_compare);
	CHECK_TIME_END(compute_time);
	printf("*** RECORD type of size %d: Time taken by qsort() = %.3fms\n", sizeof(RECORD), compute_time);

#ifdef PRINT_DATA
	printf("* Data after sort: ");
	for (int i = 0; i < n; i++)
		printf(" [score = %d] ", Array[i].score); printf("\n\n");
#endif

	init_RECORD2_array(Array2, n);

#ifdef PRINT_DATA
	printf("* Data before sort: ");
	for (int i = 0; i < n; i++)
		printf(" ([%u]) ", Array2[i]);
	printf("\n\n");
#endif

	CHECK_TIME_START;
	qsort(Array2, n, sizeof(RECORD2), my_unsigned_int_keys_compare);
	CHECK_TIME_END(compute_time);
	printf("*** RECORD2 type of size %d: Time taken by qsort() = %.3fms\n", sizeof(RECORD2), compute_time);

#ifdef PRINT_DATA
	printf("* Data after sort: ");
	for (int i = 0; i < n; i++)
		printf(" ([%u]) ", Array2[i]);
	printf("\n\n");
#endif
}