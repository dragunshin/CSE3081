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
#include "my_quick_sorts.h"

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
	FILE *cmd_file, *input_file, *output_file;
	int function_number;
	int n;
	char input_filename[256];
	char output_filename[256];
	RECORD *Array = NULL;

	// Read HW3_commands.txt
	cmd_file = fopen("HW3_commands.txt", "r");
	if (cmd_file == NULL) {
		fprintf(stderr, "Error: Cannot open HW3_commands.txt\n");
		return;
	}

	fscanf(cmd_file, "%d", &function_number);
	fscanf(cmd_file, "%d", &n);
	fscanf(cmd_file, "%s", input_filename);
	fscanf(cmd_file, "%s", output_filename);
	fclose(cmd_file);

	printf("=== HW3: Quick Sort (S20211052) ===\n");
	printf("Function number: %d\n", function_number);
	printf("*** Array size: %d\n", n);
	printf("Input file: %s\n", input_filename);
	printf("Output file: %s\n\n", output_filename);

	// Allocate memory
	Array = (RECORD *)malloc(sizeof(RECORD) * n);
	if (Array == NULL) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		return;
	}

	// Read binary input file
	input_file = fopen(input_filename, "rb");
	if (input_file == NULL) {
		fprintf(stderr, "Error: Cannot open input file %s\n", input_filename);
		free(Array);
		return;
	}

	size_t read_count = fread(Array, sizeof(RECORD), n, input_file);
	fclose(input_file);

	if (read_count != (size_t)n) {
		fprintf(stderr, "Error: Read %zu records, expected %d\n", read_count, n);
		free(Array);
		return;
	}

	printf("Read %d records from %s\n", n, input_filename);

#ifdef PRINT_DATA
	printf("* Data before sort: ");
	for (int i = 0; i < n; i++)
		printf(" [score = %u] ", Array[i].score);
	printf("\n\n");
#endif

	// Sort based on function number
	printf("Sorting with function %d...\n", function_number);
	CHECK_TIME_START;

	switch (function_number) {
		case 1:
			my_qsort(Array, n, sizeof(RECORD), my_record_keys_compare);
			break;
		case 21:
			my_qsort_orig(Array, n, sizeof(RECORD), my_record_keys_compare);
			break;
		case 22:
			my_qsort_median_insert(Array, n, sizeof(RECORD), my_record_keys_compare);
			break;
		case 23:
			my_qsort_median_insert_iter(Array, n, sizeof(RECORD), my_record_keys_compare);
			break;
		case 24:
			my_qsort_final(Array, n, sizeof(RECORD), my_record_keys_compare);
			break;
		default:
			fprintf(stderr, "Error: Invalid function number %d (valid: 1, 21, 22, 23, 24)\n", function_number);
			free(Array);
			return;
	}

	CHECK_TIME_END(compute_time);
	printf("*** RECORD type of size %d: Time taken = %.3fms\n\n", sizeof(RECORD), compute_time);

#ifdef PRINT_DATA
	printf("* Data after sort: ");
	for (int i = 0; i < n; i++)
		printf(" [score = %u] ", Array[i].score);
	printf("\n\n");
#endif

	// Verify sorting
	for (int i = 0; i < n - 1; i++) {
		if (Array[i].score > Array[i + 1].score) {
			fprintf(stderr, "Error: Array is not sorted at index %d!\n", i);
			free(Array);
			return;
		}
	}
	printf("Verification: Array is correctly sorted\n");

	// Write binary output file
	output_file = fopen(output_filename, "wb");
	if (output_file == NULL) {
		fprintf(stderr, "Error: Cannot open output file %s\n", output_filename);
		free(Array);
		return;
	}

	size_t write_count = fwrite(Array, sizeof(RECORD), n, output_file);
	fclose(output_file);

	if (write_count != (size_t)n) {
		fprintf(stderr, "Error: Wrote %zu records, expected %d\n", write_count, n);
		free(Array);
		return;
	}

	printf("Wrote %d sorted records to %s\n", n, output_filename);
	printf("=== Completed successfully ===\n");

	free(Array);
}