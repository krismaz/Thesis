/* 
 * File:   AnnealingSort.h
 * Author: kris
 *
 * Created on 08 November 2014, 16:12
 */

/**
@file AnnealingSort.cpp
*/


#include "Defines.h"
#include "AnnealingSort.h"
#include "Utils.h"
#include <cassert>
#include <ctgmath>
#include <vector>
#include <ctime>
#include <iostream>

unsigned long long comparisons = 0;

static unsigned int g_seed; ///<Seed for PRNG
FASTSRAND(g_seed)
FASTRAND(g_seed)

using namespace std;

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
static void CompareAndExchange(int * const Array, unsigned int const i, unsigned int const j, unsigned int const N) {
    assert(i<j);
    assert(j<N);
    assert(i<N);
    assert(i>=0);
    assert(j>=0);
    comparisons++;
    
    CE(Array, i, j)
}


unsigned int MinSwap(unsigned int a, unsigned int b){
    return a ^ ((b ^ a) & -(b < a));
}

unsigned long long AnnealingSort(int * const Array , unsigned int const N, double scale, double h, double q, int c){
    fast_srand(time(0));
    float e(2.71828182845904523536);
    
    unsigned int r(h * log2(N)/log2(log2(N)));

    unsigned int p1Limit(q*pow(log2(N),6)); //This is lower than 2N, unless N is super big (300 mil?)

    unsigned int p2Limit(64.0*e*e*log2(N)*scale+1); //This is what kills performance, and scale can be super low and still work
    
    unsigned int jump(2*N);

    //Start building the annealing sequence
    vector<unsigned int> T;
    vector<unsigned int> R;

    while(jump > p1Limit){
            T.push_back(jump);
            T.push_back(jump);
            R.push_back(c);
            R.push_back(c);
            jump = jump/2;
    }

    jump = p1Limit;

    while(jump > p2Limit){
            T.push_back(jump);
            R.push_back(r);
            jump = jump/2;
    }

    for(auto i = 0; i < p2Limit;i++){
            T.push_back(1);
            R.push_back(1);
    }
    //Main part of the algorithm
    for(auto rt = 0; rt < T.size(); ++rt){
        unsigned int r(R[rt]);
        unsigned int t(T[rt]); 
        for (int i = 0; i < N-1;  ++i){
          for(int k = 0; k<r; ++k){
            unsigned int s = i+1+fast_rand() % MinSwap(N-i,t);
            CompareAndExchange(Array, i, s, N);
        }
    }
    for (int i = N-1; i > 0;  --i){
      for(int k = 0; k<r; ++k){
         unsigned int s = i-1-fast_rand() % MinSwap(t, i);
         CompareAndExchange(Array, s, i, N);
     }
 }
}
    return comparisons;
}


unsigned long long AnnealingSort(int * const Array , unsigned int const N){
    return AnnealingSort(Array, N, 1.0, 2.0, 1.0, 10);
}