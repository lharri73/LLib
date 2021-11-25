#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <map>
#include <vector>

typedef struct hostpair {
    std::string host;
    std::string name;
} hostpair;


int main(int argc, char** argv){

    if(argc == 1){
        fprintf(stderr, "usage: %s host {ping options}\n", argv[0]);
        exit(1);
    }

    const char* user = std::getenv("USER");
    std::string user2 = user;
#ifdef __APPLE__
    std::string ssh_path = std::string("/Users/") + user + std::string("/.ssh/config");
#else
    std::string ssh_path = std::string("/home/") + user + std::string("/.ssh/config");
#endif

    std::ifstream fin(ssh_path.c_str());
    if(!fin.is_open()){
        fprintf(stderr, "could not open ssh config at: '%s'\n", ssh_path.c_str());
        exit(1);
    }


    std::string tmp;
    std::map<std::string,std::string> names;
    hostpair cur_pair;
    while(fin >> tmp){
        if(strcasecmp(tmp.c_str(), "HOST") == 0){
            fin >> cur_pair.host;
        }else if(strcasecmp(tmp.c_str(), "HOSTNAME") == 0){
            fin >> cur_pair.name;
            names.insert(std::make_pair(cur_pair.host, cur_pair.name));
        }

    }
    fin.close();

    std::map<std::string,std::string>::iterator it;

    it = names.find(argv[1]);
    if(it == names.end()){
        fprintf(stderr, "%s does not exist in %s\n", argv[1], ssh_path.c_str());
        exit(1);
    }


    std::vector<char*> new_argv;
    new_argv.push_back("ping");

    // what memory leak? just go buy more memory...it's 2021!
    char* garbage = (char*)malloc(it->second.size()*sizeof(char));
    memcpy(garbage, it->second.c_str(), it->second.size()+1);
    new_argv.push_back(garbage);
    for(int i=2; i < argc; i++){
        new_argv.push_back(argv[i]);
    }
    new_argv.push_back(nullptr);

    execvp("ping", new_argv.data());
    perror("ping did not execute");

    // rely on the operating system to clean up memory leak..
    // if ping executed properly
    return 0;
}
