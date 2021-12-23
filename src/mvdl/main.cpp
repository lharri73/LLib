#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <string.h>
#include <vector>

#include "dlLib/dlLib.hpp"

int main(int argc, char** argv){

    std::multimap<time_t, std::string> files = LLib::parse_downloads();

    std::multimap<time_t,std::string>::const_reverse_iterator it = files.crbegin(); // the first element in the reversed map (last element in the non-reversed map)

    std::vector<char*> new_argv;
    
    // did someone say memory leak?
    char* tmp = (char*)malloc(sizeof(char) * it->second.size());
    memcpy(tmp, it->second.c_str(), it->second.size()+1);

    new_argv.push_back("mv");
    new_argv.push_back(tmp);

    std::string dst;
    if(argc == 1){
        dst = ".";
    }else if(argc == 2){
        dst = argv[1];
    }else{
        fprintf(stderr, "usage: %s [destination]\n", argv[0]);
        fprintf(stderr, "Moves the most recently downloaded file to the provided destination -or- current location.");
        exit(2);
    }

    char* tmp2 = (char*)malloc(sizeof(char) * dst.size());
    memcpy(tmp2, dst.c_str(), dst.size()+1);
    new_argv.push_back(tmp2);
    new_argv.push_back(nullptr);

    printf("mv %s %s\n", tmp, tmp2);
    execvp("mv", new_argv.data());
    perror("mv did not execute:");

    return 0;
}
