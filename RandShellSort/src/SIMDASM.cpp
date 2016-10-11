/* 
 * File:   SIMDASM.cpp
 * Author: kris
 *
 * Created on 23 November 2014, 17:21
 */

#include <algorithm>


void static inline CompareExchange(int& A, int& B){
	if(B<A){
		std::swap(A,B);
	}
}



void CompareExchange4(int* A, int* B){
	CompareExchange(A[0], B[0]);
	CompareExchange(A[1], B[1]);
	CompareExchange(A[2], B[2]);
	CompareExchange(A[3], B[3]);
}
