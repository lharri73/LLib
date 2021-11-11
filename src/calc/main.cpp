#include "calc/solve.h"
#include <cstdio>
#include <sstream>
#include <iostream>

int main(int argc, char** argv){
    std::stringstream stin("");
    std::string tmp;
    if(argc == 1){
        while(std::cin >> tmp)
            stin << tmp;
    }else{
        for(int i = 1; i < argc; i++){
            stin << argv[i];
        }
    }

    tmp = stin.str();
    float ret = calc::solve(tmp.c_str(), 0, tmp.size());

    if((int)ret == ret)
        printf("%d\n", (int)ret);
    else
        printf("%f\n", ret);

    return 0;
}
