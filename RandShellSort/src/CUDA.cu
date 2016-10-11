/* 
 * File:   CUDA.cu
 * Author: kris
 *
 * Created on 03 December 2014, 17:25
 */

/**
@file CUDA.cu
 */
#include "Defines.h"
#include "CUDA.h"
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <algorithm>
#include <curand_kernel.h>
#include <ctime>

using namespace std;



//Following CUDA error checking code was taken from a StackOverflow thread
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true) {
    if (code != cudaSuccess) {
        cerr << "GPUassert: " << cudaGetErrorString(code) << file << line << " - " << code << endl;
        if (abort) exit(code);
    }
}
//End CUDA error checking from stackoverflow

__global__ void CudaHShakeUpDev(int * A, int offset, int max) {
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= offset) return;
    for (int i = idx; i < max - offset; i += offset) {
        CE(A, i, i + offset);
    }
}

__global__ void CudaHShakeDownDev(int * A, int offset, int max) {
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= offset) return;
    for (int i = max - offset + idx; i >= offset; i -= offset) {
        CE(A, i - offset, i);
    }
}

__global__ void CudaBitonicStep(int * A, int m, int ms, int NT) {
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= NT) return;
    bool ascending = (idx & m - 1) < m >> 1; //idx % m < m / 2; 
    //int i = idx % (ms / 2) + ms * (idx / (ms / 2));
    int i = (idx & (ms - 1)) + ((idx << 1) & (~((ms << 1) - 1)));
    if (ascending) {
        CE(A, i, i + ms)
    } else {
        CE(A, i + ms, i)
    }
}

__global__ void CudaBitonicMultiStep(int * A, int NT) {
    uint bidx = blockIdx.y * blockDim.y + blockIdx.x;
    uint tidx = threadIdx.x;
    if (bidx * blockDim.x + tidx >= NT) return;
    int __shared__ cache[CUDAMULTISTEPSIZE];
    cache[tidx] = A[bidx * CUDAMULTISTEPSIZE + tidx];
    cache[tidx + CUDAMULTISTEPSIZE / 2] = A[bidx * CUDAMULTISTEPSIZE + tidx + CUDAMULTISTEPSIZE / 2];
    __syncthreads();
#pragma unroll
    for (int s = 1; s <= CUDAMULTISTEPSIZEB; s++) {
        uint sn = 1 << s;
        bool ascending = tidx % sn < sn / 2;
#pragma unroll //CUDA is not happy about this
        for (int m = s - 1; m >= 0; m--) {
            uint mn = 1 << m;
            uint i = (tidx & (mn - 1)) + ((tidx << 1) & (~((mn << 1) - 1)));
            if (ascending) {
                CE(cache, i, i + mn)
            } else {
                CE(cache, i + mn, i)
            }
            __syncthreads();
        }
    }
    A[bidx * CUDAMULTISTEPSIZE + tidx] = cache[tidx];
    A[bidx * CUDAMULTISTEPSIZE + tidx + CUDAMULTISTEPSIZE / 2] = cache[tidx + CUDAMULTISTEPSIZE / 2];
}

__global__ void CudaBitonicMultiMergeStep(int * A, uint sn, int NT) {
    uint bidx = blockIdx.y * blockDim.y + blockIdx.x;
    uint tidx = threadIdx.x;
    if (bidx * blockDim.x + tidx >= NT) return;
    int __shared__ cache[CUDAMULTISTEPSIZE];
    cache[tidx] = A[bidx * CUDAMULTISTEPSIZE + tidx];
    cache[tidx + CUDAMULTISTEPSIZE / 2] = A[bidx * CUDAMULTISTEPSIZE + tidx + CUDAMULTISTEPSIZE / 2];
    __syncthreads();
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    bool ascending = idx % sn < sn / 2;
#pragma unroll //CUDA is not happy about this
    for (int m = CUDAMULTISTEPSIZEB; m >= 0; m--) {
        uint mn = 1 << m;
        uint i = (tidx & (mn - 1)) + ((tidx << 1) & (~((mn << 1) - 1)));
        if (ascending) {
            CE(cache, i, i + mn)
        } else {
            CE(cache, i + mn, i)
        }
        __syncthreads();
    }
    A[bidx * CUDAMULTISTEPSIZE + tidx] = cache[tidx];
    A[bidx * CUDAMULTISTEPSIZE + tidx + CUDAMULTISTEPSIZE / 2] = cache[tidx + CUDAMULTISTEPSIZE / 2];
}

__global__ void CudaShuffleStep(int * A, curandState *state, int seed, int ms, int NT) { //No shuffling done here
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= NT) return;
    if (ms == NT * 2) {
        if (idx < 512) {
            curand_init(seed, idx, 0, &state[idx]);
        }
        return;
    }
    int r = curand(&(state[idx % 512]));
    int i = idx % (ms / 2) + ms * (idx / (ms / 2));
    //int i = (idx & (ms-1)) + ((idx << 1) & (~((ms<<1)-1)));
    if (r % 2 == 1) {
        int temp = A[i];
        A[i] = A[i + ms];
        A[i + ms] = temp;
    }
}

__global__ void CudaMergeStep(int * A, int p, int r, int d, int NT) {
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= NT) return;
    if ((idx & p) == r) {
        CE(A, idx, idx + d);
    }
}

__global__ void CudaOddEvenMultiMergeStep(int * A, int NT) {
    uint bidx = blockIdx.y * blockDim.y + blockIdx.x;
    uint tidx = threadIdx.x;
    if (bidx * blockDim.x + tidx >= NT) return;
    int __shared__ cache[CUDAMULTISTEPSIZE/2];
    cache[tidx] = A[bidx * CUDAMULTISTEPSIZE/2 + tidx];
    __syncthreads();
    for (int p = CUDAMULTISTEPSIZE / 4; p > 0; p /= 2) {
        int d = p;
        int r = 0;
        for (int q = CUDAMULTISTEPSIZE / 4; q >= p; q /= 2) {
            int H = CUDAMULTISTEPSIZE/2  - d;

            
            if ((tidx & p) == r && tidx < H) {
                CE(cache, tidx, tidx + d);
            }

            d = q - p;
            r = p;
            __syncthreads();
        }
    }
    __syncthreads();
    A[bidx * CUDAMULTISTEPSIZE/2 + tidx] = cache[tidx];
}

texture<int, 1, cudaReadModeElementType> perm;

__global__ void CudaRegionCompareDev(int* A, int r2, int jump) {
    uint idx = blockIdx.y * blockDim.x * blockDim.y + blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= jump) return;
    int id = tex1Dfetch(perm, idx);
    CE(A, id, r2 * jump + idx);
}

__host__ int* ToDevice(int * A, int N) {
    int* d_A;
    cudaMalloc(&d_A, N * sizeof (int));
    cudaMemcpy(d_A, A, N * sizeof (int), cudaMemcpyHostToDevice);
    return d_A;
}

__host__ void FromDevice(int * dev, int* host, int N) {
    cudaMemcpy(host, dev, N * sizeof (int), cudaMemcpyDeviceToHost);
    cudaFree(dev);
}

__host__ int * OnDevice(int N) {
    int* d_A;
    cudaMalloc(&d_A, N * sizeof (int));
    return d_A;
}

__host__ void DestroyDevice(int* dev) {
    cudaFree(dev);
}

__host__ void MoveToDevice(int* dev, int * host, int N) {
    cudaMemcpy(dev, host, N * sizeof (int), cudaMemcpyHostToDevice);
}

__host__ void CudaHShakeUp(int* A, int H, int N) {
    dim3 grid_size;
    grid_size.x = min(CUDABLOCKX, H / CUDATHREADS + (H % CUDATHREADS != 0));
    grid_size.y = H / (CUDABLOCKX * CUDATHREADS) + (H % (CUDABLOCKX * CUDATHREADS) != 0);

    CudaHShakeUpDev << <grid_size, min(CUDATHREADS, H)>>>(A, H, N);
    //gpuErrchk( cudaPeekAtLastError() );
    //gpuErrchk( cudaDeviceSynchronize() );
}

__host__ void CudaHShakeDown(int* A, int H, int N) {
    dim3 grid_size;
    grid_size.x = min(CUDABLOCKX, H / CUDATHREADS + (H % CUDATHREADS != 0));
    grid_size.y = H / (CUDABLOCKX * CUDATHREADS) + (H % (CUDABLOCKX * CUDATHREADS) != 0);

    CudaHShakeDownDev << <grid_size, min(CUDATHREADS, H)>>>(A, H, N);
    //gpuErrchk( cudaPeekAtLastError() );
    //gpuErrchk( cudaDeviceSynchronize() );
}

__host__ void CudaBitonicSort(int* A, int N) {
    int H = N / 2;
    dim3 grid_size;
    grid_size.x = min(CUDABLOCKX, H / CUDATHREADS + (H % CUDATHREADS != 0));
    grid_size.y = H / (CUDABLOCKX * CUDATHREADS) + (H % (CUDABLOCKX * CUDATHREADS) != 0);
    dim3 sgrid_size;
    sgrid_size.x = min(CUDABLOCKX, H / (CUDAMULTISTEPSIZE / 2) + (H % (CUDAMULTISTEPSIZE / 2) != 0));
    sgrid_size.y = H / (CUDABLOCKX * (CUDAMULTISTEPSIZE / 2)) + (H % (CUDABLOCKX * (CUDAMULTISTEPSIZE / 2)) != 0);

    CudaBitonicMultiStep << <sgrid_size, CUDAMULTISTEPSIZE / 2 >> >(A, H);
    for (int i = CUDAMULTISTEPSIZE; i <= N; i *= 2) { //Sorting size

        for (int ms = i / 2; ms > 0; ms /= 2) {
            if (ms == CUDAMULTISTEPSIZE / 2) {
                CudaBitonicMultiMergeStep << <sgrid_size, CUDAMULTISTEPSIZE / 2 >> >(A, i, H);
                break;
            }
            CudaBitonicStep << <grid_size, CUDATHREADS>>>(A, i, ms, H);
        }
    }
    gpuErrchk(cudaPeekAtLastError());
    gpuErrchk(cudaDeviceSynchronize());
}

__host__ void CudaOddEvenMergeSort(int* A, int N) {
    //Based Chinese scientists

    for (int p = N / 2; p > 0; p /= 2) {
        int d = p;
        int r = 0;
        for (int q = N / 2; q >= p; q /= 2) {
            int H = N - d;
            dim3 grid_size;
            grid_size.x = min(CUDABLOCKX, H / CUDATHREADS + (H % CUDATHREADS != 0));
            grid_size.y = H / (CUDABLOCKX * CUDATHREADS) + (H % (CUDABLOCKX * CUDATHREADS) != 0);

            CudaMergeStep << <grid_size, CUDATHREADS>>>(A, p, r, d, H);

            d = q - p;
            r = p;
        }
    }
    gpuErrchk(cudaPeekAtLastError());
    gpuErrchk(cudaDeviceSynchronize());
    
}

__host__ void CudaShuffle(int* A, int N) { //This doesn't shuffle
    int H = N / 2;
    dim3 grid_size;
    grid_size.x = min(CUDABLOCKX, H / CUDATHREADS + (H % CUDATHREADS != 0));
    grid_size.y = H / (CUDABLOCKX * CUDATHREADS) + (H % (CUDABLOCKX * CUDATHREADS) != 0);
    curandState *devStates;
    cudaMalloc(&devStates, 512 * sizeof (curandState));
    for (int i = N; i > 0; i /= 2) {
        CudaShuffleStep << <grid_size, CUDATHREADS>>>(A, devStates, time(0), i, H);
    }
    gpuErrchk(cudaPeekAtLastError());
    gpuErrchk(cudaDeviceSynchronize());

}

__host__ void CudaRegionCompare(int* A, int r2, int * d_perm, int jump) {
    cudaBindTexture(0, perm, d_perm, jump * sizeof (int));
    dim3 grid_size;
    grid_size.x = min(CUDABLOCKX, jump / CUDATHREADS + (jump % CUDATHREADS != 0));
    grid_size.y = jump / (CUDABLOCKX * CUDATHREADS) + (jump % (CUDABLOCKX * CUDATHREADS) != 0);
    CudaRegionCompareDev << <grid_size, CUDATHREADS>>>(A, r2, jump);
    //gpuErrchk(cudaPeekAtLastError());
    //gpuErrchk(cudaDeviceSynchronize());
    cudaUnbindTexture(perm);
}