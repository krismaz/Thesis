/* 
 * File:   SIMD.h
 * Author: kris
 *
 * Created on 13 November 2014, 16:39
 */

#ifndef SIMD_H
#define	SIMD_H

#include <stdint.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include "Utils.h"

inline void CompareExchange4(int * const Array1, int * const Array2){
    __m128i a = _mm_load_si128((const __m128i *)Array1);
    __m128i b = _mm_load_si128((const __m128i *)Array2);
    _mm_store_si128((__m128i *)Array1, _mm_min_epi32(a,b));
    _mm_store_si128((__m128i *)Array2, _mm_max_epi32(a,b));
}

inline void CompareExchange4U(int * const Array1, int * const Array2){
    __m128i a = _mm_load_si128((const __m128i *)Array1);
    __m128i b = _mm_loadu_si128((const __m128i *)Array2);
    _mm_store_si128((__m128i *)Array1, _mm_min_epi32(a,b));
    _mm_storeu_si128((__m128i *)Array2, _mm_max_epi32(a,b));
}

inline void CompareExchange4UU(int * const Array1, int * const Array2){
    __m128i a = _mm_loadu_si128((const __m128i *)Array1);
    __m128i b = _mm_loadu_si128((const __m128i *)Array2);
    _mm_storeu_si128((__m128i *)Array1, _mm_min_epi32(a,b));
    _mm_storeu_si128((__m128i *)Array2, _mm_max_epi32(a,b));
}

inline void CompareExchange4(int i1, int i2, int i3, int i4, int * const Array1, int * const Array2){
    __m128i a =  _mm_set_epi32(i1, i2, i3, i4);
    __m128i b = _mm_load_si128((const __m128i *)Array2);
    _mm_store_si128((__m128i *)Array1, _mm_min_epi32(a,b));
    _mm_store_si128((__m128i *)Array2, _mm_max_epi32(a,b));
}
#endif	/* SIMD_H */

