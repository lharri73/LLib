#pragma once
#include <cuda_fp16.h>
#include "libCUDSafe/cudaUtils.hpp"

// Defines a set of gaurds to prevent overflows our out of 
// bounds errors. Useful for debugging 
#define CUDA_ASSERT_NEG(var, error) error_check(var < 0.0f, __FILE__, __LINE__, error, var);
#define CUDA_ASSERT_POS(var, error) error_check(var > 0.0f, __FILE__, __LINE__, error, var);
#define CUDA_ASSERT_POS_E(var, error) error_check(var >= 0.0f, __FILE__, __LINE__, error, var);
#define CUDA_ASSERT_GT(var1, var2, error) error_check(var1 >= var2, __FILE__, __LINE__, error, var1, var2);
#define CUDA_ASSERT_LT_E(var1, var2, error) error_check(var1 <= var2, __FILE__, __LINE__, error, var1, var2);
#define CUDA_ASSERT_GT_LINE(var1, var2, error, line,file) error_check(var1 >= var2, file, line, error, var1, var2, true);

#define array_index(row,col,info) array_index_macro(row,col,info,__LINE__,__FILE__)

__device__ 
__forceinline__
void error_check(bool condition, const char* file, int line, const char* error, float var, float var2=0, bool debug=false){
    if(!condition){
        if(!debug)
            printf("CUDA ERROR: %s\n\tGot %f (%f)\n", error, var, var2);
        else
            printf("CUDA ERROR at %s:%d:\n\t%s\n\tGot %f (%f)\n", file, line, error, var, var2);
        asm ("exit;");   // inline ptx assembly to cause an illegal instruction
    }
}

