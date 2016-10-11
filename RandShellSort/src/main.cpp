/* 
 * File:   main.cpp
 * Author: kris
 *
 * Created on 30 October 2014, 09:34
 */

/**
@file main.cpp
*/
#include "Defines.h"
#include <cstdlib>
#include <iostream>
#include <locale>
#include <algorithm>
#include "IO.h"
#include "RandShellSort.h"
#include "Comparables.h"
#include "AnnealingSort.h"
#include "SortingNetworks.h"
#include "Utils.h"
#include "Shellsorts.h"
#include "CUDA.h"


static unsigned int g_seed; ///<Seed for PRNG
FASTSRAND(g_seed)
FASTRAND(g_seed)

using namespace std;


static unsigned long long comparisons = 0; ///<Counter for comparisons performed

static bool STDIntCompare(int a, int b){
    comparisons++;
    return a < b;
}

/**
 * Main function
 * 
 * Run with algorithm name and data file to sort
 *
 * @param [in] argc number of arguments
 * @param [in] argv pointer to arguments
 * 
 * @returns 0
 */

int main(int argc, char** argv) {
    //cout << "Compiled " << __DATE__ << " " << __TIME__ << endl;
    if(argc == 1)
    {
        int size = -1;
        int* data = read_file(size, "data.dat");
        cout << ((size_t)data)%(16) << endl;
        cout << size << endl;
        if (size == -1) return 1;
#ifdef CUDA
        if(size < 1024){
            cout << "WARNING, size less than 1024, CUDA might break." << endl;
        }
#endif
        if(size !=  1<<((__builtin_clz(size)^0x1F))){
            cout << "WARNING, size not a power of 2, some algorithms, and SIMD/CUDA, might not work." << endl;
        }
        cerr << TouchMem(data, size) << endl;
        //cerr << InsertionSort(data, size) << endl;
        cout << data [0] << endl;
        delete[] data;
    }
    else 
    {
        unsigned long long totalComparisons = 0;
        string operation = argv[1];
        if(operation == "Run")
        {
            string algorithm = argv[2];
            int size = -1;
            int* data = read_file(size, argv[3]);
            if(size !=  1<<((__builtin_clz(size)^0x1F))){
                cout << "WARNING, size not a power of 2, some algorithms, and SIMD, might not work." << endl;
            }
            if(algorithm == "RShellSortIns"){
                totalComparisons += RandomizedShellSort(data, size);
                totalComparisons +=InsertionSort(data, size);
            } else if (algorithm == "RShellSortVegas"){
                do{
                    totalComparisons += RandomizedShellSort(data, size);
                } while(!is_sorted(&data[0], &data[size], STDIntCompare));
                totalComparisons += comparisons;
            } else if (algorithm == "RShellSortPure"){
                totalComparisons += RandomizedShellSort(data, size);
            } else if(algorithm == "STDSort"){
               totalComparisons += STDSort(data, size);
            } else if(algorithm == "AnnealingSortIns"){
                totalComparisons += AnnealingSort(data, size, 0.0, 1.0, 1.0, 10);
                totalComparisons += InsertionSort(data, size);
            } else if(algorithm == "AnnealingSortIns"){
                do{
                totalComparisons += AnnealingSort(data, size, 0.0, 1.0, 1.0, 10);
                } while(!is_sorted(&data[0], &data[size], STDIntCompare));
                totalComparisons += comparisons;
            } else if(algorithm == "AnnealingSortPure"){
                totalComparisons += AnnealingSort(data, size, 0.0, 1.0, 1.0, 10);
            } else if(algorithm == "BitonicSort"){
                totalComparisons += BitonicSort2(data, size);
            }else if(algorithm == "BitonicSortOld"){
                totalComparisons += BitonicSort(data, size);
            } else if(algorithm == "OddEvenMergeSort"){
                totalComparisons += OddEvenMergeSort2(data, size);
            } else if(algorithm == "BitonicSortCUDA"){
                totalComparisons += BitonicSort4(data, size);
            } else if(algorithm == "OddEvenMergeSortCUDA"){
                totalComparisons += OddEvenMergeSort3(data, size);
            } else if(algorithm == "Pratt"){
                totalComparisons += PrattsShellsort(data, size);
            } else if(algorithm == "ShakerSort"){
                totalComparisons += ShakerSort(data, size);
            }
            cerr << totalComparisons << ",comparisons" << endl;
            delete[] data;
        } else if(operation == "AnnealingTest")
        {
            fast_srand(time(nullptr));
            int n = atoi(argv[2]);
            float scale = atof(argv[3]);
            float h = atof(argv[4]);
            int * data = new int[n];
            int wrong = 0;
            for(int i = 0; i < 100; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    data[j] = fast_rand();
                }
                unsigned long long comparisons = AnnealingSort(data, n, scale, h, 1.0, 1);
                cerr << comparisons << ",comparisons" << endl;
                if(!is_sorted(&data[0], &data[n], STDIntCompare))
                    wrong += 1;
            }
            cerr << wrong << ",errors" << endl;
        }
        else if(operation == "ShakerSortTest")
        {
            fast_srand(time(nullptr));
            int n = atoi(argv[2]);
            int * data = new int[n];
            int wrong = 0;
            for(int i = 0; i < 100; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    data[j] = fast_rand();
                }
                unsigned long long comparisons = ShakerSort(data, n);
                cerr << comparisons << ",comparisons" << endl;
                if(!is_sorted(&data[0], &data[n], STDIntCompare))
                    wrong += 1;
            }
            cerr << wrong << ",errors" << endl;
        }
    }
   
   
    return 0;
}


