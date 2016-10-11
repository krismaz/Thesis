/* 
 * File:   Comparables.cpp
 * Author: kris
 *
 * Created on 30 October 2014, 18:28
 */
#include<stdlib.h>
#include <algorithm>

/**
 @file Comparables.cpp
 */

using namespace std;

static unsigned long long comparisons = 0; ///<file-internal counter for number fo comparisons

/**
 * c-style comparator for qsort
 *
 * @param [in] a pointer to first element
 * @param [in] b pointer to second element
 * 
 * @returns 1, 0, -1 depending on the content of a and b
 */
static int intCompare(const void* a, const void* b) {
    comparisons++;
    int ia = *((int*) a);
    int ib = *((int*) b);
    if (ia > ib) return 1;
    if (ia == ib) return 0;
    return -1;
}

/**
 * cppstd-style comparator for stdsort
 *
 * @param [in] a first element
 * @param [in] b second element
 * 
 * @returns a < b
 */
static bool STDIntCompare(int a, int b) {
    comparisons++;
    return a < b;
}

/**
 * QSort wrapper
 * 
 * This makes a call to qsort with the given data, it should be O(nlogn), but no guarantee is given
 *
 * @param Array array containing elements
 * @param [in] N size of Array
 * 
 * @returns the number of comparison done
 */
unsigned long long QSortWrap(int* Array, int N) {
    qsort(Array, N, sizeof (int), intCompare);
    return comparisons;
}

/**
 * Insertion Sort
 * 
 * Insertion Sorts the array, O(n) for sorted data, O(n^2) worst case, highly adaptive
 * Performs N-1 comparisons on sorted data
 *
 * @param Array array containing elements
 * @param [in] N size of Array
 * 
 * @returns the number of comparisons done
 */

unsigned long long InsertionSort(int* Array, int N) {
    for (int i = 1; i < N; ++i) {
        int cur = Array[i];
        int point = i;
        while (point > 0 && ++comparisons && Array[point - 1] > cur) {
            Array[point] = Array[point - 1];
            --point;
        }
        Array[point] = cur;
    }
    return comparisons;
}

/**
 * std::sort wrapper
 * 
 * This makes a call to std::sort with the given data, it is worst-case O(nlogn) from c++11 and ahead
 *
 * @param Array array containing elements
 * @param [in] N size of Array
 * 
 * @returns the number of comparison done
 */
unsigned long long STDSort(int* Array, int N) {
    sort(&Array[0], &Array[N], STDIntCompare);
    return comparisons;
}

unsigned long long TouchMem(int* Array, int N) {
    long long touches = 0;
    for (int k = N; k > 0; k /= 4) {
        for (int i = 0; i < 1; i++) {
            for (int l = 0; l < 1024; l++) {
                for (int j = l; j < N; j += 1024) {
                    Array[j]++;
                    touches++;
                }
            }
        }
    }
    return touches;
}