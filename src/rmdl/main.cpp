#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <utility>
#include <string.h>
#include <vector>

#include "dlLib/dlLib.hpp"


int main(int argc, char** argv){

    if(argc != 1){
        fprintf(stderr, "usage: %s [destination]\n", argv[0]);
        fprintf(stderr, "Moves the most recently downloaded file to the provided destination -or- current location.");
        exit(2);
    }

    std::multimap<time_t,std::string> files = LLib::parse_downloads();

    std::multimap<time_t,std::string>::const_reverse_iterator it = files.crbegin(); // the first element in the reversed map (last element in the non-reversed map)

    std::vector<char*> new_argv;
    
    // don't have to free this because exec overwrites the heap
    char* tmp = (char*)malloc(sizeof(char) * it->second.size());
    memcpy(tmp, it->second.c_str(), it->second.size()+1);

    new_argv.push_back("rm");
    new_argv.push_back(tmp);
    new_argv.push_back(nullptr);

    printf("rm %s \n", tmp);
    execvp("rm", new_argv.data());
    perror("rm did not execute:");

    return 0;
}
