/* 
 * File:   Defines.h
 * Author: kris
 *
 * Created on 20 November 2014, 12:39
 */

#ifndef DEFINES_H
#define	DEFINES_H

#ifdef	_ASSERT_H
#pragma GCC warning "cassert included before defines"
#endif

#define CUDAMULTISTEPSIZE 1024
#define CUDAMULTISTEPSIZEB 9

#define CUDATHREADS 512 ///<Maximum Cuda Threads
#define CUDABLOCKX (1<<16) ///<Maximum Cuda block dim
#define CUDAWARPCORES (32*6) ///<Processor count time warp size

#define OMPBITRSS 16 ///<Use only OMP when size bigger than 2^k
#define OMPBITPRATT 12 ///<Use only OMP when size bigger than 2^k
#define OMPBITSSHAKER 6 ///<Use only OMP when size bigger than 2^k
#define OMPBITSOEMS 14 ///<Use only OMP when size bigger than 2^k
#define OMPBITSBS 16 ///<Use only OMP when size bigger than 2^k
#define OMPTHREADS 4 ///<How many virtual cores do we have?
#define L1WIDTH 1024 ///<Shellsort L1 cache shifter
#define LEAVINGMERGE ///<Use memory-leaving in Odd-Even Merge sort, turning this off turns cache-misses up to 11

#define DOUBLELAYER

/**
 * Parallel?
 * SIMD is SSE4.1
 * CUDA is CUDA
 * OMP is OpenMP
 *
 */
#define NONE

//#define BAILOUT ///<Switch to 16-sorter at small data sizes

#define PASSES 1 ///< Amount of compare exchanges, trades off speed for sorting chance

/**
 * Random number generator to use in RandShellSort
 * 
 * INTELPRNG is a strangely fast PRNG from Intel, that seems to be super duper fast
 * SSE4PRNG is a SSE4-based PRNG, that should be super fast, but somehow falls flat. 70% slower than intel one even when batching is used
 */
#define INTELPRNG

#define NDEBUG ///<Disable Assertions


/**
 * CompareExchange version
 * 
 * XOR, MINMAX, SWAP, LOG
 */
#define XOR

//http://www.graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax is the true source of the min-max no-branching swap
#ifdef XOR
#define CE(Array,i,j) \
    int x = Array[i]; \
    int y = Array[j]; \
    int mask = ((x ^ y) & -(x < y)); \
    Array[i] = y ^ mask; \
    Array[j] = x ^ mask; 
#endif

#ifdef SWAP
#ifndef __NVCC__
#define CE(Array, i, j) \
     if(Array[i] > Array[j]) std::swap(Array[i], Array[j]);
#else
#define CE(A, i, j)
#endif
   
#endif

#ifdef MINMAX
#ifndef __NVCC__
#define CE(Array, i, j) \
     int x = Array[i]; \
    Array[i] = std::min(Array[i], Array[j]); \
    Array[j] = std::max(x, Array[j]);
#else
#define CE(A, i, j)
#endif
#endif

#ifdef LOG
#ifndef __NVCC__
#define CE(A, i, j) \
    std::cout << "(" << i << "," << j << ")" << std::endl;
#else
#define CE(A, i, j)
#endif
#endif
#endif	/* DEFINES_H */

