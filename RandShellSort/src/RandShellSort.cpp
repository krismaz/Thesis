/* 
 * File:   RandShellSort.cpp
 * Author: kris
 *
 * Created on 30 October 2014, 09:59
 */

/**
@file RandShellSort.cpp
 */


#include "Defines.h"
#include <cstdlib>
#include <algorithm>
#include <random>
#include "RandShellSort.h"
#include "Utils.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <strings.h>
#include "SIMD.h"
#include "CUDA.h"
#include <omp.h>

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

static void CompareAndExchange(int * const Array, unsigned int const i, unsigned int const j){ //Compiler inlines this automatically
    assert(i < j);
    assert(i >= 0);
    assert(j >= 0);
    comparisons++;

    CE(Array, i, j)
}

static void Sort16(int * const Array, unsigned int const off1, unsigned int const off2) {
    CompareAndExchange(Array, off1, off1 + 1);
    CompareAndExchange(Array, off1 + 2, off1 + 3);
    CompareAndExchange(Array, off1 + 4, off1 + 5);
    CompareAndExchange(Array, off1 + 6, off1 + 7);
    CompareAndExchange(Array, off2 + 0, off2 + 1);
    CompareAndExchange(Array, off2 + 2, off2 + 3);
    CompareAndExchange(Array, off2 + 4, off2 + 5);
    CompareAndExchange(Array, off2 + 6, off2 + 7);

    CompareAndExchange(Array, off1 + 0, off1 + 2);
    CompareAndExchange(Array, off1 + 1, off1 + 3);
    CompareAndExchange(Array, off1 + 4, off1 + 6);
    CompareAndExchange(Array, off1 + 5, off1 + 7);
    CompareAndExchange(Array, off2, off2 + 2);
    CompareAndExchange(Array, off2 + 1, off2 + 3);
    CompareAndExchange(Array, off2 + 4, off2 + 6);
    CompareAndExchange(Array, off2 + 5, off2 + 7);

#ifdef SIMD
    CompareExchange4(&Array[off1], &Array[off1 + 4]);
    CompareExchange4(&Array[off2], &Array[off2 + 4]);
    CompareExchange4(&Array[off1], &Array[off2]);
    CompareExchange4(&Array[off1 + 4], &Array[off2 + 4]);

    comparisons += 16;
#else
    CompareAndExchange(Array, off1 + 0, off1 + 4);
    CompareAndExchange(Array, off1 + 1, off1 + 5);
    CompareAndExchange(Array, off1 + 2, off1 + 6);
    CompareAndExchange(Array, off1 + 3, off1 + 7);
    CompareAndExchange(Array, off2, off2 + 4);
    CompareAndExchange(Array, off2 + 1, off2 + 5);
    CompareAndExchange(Array, off2 + 2, off2 + 6);
    CompareAndExchange(Array, off2 + 3, off2 + 7);

    CompareAndExchange(Array, off1 + 0, off2);
    CompareAndExchange(Array, off1 + 1, off2 + 1);
    CompareAndExchange(Array, off1 + 2, off2 + 2);
    CompareAndExchange(Array, off1 + 3, off2 + 3);
    CompareAndExchange(Array, off1 + 4, off2 + 4);
    CompareAndExchange(Array, off1 + 5, off2 + 5);
    CompareAndExchange(Array, off1 + 6, off2 + 6);
    CompareAndExchange(Array, off1 + 7, off2 + 7);
#endif
    CompareAndExchange(Array, off1 + 1, off1 + 2);
    CompareAndExchange(Array, off1 + 3, off2 + 4);
    CompareAndExchange(Array, off2 + 5, off2 + 6);
    CompareAndExchange(Array, off1 + 4, off2);
    CompareAndExchange(Array, off1 + 7, off2 + 3);
    CompareAndExchange(Array, off1 + 5, off2 + 2);
    CompareAndExchange(Array, off2 + 1, off1 + 6);

    CompareAndExchange(Array, off1 + 2, off2);
    CompareAndExchange(Array, off1 + 7, off2 + 5);
    CompareAndExchange(Array, off1 + 3, off2 + 1);
    CompareAndExchange(Array, off1 + 5, off2 + 4);
    CompareAndExchange(Array, off1 + 1, off1 + 4);
    CompareAndExchange(Array, off1 + 6, off2 + 2);
    CompareAndExchange(Array, off2 + 3, off2 + 6);

    CompareAndExchange(Array, off1 + 2, off1 + 4);
    CompareAndExchange(Array, off1 + 3, off1 + 5);
    CompareAndExchange(Array, off1 + 6, off2);
    CompareAndExchange(Array, off1 + 7, off2 + 1);
    CompareAndExchange(Array, off2 + 2, off2 + 4);
    CompareAndExchange(Array, off2 + 3, off2 + 5);

    CompareAndExchange(Array, off1 + 3, off1 + 6);
    CompareAndExchange(Array, off1 + 5, off2);
    CompareAndExchange(Array, off1 + 7, off2 + 2);
    CompareAndExchange(Array, off2 + 1, off2 + 4);

    CompareAndExchange(Array, off1 + 3, off1 + 4);
    CompareAndExchange(Array, off1 + 5, off1 + 6);
    CompareAndExchange(Array, off1 + 7, off2);
    CompareAndExchange(Array, off2 + 1, off2 + 2);
    CompareAndExchange(Array, off2 + 3, off2 + 4);
}


#ifdef INTELPRNG
static unsigned int g_seed; ///<Seed for PRNG
FASTSRAND(g_seed)
FASTRAND(g_seed)
#endif

static int* permutation; ///<Allocation is fairly expensive, so use a global permutation table

#ifdef CUDA
int * d_A;
int * d_perm;
#endif

#ifdef INTELPRNG

static inline size_t randWrapper(const unsigned int n) {
    return fast_rand() % n;
}
#endif

#ifdef SSE4PRNG
#define PRNGCACHESIZE 1024
static fastrand rander;
static unsigned int prngcache[PRNGCACHESIZE];
static unsigned int rcount = 0;

static inline size_t randWrapper(const int n) {


    if (rcount % PRNGCACHESIZE == 0) {
        for (int i = 0; i < PRNGCACHESIZE; i += 4) {
            FastRand_SSE4(&rander);
            std::memcpy(&prngcache[i], &(rander.res[0]), sizeof (int)*4);
        }
    }
    return prngcache[++rcount % PRNGCACHESIZE] % n;
}
#endif

#ifdef SIMD
int SIMDBuffer[4] __attribute__((aligned(16))); ///<16-bit aligned buffer for SIMD. Note that stack variables and alignment are a bitch, so it's global
#endif

template<int bits>
static void RegionCompare(int * const Array, unsigned int const r1, unsigned int const r2, unsigned int const N);

#ifdef BAILOUT

template<>
void RegionCompare<1>(int * const Array, unsigned int const r1, unsigned int const r2, unsigned int const N) {
}

template<>
void RegionCompare<2>(int * const Array, unsigned int const r1, unsigned int const r2, unsigned int const N) {
}

template<>
void RegionCompare<3>(int * const Array, unsigned int const r1, unsigned int const r2, unsigned int const N) {
    Sort16(Array, r1 * 8, r2 * 8);
}
#endif

/**
 * Compare-Exchange two region of the data
 * 
 * This performs the basic compare-exchange operation on 2 parts of the input
 *
 * @param Array array containing elements
 * @param [in] r1 starting index of first region
 * @param [in] r2 starting index of second region
 * @param [in] jump region size
 * @param [in] N size of Array
 * @pre r1+jump <= r2
 */
template<int bits>
static void RegionCompare(int * const Array, unsigned int const r1, unsigned int const r2, unsigned int const N) {
    unsigned int const jump = 1 << bits;
    for (int count = 0; count < PASSES; count++) {
#ifdef SIMD
        if (jump > 4) {
            //SIMD can be used for some pretty cool things, the following few lines is a loop that stores all the indexes of the first region in an array
            __m128i adder = _mm_set1_epi32(4);
            __m128i base = _mm_set_epi32(r1*jump, r1 * jump + 1, r1 * jump + 2, r1 * jump + 3);
            for (int i = 0; i < jump; i += 4) {
                _mm_store_si128((__m128i *) & permutation[i], base);
                base = _mm_add_epi32(base, adder);
            }
            std::random_shuffle(&permutation[0], &permutation[jump], randWrapper);
            for (int i = 0; i < jump; i += 4) {
                CompareExchange4(Array[permutation[i]], Array[permutation[i + 1]], Array[permutation[i + 2]], Array[permutation[i + 3]], &SIMDBuffer[0], &Array[r2 * jump + i]);
                comparisons += 4;
                Array[permutation[i]] = SIMDBuffer[0];
                Array[permutation[i + 1]] = SIMDBuffer[1];
                Array[permutation[i + 2]] = SIMDBuffer[2];
                Array[permutation[i + 3]] = SIMDBuffer[3];
            }
            return;
        }
#endif
#ifdef OMP
        if (bits > OMPBITRSS) {
            unsigned int sjump = jump;
            int rjump = std::min(jump, N - r2 * jump);
            #pragma omp parallel firstprivate(sjump, jump, Array, r1, r2, N, rjump) private(comparisons)
            {
                #pragma omp for schedule(static, sjump/OMPTHREADS)
                for (int i = 0; i < sjump; i++) {
                    permutation[i] = r1 * jump + i;
                }
                #pragma omp single
                    std::random_shuffle(&permutation[0], &permutation[jump], randWrapper);
                #pragma omp for schedule(static, sjump/OMPTHREADS)
                for (int i = 0; i < rjump; i++) {
                    CompareAndExchange(Array, permutation[i], r2 * jump + i);
                }
            }
            return;
        }
#endif
        for (int i = 0; i < jump; i++) {
            permutation[i] = r1 * jump + i;
        }

        int rjump = std::min(jump, N - r2 * jump);
        std::random_shuffle(&permutation[0], &permutation[jump], randWrapper);
#ifdef CUDA
        if (jump > 512) {
            MoveToDevice(d_perm, permutation, jump);
            CudaRegionCompare(d_A, r2, d_perm, jump);
            return;
        }
#endif

        for (int i = 0; i < rjump; i++) {
            CompareAndExchange(Array, permutation[i], r2 * jump + i);
        }
    }
}

template<int bits>
static void RandomizedShellSortJumper(int * const Array, unsigned int const N);

template<>
void RandomizedShellSortJumper<-1 > (int * const A, unsigned int const N) {
}

template<int bits>
static void RandomizedShellSortJumper(int * const Array, unsigned int const N) {
    unsigned int const jump = 1 << bits;
#ifdef CUDA
    if (jump == 512) {
        FromDevice(d_A, Array, N);
    }
#endif
    if (jump < N) {
        int regions = N / jump + ((N % jump)&1);

        //Shaker Phase
        for (int i = 0; i + 1 < regions; ++i) {
            RegionCompare<bits>(Array, i, i + 1, N);
        }
        for (int i = regions - 1; i > 0; --i) {
            RegionCompare<bits>(Array, i - 1, i, N);
        }

        //Brick Phase
        for (int i = 0; i + 3 < regions; ++i) {
            RegionCompare<bits>(Array, i, i + 3, N);
        }
        for (int i = 0; i + 2 < regions; ++i) {
            RegionCompare<bits>(Array, i, i + 2, N);
        }
        for (int i = 0; i + 1 < regions; i += 2) {
            RegionCompare<bits>(Array, i, i + 1, N);
        }
        for (int i = 1; i + 1 < regions; i += 2) {
            RegionCompare<bits>(Array, i, i + 1, N);
        }
    }
    RandomizedShellSortJumper < bits - 1 > (Array, N);
}

/**
 * Randomized ShellSort
 * 
 * This performs the sequence of region-compares that constitutes the Randomized ShellSort
 *
 * @param Array array containing elements
 * @param [in] N size of Array
 * 
 * @returns the number of compare-exchange operations performed
 */
unsigned long long RandomizedShellSort(int * const Array, unsigned int const N) {
    unsigned int two_msb = 1 << ((__builtin_clz(N)^0x1F)); //Largest power of 2 smaller than or equal to N
    permutation = new int[two_msb];
#ifdef INTELPRNG
    fast_srand(time(nullptr));
#endif
#ifdef SSE4PRNG
    InitFastRand(rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), &rander);
#endif
#ifdef CUDA
    d_A = ToDevice(Array, N);
    d_perm = OnDevice(N);
#endif
    RandomizedShellSortJumper<31>(Array, N);
    delete[] permutation;
#ifdef CUDA
    DestroyDevice(d_perm);
#endif
    return comparisons;
}


