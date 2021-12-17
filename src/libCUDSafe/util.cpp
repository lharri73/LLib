#include "libCUDSafe/cudaUtils.hpp"
#include <sstream>
/* All cuda functions return a cudaError_t, this function makes sure that it is
 * success. If not, it throws a runtime error. This is usefull for debugging 
 * rather than getting a segmentation fault */
void checkCudaError(cudaError_t error){
    if(error != cudaSuccess){
        std::stringstream ss;
        ss << "Internal error...\n";
        ss << "\tCUDA: " << cudaGetErrorString(error);
        throw std::runtime_error(ss.str());
    }
}

void safeCudaFree_macro(void *ptr, int line, const char* file){
    if(ptr != nullptr){
        cudaError_t error = cudaFree(ptr);
        if(error != cudaSuccess){
            std::stringstream ss;
            ss << "gaussMap:: Internal error during cudaFree at " << file << ":" << line << '\n';
            ss << "\tCUDA: " << error << '\n';
            throw std::runtime_error(ss.str());
        }
    }
}

