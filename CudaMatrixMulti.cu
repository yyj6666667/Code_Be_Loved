/*
 *这份代码对于作者来说意义很深，教会了作者用大的眼光看待TILE中thread
 *以下均为作者手打
*/

#include <iostream>

#define TILE_WIDTH 8

__global__ void MatrixMulSharedMemKernel(float* M, float* N, float* P, 
    int m, int n) { 
        //分配shared Memory
        __shared__ float Ms[TILE_WIDTH][TILE_WIDTH];
        __shared__ float Ns[TILE_WIDTH][TILE_WIDTH];
        int bx = blockIdx.x;  int by = blockIdx.y;
        int tx = threadIdx.x; int ty = threadIdx.y;
        //计算在大矩阵中的真实索引
        int col = bx * blockDim.x + tx;
        int row = by * blockDim.y + ty;
        //计算迭代次数
        int nIter = ceil(m * 1.0 / TILE_WIDTH);
        //对于每一个TILE中的Thread而言，这个Pvalue是单独的
        float Pvalue = 0;
        for (int i = 0; i < nIter; i++) {
            //装填SharedMem of TILE size
            Ms[ty][tx] = M[row * n + tx + i * TILE_WIDTH];   //M[row][tx + i * TILE_WIDTH], 这里的 i * TILE_WIDTH 可以看成是地址偏移后的起始基地址
            Ns[ty][tx] = N[(ty + i * TILE_WIDTH) * n + col]; //N[ty + i * TILE_WIDTH][col]
            __syncthreads();
            
            for (int j = 0; j < TILE_WIDTH; j++) {
                Pvalue += Ms[ty][j] * Ns[j][tx];
            }
            __syncthreads();
        }
        P[row * n + col] = Pvalue;
    }

int main(int argc, char* argv[]) {
    int size, check;
    getArg(argc, argv, size, check);
    int m = size, n = size, k = size;
    //declare matrix of host and device(GPU)
    float *h_M, *h_N, *h_P, *d_M, *d_N, *d_P;
    size_t sizeM = m * k * sizeof(float);
    size_t sizeN = k * n * sizeof(float);
    size_t sizeP = m * n * sizeof(float);

    h_M = (float*) malloc(sizeM);
    h_N = (float*) malloc(sizeN);
    h_P = (float*) malloc(sizeP);

    cudaMalloc(&d_M, sizeM);
    cudaMalloc(&d_N, sizeN);
    cudaMalloc(&d_P, sizeP);

    cudaMemcpy(d_M, h_M, sizeM, cudaMemcpyHostToDevice);
    cudaMemcpy(d_N, h_N, sizeN, cudaMemcpyHostToDevice);

    dim3 grid(ceil(n*1.0 / TILE_WIDTH), ceil(m*1.0 / TILE_WIDTH));
    dim3 block(TILE_WIDTH, TILE_WIDTH);

    int nIter = 5;//避免冷启动
    for(int i = 0; i < nIter; i++) {
        MatrixMulSharedMemKernel<<<grid, block>>>(d_M, d_N, d_P, m, n);
    }
    printf("finished");

    free(h_P);
    free(h_N);
    free(h_M);
    cudafree(d_P);
    cudafree(d_M);
    cudafree(d_N);

    return 0;
}