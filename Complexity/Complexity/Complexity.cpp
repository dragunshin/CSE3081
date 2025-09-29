//
// Written for CSE3081-1/AIE3051-1 Sogang University
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
static __int64 _start, _freq, _end;
static float _compute_time;
#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))
// CHECK_TIME_START(_start, _freq);
// CHECK_TIME_END(_start, _end, _freq, _compute_time);

#define MAX_CONSTANT_ITERATION 512

// O(1) or O(c)
unsigned long long f0(int n) {
	unsigned long long counter = 0;
	int r = n % 2;
	if (r == 0) {
		counter++;
	}
	else {
		counter++;
	}
	return counter;
}

// O(m + n)
unsigned long long f1(int m, int n) {
	unsigned long long counter = 0;
	for (int i = 0; i < m; i++)
		counter++;
	for (int j = 0; j < n; j++) 
		counter++;
	return counter;
}

// O(log n)
unsigned long long f2(int n, float* dummy) {
	unsigned long long counter = 0;
	int i = 1;
	while (i <= n) {
		i *= 2;
		for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
			*dummy /= 1.00001;
		counter++;
	}
	return counter;
}

// O(n log n)
unsigned long long f3(int n, float* dummy) {
	unsigned long long counter = 0;
	for (int i = n / 2; i <= n; i++) {
		for (int j = 1; j <= n; j = j * 2) {
			for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
				*dummy /= 1.00001;
			counter++;
		}
	}
	return counter;
}

// O(n^2)
unsigned long long f4(int n, float* dummy) {
	unsigned long long counter = 0;
	for (int i = 0; i < n; i++)
		for (int j = i; j > 0; j--) {
			for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
				*dummy /= 1.00001;
			counter++;
		}
	return counter;
}

// O(n^2)
unsigned long long f5(int* Array, int n, int x, float* dummy) {
	unsigned long long counter = 0;
	for (int i = 0; i < n - 1; i++) {
		for (int j = (i + 1); j < n; j++) {
			if (Array[i] + Array[j] == x) {
				for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
					*dummy /= 1.00001;
				counter++;
				return counter;
			}
			else {
				for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
					*dummy /= 1.00001;
				counter++;
			}
		}
	}
	return counter;
}

// O(sqrt(n))
unsigned long long f6(int n, float* dummy) {
	unsigned long long counter = 0;
	int i = 1, j = 1, m = 0; // n > 0
	while (j <= n) {
		i++;
		j = j + i;
		m = m + 2;
		for (int k = 0; k < MAX_CONSTANT_ITERATION; k++)
			*dummy /= 1.00001;
		counter++;
	}
	return counter;
}

// O(n^3)
unsigned long long f7(int n, float* dummy) {
	unsigned long long counter = 0;
	unsigned long long x = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= i; j++)
			for (int k = 1; k <= j; k++) {
				x += i + j + k;
				for (int l = 0; l < MAX_CONSTANT_ITERATION; l++)
					*dummy /= 1.00001;
				counter++;
			}
	return counter;
}

// O(n^5) 
unsigned long long f8(int n, float* dummy) {
	unsigned long long counter = 0;
	unsigned long long x = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= i * i; j++)
			for (int k = 1; k <= j; k++) {
				x += i + j + k;
				for (int l = 0; l < MAX_CONSTANT_ITERATION; l++)
					*dummy /= 1.00001;
				counter++;
			}
	return counter;
}

#define RUN_F5

int main() {
	float dummy;
	unsigned long long counter;
	printf("- Size of unsinged long long = %d\n\n", sizeof(unsigned long long));
	srand(time(NULL));

#ifdef RUN_F2
#define F2_MAX_N 1048576*256

	for (int n = 1; n <= F2_MAX_N; n *= 2) {  
		dummy = (float)rand();
		CHECK_TIME_START(_start, _freq);
		counter = f2(n, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		printf("* [F2] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);
	}
#endif

#ifdef RUN_F3
#define F3_MAX_N 1048576
	for (int n = 1; n <= F3_MAX_N; n *= 2) {
		dummy = (float)rand();
		CHECK_TIME_START(_start, _freq);
		counter = f3(n, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		printf("* [F3] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);
	}
#endif

#ifdef RUN_F4
#define F4_MAX_N 1048576
	for (int n = 1; n <= F4_MAX_N; n *= 2) {
		dummy = (float) rand();
		CHECK_TIME_START(_start, _freq);
		counter = f4(n, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		printf("* [F4] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);  
	}
#endif

#ifdef RUN_F5
#define F5_MAX_N 1048576
	for (int n = 2; n <= F5_MAX_N; n *= 2) {
		int* A = (int*)malloc(sizeof(int) * n);
		for (int i = 0; i < n; i++)
			A[i] = rand() % 257 - 128; // -128 <= A[i] <= 128
		dummy = (float)rand();

		CHECK_TIME_START(_start, _freq);
		counter = f5(A, n, 512, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		printf("* [F5] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);
		free(A);
	}
#endif

#ifdef RUN_F6
#define F6_MAX_N 1048576*256
	for (int n = 1; n <= F6_MAX_N; n *= 2) {
		dummy = (float)rand();
		CHECK_TIME_START(_start, _freq);
		counter = f6(n, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		unsigned long long p = floor((-3.0 + sqrt(8.0 * n + 1.0)) / 2.0) + 1;
		printf("* [F6] n = %7d: counter = %llu == %llu (%f), Time = %.3fms\n", n, counter, p, dummy, _compute_time);
	}
#endif

#ifdef RUN_F7
#define F7_MAX_N 1048576
	for (int n = 1; n <= F7_MAX_N; n *= 2) {
		dummy = (float)rand();
		CHECK_TIME_START(_start, _freq);
		counter = f7(n, &dummy);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);
		printf("* [F7] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);
		printf("        n(analysis) = %llu\n", (unsigned long long)n * (unsigned long long)(n + 1) * (unsigned long long)(n + 2) / 6);
	}
#endif

#ifdef RUN_F8
#define F8_MAX_N 1048576
	for (int n = 1; n <= F8_MAX_N; n *= 2) {
	dummy = (float)rand();
	CHECK_TIME_START(_start, _freq);
	counter = f8(n, &dummy);
	CHECK_TIME_END(_start, _end, _freq, _compute_time);
	printf("* [F8] n = %7d: counter = %llu (%f), Time = %.3fms\n", n, counter, dummy, _compute_time);
	}
#endif
}