#pragma once
#include <cuda_runtime.h>
#include <thrust/device_vector.h>
#include <helper_cuda.h>
#include <cusparse_v2.h>
#include <math.h>
#include <cooperative_groups.h>

#define CUDACHKERR() { \
cudaError_t ierr = cudaGetLastError();\
if (ierr != cudaSuccess){ \
    printf("%s in %s at line %d\n", cudaGetErrorString(ierr), __FILE__, __LINE__);\
    exit(EXIT_FAILURE); \
}\
}\

#define CUBLASCHKERR(cublas_err){\
if (cublas_err != CUBLAS_STATUS_SUCCESS){ \
    printf("%s in %s at line %d\n", _cudaGetErrorEnum(cublas_err) , __FILE__, __LINE__);\
    exit(EXIT_FAILURE); \
}\
}\

namespace cuFSP{
    typedef double (*PropFun) (int* x, int reaction);
    typedef thrust::device_vector<double> thrust_dvec;

    struct CSRMat {
        double *vals = nullptr;
        int *col_idxs = nullptr;
        int *row_ptrs = nullptr;
        int n_rows, n_cols, nnz;
    };

    struct CSRMatInt {
        int *vals = nullptr;
        int *col_idxs = nullptr;
        int *row_ptrs = nullptr;
        int n_rows, n_cols, nnz;
    };

    struct SDKronMat{
        int d;
        double *vals = nullptr;
        int *offsets;
    };

    __global__
    void fsp_get_states(int *d_states, int dim, int n_states, int *n_bounds);

    __device__ __host__
    void indx2state(int indx, int *state, int dim, int *fsp_bounds);

    __device__ __host__
    int state2indx(int *state, int dim, int *fsp_bounds);

    __host__
    __device__
    void reachable_state(int *state, int *rstate, int reaction, int direction,
                         int n_species, int *stoich_val, int *stoich_colidxs, int *stoich_rowptrs);

    __device__ __host__
    int rect_fsp_num_states(int n_species, int *fsp_bounds);
}

