#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

int main(int argc, char** argv){

    struct stat statDat;
    stat("/Users/landon/Downloads", &statDat);
    std::cout << statDat << '\n';
    return 0;
}
