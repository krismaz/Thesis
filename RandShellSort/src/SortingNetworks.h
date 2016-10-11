/* 
 * File:   SortingNetworks.h
 * Author: kris
 *
 * Created on 10 November 2014, 14:44
 */

#ifndef SORTINGNETWORKS_H
#define	SORTINGNETWORKS_H


unsigned long long BitonicSort(int * const Array , unsigned int const N);
unsigned long long BitonicSort2(int * const Array , unsigned int const N);
unsigned long long BitonicSort3(int * const Array , unsigned int const N);
unsigned long long BitonicSort4(int * const Array , unsigned int const N);
unsigned long long OddEvenMergeSort(int * const Array , unsigned int const N);
unsigned long long OddEvenMergeSort2(int * const Array , unsigned int const N);
unsigned long long OddEvenMergeSort3(int * const Array, unsigned int const N);
unsigned long long Sort16(int * const Array, unsigned int const off1 = 0,  unsigned int const off2 = 8);
unsigned long long PeriodicSort2(int * const Array , unsigned int const N);

#endif	/* SORTINGNETWORKS_H */

