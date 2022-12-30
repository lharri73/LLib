#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <string.h>
#include <sys/types.h>

#include "dlLib/dlLib.hpp"

int main(int argc, char** argv){
    fprintf(stderr, "not implemented --Landon\n");

#if 0
    std::string cmd = "xclip -sel clip";
    if(strncmp(argv[1],"--cmd",5)==0){
        if(strnlen(argv[1],7) > 5){
            fprintf(stderr, "'--cmd' argument should be in the form of \"--cmd '[cmd]'\" got '%s'\n", argv[1]);
            exit(1);
        }
        cmd = argv[2];
        argv+=2;
        argc-=2;
    }

    if(argc == 1){
        std::multimap<time_t, std::string> files = LLib::parse_downloads();
        auto it = files.crbegin();
    }else{

    }
#endif
    return 1;
}
