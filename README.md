# Landon's Library of Things
This is a small collection of tools that I've written to make life easy. 
Mosltly, just to solve small problems or perform silly little tasks that are 
otherwise tedious. 

## Installation
This requires cmake...and that's it. 

1. Run `cmake .`
2. Add the `bin` folder to `PATH` 
3. Add `lib` to `LD_LIBRARY_PATH`

## Libraries and Programs
- `sping`
    - Search through an ssh config and ping the hostname of the provided host
- `libMOA`
    - Sometimes the `rand` library isn't all that random...This provides a more
      random number generator that generates various random types. 
    - Originally sourced from `http://www.agner.org/random/`

- `calc`
    - a command line calculator. Honors order of operations and parenthetical expressions. 
- `mvdl`
    - Moves the most recently modified file in your downloads folder to the currnet
      location of the shell. Takes no arguments. That is all. 
- `libCUDSafe`
    - Provides a protected interface to the CUDA C++ host and device calls.
    - Implements the following Host functions (on failure, each prints the CUDA error string and 
      which file and line the error occurred in.):
        - `safeCudaMalloc`: allocate memory and throws runtime error on failure
        - `safeCudaMemcpy2Device`: copy host memory to device memory. 
        - `safeCudaMemcpy2Host`: copy device memory back to host. ""
        - `safeCudaMemset`: Memset, but for device memory. 
        - `safeCudaFree`: Like Free, but throws a runtime error
    - Implements the following Device functions (on failure, each prints the value to test
      against, and the value provided). The assertions
      insert a bad assembly instruction to halt execution within the device kernel so you don't
      get thousands of lines of garbage from a single error. 
        - `CUDA_ASSERT_NEG`: raise an exception if a value is not negative
        - `CUDA_ASSERT_NEG_E`: raise an exception if a value is not negative or equal to 0
        - `CUDA_ASSERT_POS`: raise an exception if a value is not positive
        - `CUDA_ASSERT_POS_E`: raise an exception if a value is not positive or equal to 0
        - `CUDA_ASSERT_GT`: raise an exception if the test value is not greater than set value
        - `CUDA_ASSERT_GT_E`: raise an exception if the test value is not greater or equal to the set value
        - `CUDA_ASSERT_LT`: raise an exception if the test value is not less than set value
        - `CUDA_ASSERT_LT_E`: raise an exception if the test value is not less or equal to the set value

## TODO
    - random word generator
    - random number generator (program)
    - python array to matlab (and back)
    - remove recent download (rmdl)
    - https://github.com/cgag/loc
