/* 
 * File:   ShellSorts.cpp
 * Author: kris
 *
 * Created on 26 November 2014, 15:18
 */

#include "Defines.h"
#include <cassert>
#include "Shellsorts.h"
#include <iostream>
#include <algorithm>
#include "SIMD.h"
#include <utility>
#include <vector>
#include <algorithm>
#include "CUDA.h"
#include <omp.h>

using namespace std;

static unsigned long long comparisons = 0; ///Counter for comparisons

/**
 * Compare two elements, and exchange them if needed
 * 
 * This could probably be done better since we are working with integers
 *
 * @param Array array containing elements
 * @param [in] i index of first element
 * @param [in] j index of second element
 * @param [in] N size of a
 */
static void CompareAndExchange(int * const Array, unsigned int const i, unsigned int const j) { //Inline does nothing here, assume compiler knows best
    assert(i >= 0);
    assert(j >= 0);
    comparisons++;
    CE(Array, i, j)
}

__attribute__((optimize("unroll-loops")))
unsigned long long PrattsShellsort(int * const Array, unsigned int const N) {
    vector<int> sequence;
    for (auto i = N / 2; i > 0; i /= 2) {
        auto j = i;
        do {
            sequence.push_back(j);
            j = (3 * j) / 2;
        } while (j % 3 == 0 && j < N);
    }
#ifdef CUDA
    int * d_A = ToDevice(Array, N);
#endif
    for (auto j : sequence) {
#ifdef CUDA
        if (j >= CUDAWARPCORES & d_A != nullptr) {
            CudaHShakeUp(d_A, j, N);
            continue;
        }
        if (d_A != nullptr) {
            FromDevice(d_A, Array, N);
            d_A = nullptr;
        }

#endif
#ifdef SIMD
        if (j >= 4) {
            if (j % 4 != 0) {
                for (auto k = 0; k < N - j - 3; k += 4) {
                    CompareExchange4U(&Array[k], &Array[k + j]);
                    comparisons += 4;
                }
            } else {
                for (auto k = 0; k < N - j - 3; k += 4) {
                    CompareExchange4(&Array[k], &Array[k + j]);
                    comparisons += 4;
                }
            }

            for (auto k = 0; k < (N - j) % 4; ++k) {
                CompareAndExchange(Array, N - j - 1 - k, N - 1 - k);
            }
            continue;
        }
#endif
#ifdef OMP
        if (j % (1<<OMPBITPRATT) == 0) {
#pragma omp parallel private(comparisons) num_threads(OMPTHREADS)
            {
                for (auto k = omp_get_thread_num() * L1WIDTH; k < N - j; k += (L1WIDTH * OMPTHREADS)) {
                    for (auto k2 = 0; k2 < L1WIDTH; k2++) { //Cray cray? No, this is L1 cache
                        CompareAndExchange(Array, k + k2, k + k2 + j);
                    }
                }
            }
        } else if( j > (1<<OMPBITPRATT)){
            #pragma omp parallel private(comparisons) num_threads(OMPTHREADS)
            {
                int jj = j / OMPTHREADS + 1; //We really should check for number of threads, but if we cannot get this rather low amount of threads, I accept failure
                int tid = omp_get_thread_num();
                for (auto i = jj * tid; i < N - j; i += j) {
                    for (auto k = 0; k < jj && k + jj * tid < j && i + k + j < N; k++) {
                        CompareAndExchange(Array, i + k, i + k + j);
                    }
                }
            }
        } else
#endif
        {
            for (auto k = 0; k < N - j; ++k) {
                CompareAndExchange(Array, k, k + j);
            }
        }


    }

    return comparisons;
}

unsigned long long ShakerSort(int * const Array, unsigned int const N) {
    vector<int> sequence;
    sequence.push_back(1);
    sequence.push_back(1);
    double a = 1.7;
    double accum = 1.7;
    while (int(accum) < N) {
        sequence.push_back(int(accum) + 1);
        accum *= a;
    }
    reverse(sequence.begin(), sequence.end());
#ifdef CUDA
    int * d_A = ToDevice(Array, N);
#endif
    for (auto s : sequence) {
#ifdef CUDA
        if (s >= CUDAWARPCORES) {
            CudaHShakeUp(d_A, s, N);
            CudaHShakeDown(d_A, s, N);
            continue;
        } else if (d_A != nullptr) {
            FromDevice(d_A, Array, N);
            d_A = nullptr;
        }
#endif
#ifdef SIMD
        if (s >= 4 & s % 4 == 0) { //Assume n%4==0
            for (auto i = 0; i < N - s - 3; i += 4) {
                CompareExchange4(&Array[i], &Array[i + s]);
                comparisons += 4;
            }
            for (auto i = 0; i < (N - s) % 4; ++i) {
                CompareAndExchange(Array, N - s - 1 - i, N - 1 - i);
            }
            for (auto i = N - 4; i >= s; i -= 4) {
                CompareExchange4(&Array[i - s], &Array[i]);
                comparisons += 4;
            }
            for (auto i = 0; i < (N - s) % 4; ++i) {
                CompareAndExchange(Array, i, i + s);
            }
            continue;
        } else if (s >= 4) {
            for (auto i = 0; i < N - s - 3; i += 4) {
                CompareExchange4U(&Array[i], &Array[i + s]);
                comparisons += 4;
            }
            for (auto i = 0; i < (N - s) % 4; ++i) {
                CompareAndExchange(Array, N - s - 1 - i, N - 1 - i);
            }
            for (auto i = N - 4; i >= s; i -= 4) {
                CompareExchange4UU(&Array[i - s], &Array[i]);
                comparisons += 4;
            }
            for (auto i = 0; i < (N - s) % 4; ++i) {
                CompareAndExchange(Array, i, i + s);
            }
            continue;
        }
#endif
#ifdef OMP
        if (s > (1<<OMPBITSSHAKER)) {
#pragma omp parallel private(comparisons) num_threads(OMPTHREADS)
            {
                int j = s / OMPTHREADS + 1; //We really should check for number of threads, but if we cannot get this rather low amount of threads, I accept failure
                int tid = omp_get_thread_num();
                for (auto i = j * tid; i < N - s; i += s) {
                    for (auto k = 0; k < j && k + j * tid < s && i + k + s < N; k++) {
                        CompareAndExchange(Array, i + k, i + k + s);
                    }
                }
                for (auto i = N - 1 - j * (tid + 1); i >= s; i -= s) {
                    for (auto k = 1; k <= j && k + j * tid <= s && i + j - k >= s; k++) {
                        CompareAndExchange(Array, i - s + j - k, i + j - k);
                    }
                }
            }
        } else
#endif
        {
            for (auto i = 0; i < N - s; ++i) {
                CompareAndExchange(Array, i, i + s);
            }
            for (auto i = N - 1; i >= s; --i) {
                CompareAndExchange(Array, i - s, i);
            }
        }
    }
    return comparisons;
}