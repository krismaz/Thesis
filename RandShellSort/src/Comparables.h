/* 
 * File:   Comparables.h
 * Author: kris
 *
 * Created on 30 October 2014, 18:26
 */

/**
@file Comparables.h
*/

#ifndef COMPARABLES_H
#define	COMPARABLES_H

///std::qsort wrapper, no-one has any guarantees for this guy
unsigned long long QSortWrap(int* Array, int N);

///Insertionsort, O(n^2) but highly adaptive
unsigned long long InsertionSort(int* Array, int N);

///std::sort wrapper, O(nlogn) and all-round good guy
unsigned long long STDSort(int* Array, int N);

unsigned long long TouchMem(int* Array, int N);

#endif	/* COMPARABLES_H */

