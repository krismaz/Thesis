/* 
 * File:   SortingNetworks.cpp
 * Author: kris
 *
 * Created on 10 November 2014, 14:44
 */


#include "Defines.h"
#include <cassert>
#include "SortingNetworks.h"
#include <iostream>
#include <algorithm>
#include "SIMD.h"
#include <utility>
#include "CUDA.h"
#include <omp.h>
#include <unistd.h>

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
__attribute__((always_inline))
static void inline CompareAndExchange2way(int * const Array, unsigned int const i, unsigned int const j) { //The compile seems to slightly mess up inlining here

    assert(i >= 0);
    assert(j >= 0);
    comparisons++;
    // cout << "(" << i << ", " << j << ")" << endl;
    CE(Array, i, j)
}

/*Bitonic sort*/
template<bool Ascending>
static void BitonicMerge(int * const Array, unsigned int const lo, unsigned int const N) {
    if (N < 2) return;
    unsigned int two_msb = 1 << ((__builtin_clz(N)^0x1F)); //Largest power of 2 smaller than or equal to N
    if (two_msb == N) {
        two_msb /= 2;
    }
    int m(two_msb);
    if (Ascending) {
        for (int i = lo; i < lo + N - m; ++i) {
            CompareAndExchange2way(Array, i, i + m);
        }
    } else {
        for (int i = lo; i < lo + N - m; ++i) {
            CompareAndExchange2way(Array, i + m, i);
        }
    }

    BitonicMerge<Ascending>(Array, lo, m);
    BitonicMerge<Ascending>(Array, lo + m, N - m);
}

template<bool Ascending>
static void BitonicSort(int * const Array, unsigned int const lo, unsigned int const N) {
    if (N < 2) return;
    unsigned int m(N / 2);
    BitonicSort<!Ascending > (Array, lo, m);
    BitonicSort<Ascending>(Array, lo + m, N - m);
    BitonicMerge<Ascending>(Array, lo, N);

}

unsigned long long BitonicSort(int * const Array, unsigned int const N) {
    BitonicSort<true>(Array, 0, N);
    return comparisons;
}

/*Bitonic sort for powers of 2*/

template<bool Ascending, int bits>
static void BitonicMerge2(int * const Array, unsigned int const lo);

template<>
void BitonicMerge2<true, 0>(int * const Array, unsigned int const lo) {
}

template<>
void BitonicMerge2<false, 0>(int * const Array, unsigned int const lo) {
}

template<>
void BitonicMerge2 < true, -1 > (int * const Array, unsigned int const lo) {
}

template<>
void BitonicMerge2 < false, -1 > (int * const Array, unsigned int const lo) {
}

template<>
void BitonicMerge2 < true, -2 > (int * const Array, unsigned int const lo) {
}

template<>
void BitonicMerge2 < false, -2 > (int * const Array, unsigned int const lo) {
}

template<bool Ascending, int bits>
static void BitonicMerge2(int * const Array, unsigned int const lo) {
    unsigned int m = 1 << (bits - 1);
#ifdef SIMD
    if (bits >= 3) {
        if (Ascending) {
            for (int i = 0; i < m; i += 4) {
                CompareExchange4(&Array[lo + i], &Array[lo + i + m]);
                comparisons += 4;
            }
        } else {
            for (int i = 0; i < m; i += 4) {
                CompareExchange4(&Array[lo + i + m], &Array[lo + i]);
                comparisons += 4;
            }
        }
    } else
#endif
#ifdef DOUBLELAYERBS
        if (bits > 1) {
        if (Ascending) {
            for (int i = m / 2; i < m; ++i) {
                CompareAndExchange2way(Array, lo + i, lo + i + m);
            }
            for (int i = 0; i < m / 2; ++i) {
                CompareAndExchange2way(Array, lo + i, lo + i + m);
                CompareAndExchange2way(Array, lo + i, lo + i + m / 2);
                CompareAndExchange2way(Array, lo + i + m, lo + i + m / 2 + m);
            }
        } else {
            for (int i = m / 2; i < m; ++i) {
                CompareAndExchange2way(Array, lo + i + m, lo + i);
            }
            for (int i = 0; i < m / 2; ++i) {
                CompareAndExchange2way(Array, lo + i + m, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 2, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 2 + m, lo + i + m);
            }
        }
        BitonicMerge2<Ascending, bits - 2 > (Array, lo);
        BitonicMerge2<Ascending, bits - 2 > (Array, lo + m / 2);
        BitonicMerge2<Ascending, bits - 2 > (Array, lo + m);
        BitonicMerge2<Ascending, bits - 2 > (Array, lo + m + m / 2);
        return;
    }
#endif
#ifdef QUADLAYERBS
    if (bits > 4) {
        if (Ascending) {
            for (int i = m / 2; i < m / 1; ++i) {
                CompareAndExchange2way(Array, lo + i, lo + i + m);
            }
            for (int i = m / 4; i < m / 2; ++i) {
                CompareAndExchange2way(Array, lo + i, lo + i + m);
                CompareAndExchange2way(Array, lo + i, lo + i + m / 2);
                CompareAndExchange2way(Array, lo + i + m, lo + i + m + m / 2);
            }
            for (int i = 0; i < m / 4; ++i) {
                CompareAndExchange2way(Array, lo + i, lo + i + m);
                CompareAndExchange2way(Array, lo + i, lo + i + m / 2);
                CompareAndExchange2way(Array, lo + i, lo + i + m / 4);
                CompareAndExchange2way(Array, lo + i + m / 2, lo + i + m / 2 + m / 4);
                CompareAndExchange2way(Array, lo + i + m, lo + i + m + m / 2);
                CompareAndExchange2way(Array, lo + i + m, lo + i + m + m / 4);
                CompareAndExchange2way(Array, lo + i + m + m / 2, lo + i + m + m / 2 + m / 4);
            }
        } else {
            for (int i = m / 2; i < m / 1; ++i) {
                CompareAndExchange2way(Array, lo + i + m, lo + i);
            }
            for (int i = m / 4; i < m / 2; ++i) {
                CompareAndExchange2way(Array, lo + i + m, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 2, lo + i);
                CompareAndExchange2way(Array, lo + i + m + m / 2, lo + i + m);
            }
            for (int i = 0; i < m / 4; ++i) {
                CompareAndExchange2way(Array, lo + i + m, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 2, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 4, lo + i);
                CompareAndExchange2way(Array, lo + i + m / 2 + m / 4, lo + i + m / 2);
                CompareAndExchange2way(Array, lo + i + m + m / 2, lo + i + m);
                CompareAndExchange2way(Array, lo + i + m + m / 4, lo + i + m);
                CompareAndExchange2way(Array, lo + i + m + m / 2 + m / 4, lo + i + m + m / 2);
            }
        }
        BitonicMerge2<Ascending, bits - 3 > (Array, lo);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m / 4);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m / 2);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m / 2 + m / 4);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m + m / 4);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m + m / 2);
        BitonicMerge2<Ascending, bits - 3 > (Array, lo + m + m / 2 + m / 4);
        return;
    }
#endif

    if (Ascending) {
        for (int i = 0; i < m; ++i) {
            CompareAndExchange2way(Array, lo + i, lo + i + m);
        }
    } else {
        for (int i = 0; i < m; ++i) {
            CompareAndExchange2way(Array, lo + i + m, lo + i);
        }
    }
#ifdef OMP
    if (bits > OMPBITSBS) {
#pragma omp task
        BitonicMerge2<Ascending, bits - 1 > (Array, lo);

#pragma omp task
        BitonicMerge2<Ascending, bits - 1 > (Array, lo + m);
#pragma omp taskwait
    } else
#endif
    {
        BitonicMerge2<Ascending, bits - 1 > (Array, lo);
        BitonicMerge2<Ascending, bits - 1 > (Array, lo + m);
    }
}

template<bool Ascending, int bits>
static void BitonicSort2(int * const Array, unsigned int const lo);

template<>
void BitonicSort2<true, 0>(int * const Array, unsigned int const lo) {
}

template<>
void BitonicSort2<false, 0>(int * const Array, unsigned int const lo) {
}

template<bool Ascending, int bits>
static void BitonicSort2(int * const Array, unsigned int const lo) {
    unsigned int m = 1 << (bits - 1);
    BitonicSort2<!Ascending, bits - 1 > (Array, lo);
    BitonicSort2<Ascending, bits - 1 > (Array, lo + m);
    BitonicMerge2<Ascending, bits>(Array, lo);
}

template<int bits>
static void BitonicSortSelect2(int * const Array, unsigned int const N);

template<>
void BitonicSortSelect2<0>(int * const Array, unsigned int const N) {
    std::cerr << "0 bit selection" << std::endl;
}

//Select the correct power of 2

template<int bits>
static void BitonicSortSelect2(int * const Array, unsigned int const N) {
    if (1 << bits != N) {
        BitonicSortSelect2 < bits - 1 > (Array, N);
        return;
    }
#ifdef OMP
#pragma omp parallel private(comparisons)
    {
#pragma omp single
        {
            BitonicSort2<true, bits>(Array, 0);
        }
    }
#else
    BitonicSort2<true, bits>(Array, 0);
#endif
}

unsigned long long BitonicSort2(int * const Array, unsigned int const N) {
#ifdef CUDA
    int* d_A = ToDevice(Array, N);
    CudaBitonicSort(d_A, N);
    FromDevice(d_A, Array, N);
    return 42;
#endif

    BitonicSortSelect2<31>(Array, N);

    return comparisons;
}

/*Bitonic sort for CUDA*/
unsigned long long BitonicSort3(int * const Array, unsigned int const N) { //WTF is this even
    for (int i = 2; i <= N; i *= 2) {
        for (int j = 0; j < N / i; j++) {
            bool descending = j % 2;
            int lo = i*j;
            cout << "Sort " << lo << " to " << (lo + i - 1) << (descending ? " descending" : " ascending") << endl;
            for (int m = i / 2; m > 0; m /= 2) {
                for (int c = 0; c < (i / 2) / m; c++) {
                    for (int k = 0; k < m; k++) {
                        if (descending) {
                            CompareAndExchange2way(Array, lo + k + c * m * 2 + m, lo + k + c * m * 2);
                        } else {
                            CompareAndExchange2way(Array, lo + k + c * m * 2, lo + k + c * m * 2 + m);
                        }
                    }
                }
            }
        }
    }
    return comparisons;
}

unsigned long long BitonicSort4(int * const Array, unsigned int const N) { //WTF is this even
    for (int i = 2; i <= N; i *= 2) {
        for (int m = i; m > 1; m /= 2) {
            for (int t = 0; t < N / 2; t++) {
                bool ascending = t % i < i / 2;
                int ti = t % (m / 2) + m * (t / (m / 2));
                if (ascending) {
                    CompareAndExchange2way(Array, ti, ti + m / 2);
                } else {
                    CompareAndExchange2way(Array, ti + m / 2, ti);
                }
            }
        }
    }
    return comparisons;
}

/*Odd-Even sort*/
static void OddEvenMerge(int * const Array, unsigned int const lo, unsigned int const range, unsigned int const r, unsigned int const N) {
    int m(r * 2);
    if (m >= range) {
        if (lo + r < N)
            CompareAndExchange2way(Array, lo, lo + r);
        return;
    }
    OddEvenMerge(Array, lo, range, m, N);
    OddEvenMerge(Array, lo + r, range, m, N);
    unsigned int upper = std::min(lo + range, N);
    for (int i = lo + r; i + r < upper; i += m) {
        CompareAndExchange2way(Array, i, i + r);
    }
}

static void OddEvenMergeSort(int * const Array, unsigned int const lo, unsigned int const range, unsigned int const N) {
    if (range < 2)
        return;
    unsigned int two_msb = 1 << ((__builtin_clz(range)^0x1F)); //Largest power of 2 smaller than or equal to N
    if (two_msb == range) {
        two_msb /= 2;
    }
    unsigned int m(two_msb);
    OddEvenMergeSort(Array, lo, m, N);
    OddEvenMergeSort(Array, lo + m, range - m, N);
    OddEvenMerge(Array, lo, range, 1, N);
}

unsigned long long OddEvenMergeSort(int * const Array, unsigned int const N) {
    unsigned int two_msb = 1 << ((__builtin_clz(N)^0x1F)); //Largest power of 2 smaller than or equal to N
    if (two_msb != N) {
        two_msb *= 2;
    }
    OddEvenMergeSort(Array, 0, N, N);
    return comparisons;
}
/*Odd-Even sort 2*/

template<int bits>
static void OddEvenMerge2(int * const Array, unsigned int const lo, unsigned int const N);

template<>
void OddEvenMerge2<32>(int * const Array, unsigned int const lo, unsigned int const N) {
}

template<int bits>
static void OddEvenMerge2(int * const Array, unsigned int const lo, unsigned int const N) {
    unsigned int r = 1 << (bits);
    unsigned int m(r * 2);
    if (m >= N) {
        CompareAndExchange2way(Array, lo, lo + r);
    } else {
        {
            OddEvenMerge2 < bits + 1 > (Array, lo, N);
            OddEvenMerge2 < bits + 1 > (Array, lo + r, N);
        }
        for (int i = r; i + r < N; i += m)
            CompareAndExchange2way(Array, lo + i, lo + i + r);
    }
}

template<int bits>
static void OddEvenLeavingMerge2(int * const Array, unsigned int const lo, int * const Buffer);

template<>
void OddEvenLeavingMerge2<0>(int * const Array, unsigned int const lo, int * const Buffer) {
};

template<>
void OddEvenLeavingMerge2<-1 > (int * const Array, unsigned int const lo, int * const Buffer) {
};

template<int bits>
static void OddEvenLeavingMerge2(int * const Array, unsigned int const lo, int * const Buffer) {
    if (bits == 1) {
        CompareAndExchange2way(Array, lo, lo + 1);
        return;
    }
    unsigned int N = 1 << bits;
#ifdef DOUBLELAYER
    if (bits > 2) {
        for (int i = 0; i < N / 4; ++i) {
            Buffer[i] = Array[lo + i * 4];
            Buffer[i + N / 4] = Array[lo + i * 4 + 2];
            Buffer[i + N / 2] = Array[lo + i * 4 + 1];
            Buffer[i + N / 2 + N / 4] = Array[lo + i * 4 + 3];
        }
#ifdef OMP
        if (bits > 16 + 2) {
#pragma omp task
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, 0, &Array[lo]);
#pragma omp task
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 4, &Array[lo + N / 4]);
#pragma omp task
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 2, &Array[lo + N / 2]);
#pragma omp task
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 2 + N / 4, &Array[lo + N / 2 + N / 4]);
#pragma omp taskwait
        } else
#endif
        {
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, 0, &Array[lo]);
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 4, &Array[lo]);
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 2, &Array[lo]);
            OddEvenLeavingMerge2 < bits - 2 > (Buffer, N / 2 + N / 4, &Array[lo]);
        }
    } else
#endif
    {
        for (int i = 0; i < N / 4; ++i) { //Double-headed copy, shouldn't matter much
            Buffer[i] = Array[lo + i * 2];
            Buffer[i + N / 2] = Array[lo + i * 2 + 1];
            Buffer[i + N / 4] = Array[lo + i * 2 + N / 2];
            Buffer[i + 3 * N / 4] = Array[lo + i * 2 + 1 + N / 2];
        }
#ifdef OMP
        if (bits > OMPBITSOEMS) {
#pragma omp task
            OddEvenLeavingMerge2 < bits - 1 > (Buffer, 0, &Array[lo]);
#pragma omp task
            OddEvenLeavingMerge2 < bits - 1 > (Buffer, N / 2, &Array[lo + N / 2]);
#pragma omp taskwait
        } else
#endif
        {
            OddEvenLeavingMerge2 < bits - 1 > (Buffer, 0, &Array[lo]);
            OddEvenLeavingMerge2 < bits - 1 > (Buffer, N / 2, &Array[lo]);
        }
    }
#ifdef SIMD
    if (bits >= 3) {

        for (int i = 0; i < N / 2 - 4; i += 4) {
            CompareExchange4U(&Buffer[i + N / 2], &Buffer[i + 1]); //&Buffer[i+1] is not 16-byte aligned
            comparisons += 4;
        }
        //Notice we save 1 comparison at the each end of the input, which sucks for SSE
        //This is what makes the number of comparisons slightly smaller than Bitonic Sort
        CompareAndExchange2way(Buffer, N - 4, N / 2 - 3);
        CompareAndExchange2way(Buffer, N - 3, N / 2 - 2);
        CompareAndExchange2way(Buffer, N - 2, N / 2 - 1);
        for (int i = 0; i < N / 2; ++i) {
            Array[lo + i * 2] = Buffer[i];
            Array[lo + i * 2 + 1] = Buffer[i + N / 2];
        }

        return;
    }
#endif
#ifdef DOUBLELAYER
    if (bits > 2) {
        for (int i = 0; i < N / 4; ++i) {
            Array[lo + i * 4] = Buffer[i];
            Array[lo + i * 4 + 2] = Buffer[i + N / 4];
            Array[lo + i * 4 + 1] = Buffer[i + N / 2];
            Array[lo + i * 4 + 3] = Buffer[i + N / 2 + N / 4];
        }
        for (int i = 2; i < N - 4; i += 4) {
            CompareAndExchange2way(Array, lo + i, lo + i + 2);
            CompareAndExchange2way(Array, lo + i + 1, lo + i + 3);
            CompareAndExchange2way(Array, lo + i - 1, lo + i);
            CompareAndExchange2way(Array, lo + i + 1, lo + i + 2);
        }
        CompareAndExchange2way(Array, lo + N - 3, lo + N - 2);
    } else
#endif
    {
        for (int i = 0; i < N / 2 - 1; ++i) {
            CompareAndExchange2way(Buffer, i + N / 2, i + 1);
        }
        for (int i = 0; i < N / 2; ++i) {
            Array[lo + i * 2] = Buffer[i];
            Array[lo + i * 2 + 1] = Buffer[i + N / 2];
        }
    }
}

template<int bits>
static void OddEvenMergeSort2(int * const Array, unsigned int const lo);

template<>
void OddEvenMergeSort2<0>(int * const Array, unsigned int const lo) {
}

#ifdef LEAVINGMERGE
static int * leavingBuffer __attribute__((aligned(16)));
#endif

template<int bits>
static void OddEvenMergeSort2(int * const Array, unsigned int const lo) {
    unsigned int m = 1 << (bits - 1);
    OddEvenMergeSort2 < bits - 1 > (Array, lo);
    OddEvenMergeSort2 < bits - 1 > (Array, lo + m);
#ifdef LEAVINGMERGE
    OddEvenLeavingMerge2<bits>(Array, lo, leavingBuffer);
    return;
#endif
    OddEvenMerge2<0>(Array, lo, 1 << (bits));
}

template<int bits>
static void OddEvenMergeSortSelect2(int * const Array, unsigned int const N);

template<>
void OddEvenMergeSortSelect2<0>(int * const Array, unsigned int const N) {
    std::cerr << "0 bit selection" << std::endl;
}

//Select the correct power of 2

template<int bits>
static void OddEvenMergeSortSelect2(int * const Array, unsigned int const N) {
    if (1 << bits != N) {
        OddEvenMergeSortSelect2 < bits - 1 > (Array, N);
        return;
    }
#ifdef LEAVINGMERGE
    leavingBuffer = new int[1 << (bits)]; //Allocate once, save time
#endif
#ifdef OMP 
#pragma omp parallel private(comparisons) 
    {
#pragma omp single
        {
            OddEvenMergeSort2<bits>(Array, 0);
        }
    }
#else
    OddEvenMergeSort2<bits>(Array, 0);
#endif
#ifdef LEAVINGMERGE
    delete leavingBuffer;
#endif
}

unsigned long long OddEvenMergeSort2(int * const Array, unsigned int const N) {
#ifdef CUDA
    int* d_A = ToDevice(Array, N);
    CudaOddEvenMergeSort(d_A, N);
    FromDevice(d_A, Array, N);
    return 0;
#endif
    OddEvenMergeSortSelect2<31>(Array, N);
    return comparisons;
}

unsigned long long OddEvenMergeSort3(int * const Array, unsigned int const N) { //OpenCL Odd-Even Mergsort .. Based China
    int t = 0;
    while (1 << t != N) ++t;
    int p = N / 2;
    bool flag = true;
    int q, r, d;
    while (p > 0) {
        if (flag) {
            q = N / 2;
            r = 0;
            d = p;
        }
        flag = true;
        for (int i = 0; i < N - d; i++) {
            if ((i & p) == r) {
                CompareAndExchange2way(Array, i, i + d);
            }
        }
        if (q != p) {
            d = q - p;
            q = q / 2;
            r = p;
            flag = false;
        }
        if (flag) {
            p = p / 2;
        }
    }
    return comparisons;
}

//Periodic Sort

void PeriodicMerge(int * const Array, unsigned int const start, unsigned int const N, unsigned int const j) {
    if (j == 0) return;
    if (N == 2) {
        CompareAndExchange2way(Array, start, start + 1);
        return;
    }
    for (auto i = 0; i < N / 2; i++) {
        CompareAndExchange2way(Array, start + i, start + N - i - 1);
    }
    PeriodicMerge(Array, start, N / 2, j - 1);
    PeriodicMerge(Array, start + N / 2, N / 2, j - 1);
}

unsigned long long PeriodicSort2(int * const Array, unsigned int const N) {
    unsigned int j = 1;
    for (int i = 2; i <= N; i *= 2) {
        PeriodicMerge(Array, 0, N, ++j);
    }
    return comparisons;
}

//16-sorter

unsigned long long Sort16(int * const Array, unsigned int const off1, unsigned int const off2) {
    CompareAndExchange2way(Array, off1, off1 + 1);
    CompareAndExchange2way(Array, off1 + 2, off1 + 3);
    CompareAndExchange2way(Array, off1 + 4, off1 + 5);
    CompareAndExchange2way(Array, off1 + 6, off1 + 7);
    CompareAndExchange2way(Array, off2 + 0, off2 + 1);
    CompareAndExchange2way(Array, off2 + 2, off2 + 3);
    CompareAndExchange2way(Array, off2 + 4, off2 + 5);
    CompareAndExchange2way(Array, off2 + 6, off2 + 7);

    CompareAndExchange2way(Array, off1 + 0, off1 + 2);
    CompareAndExchange2way(Array, off1 + 1, off1 + 3);
    CompareAndExchange2way(Array, off1 + 4, off1 + 6);
    CompareAndExchange2way(Array, off1 + 5, off1 + 7);
    CompareAndExchange2way(Array, off2, off2 + 2);
    CompareAndExchange2way(Array, off2 + 1, off2 + 3);
    CompareAndExchange2way(Array, off2 + 4, off2 + 6);
    CompareAndExchange2way(Array, off2 + 5, off2 + 7);

    CompareAndExchange2way(Array, off1 + 0, off1 + 4);
    CompareAndExchange2way(Array, off1 + 1, off1 + 5);
    CompareAndExchange2way(Array, off1 + 2, off1 + 6);
    CompareAndExchange2way(Array, off1 + 3, off1 + 7);
    CompareAndExchange2way(Array, off2, off2 + 4);
    CompareAndExchange2way(Array, off2 + 1, off2 + 5);
    CompareAndExchange2way(Array, off2 + 2, off2 + 6);
    CompareAndExchange2way(Array, off2 + 3, off2 + 7);

    CompareAndExchange2way(Array, off1 + 0, off2);
    CompareAndExchange2way(Array, off1 + 1, off2 + 1);
    CompareAndExchange2way(Array, off1 + 2, off2 + 2);
    CompareAndExchange2way(Array, off1 + 3, off2 + 3);
    CompareAndExchange2way(Array, off1 + 4, off2 + 4);
    CompareAndExchange2way(Array, off1 + 5, off2 + 5);
    CompareAndExchange2way(Array, off1 + 6, off2 + 6);
    CompareAndExchange2way(Array, off1 + 7, off2 + 7);

    CompareAndExchange2way(Array, off1 + 1, off1 + 2);
    CompareAndExchange2way(Array, off1 + 3, off2 + 4);
    CompareAndExchange2way(Array, off2 + 5, off2 + 6);
    CompareAndExchange2way(Array, off1 + 4, off2);
    CompareAndExchange2way(Array, off1 + 7, off2 + 3);
    CompareAndExchange2way(Array, off1 + 5, off2 + 2);
    CompareAndExchange2way(Array, off2 + 1, off1 + 6);

    CompareAndExchange2way(Array, off1 + 2, off2);
    CompareAndExchange2way(Array, off1 + 7, off2 + 5);
    CompareAndExchange2way(Array, off1 + 3, off2 + 1);
    CompareAndExchange2way(Array, off1 + 5, off2 + 4);
    CompareAndExchange2way(Array, off1 + 1, off1 + 4);
    CompareAndExchange2way(Array, off1 + 6, off2 + 2);
    CompareAndExchange2way(Array, off2 + 3, off2 + 6);

    CompareAndExchange2way(Array, off1 + 2, off1 + 4);
    CompareAndExchange2way(Array, off1 + 3, off1 + 5);
    CompareAndExchange2way(Array, off1 + 6, off2);
    CompareAndExchange2way(Array, off1 + 7, off2 + 1);
    CompareAndExchange2way(Array, off2 + 2, off2 + 4);
    CompareAndExchange2way(Array, off2 + 3, off2 + 5);

    CompareAndExchange2way(Array, off1 + 3, off1 + 6);
    CompareAndExchange2way(Array, off1 + 5, off2);
    CompareAndExchange2way(Array, off1 + 7, off2 + 2);
    CompareAndExchange2way(Array, off2 + 1, off2 + 4);

    CompareAndExchange2way(Array, off1 + 3, off1 + 4);
    CompareAndExchange2way(Array, off1 + 5, off1 + 6);
    CompareAndExchange2way(Array, off1 + 7, off2);
    CompareAndExchange2way(Array, off2 + 1, off2 + 2);
    CompareAndExchange2way(Array, off2 + 3, off2 + 4);

    return 61;
}