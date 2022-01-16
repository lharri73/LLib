#include <cstdio>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "con/Graph.hpp"


int main(int argc, char** argv){
    std::string input_str;
    std::stringstream stream_in;
    stream_in.str();
    if(argc == 1){
        while(std::getline(std::cin, input_str)){
            stream_in << input_str;
        }
    }else{
        for(int i=1; i < argc; i++){
            stream_in << argv[i] << ' ';
        }
    }

    double from_num;
    std::string from_unit;
    std::string to_unit;
    if(!(stream_in >> from_num)){
        fprintf(stderr, "cannot determine input number from '%s'\n", stream_in.str().c_str());
        exit(1);
    }
    if(!(stream_in >> from_unit)){
        fprintf(stderr, "cannot determine input unit from '%s'\n", stream_in.str().c_str());
        exit(1);
    }
    if(!(stream_in >> to_unit)){
        fprintf(stderr, "cannot determine output unit from '%s'\n", stream_in.str().c_str());
        exit(1);
    }


    Graph g;
    int found_path= g.dijkstras(from_unit, to_unit);
    if(found_path == 1){
        double conversion;
        conversion = g.get_value(from_num);
        std::cout << conversion << to_unit << '\n';
    }else if(found_path == 2){
        exit(1);
    }else{
        // input unit is good, output is garbage
        std::vector<Unit> units = g.list_all(from_unit);
        printf("valid units to convert from %s with:\n", from_unit.c_str());
        for(size_t i = 0; i < units.size(); i++){
            printf("- %s\n", units.at(i).name.c_str());
        }
        exit(1);
    }
    return 0;
}
