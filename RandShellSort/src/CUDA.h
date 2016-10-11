/* 
 * File:   CUDA.h
 * Author: kris
 *
 * Created on 03 December 2014, 15:44
 */

#ifndef CUDA_H
#define	CUDA_H
#include <cuda.h>
#include <cuda_runtime.h>

__host__ void CudaHShakeUp(int* A, int H, int N);
__host__ void CudaHShakeDown(int* A, int H, int N);
__host__ void CudaBitonicSort(int* A, int N);
__host__ void CudaOddEvenMergeSort(int* A, int N);
__host__ void CudaShuffle(int* A, int N);//Do not use me I don not work
__host__ void CudaRegionCompare(int* A,int r2, int * perm, int jump);
__host__ int * ToDevice(int* src, int N);
__host__ void FromDevice(int* dev, int * host, int N);
__host__ int * OnDevice(int N);
__host__ void DestroyDevice(int* dev);
__host__ void MoveToDevice(int* dev, int * host, int N);

#endif	/* CUDA_H */

