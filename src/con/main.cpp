#include <cstdio>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

struct Unit{
    std::string name;
    std::vector<std::pair<Unit*,double> > edges;
    bool visited;
};

class Graph{
    public:
        Graph();
        ~Graph();

        void dijkstras(std::pair<int,int> start, std::pair<int,int> end);

    private:
        std::vector<Unit*> map;
        std::map<std::string,Unit*> units;
};

double convert(std::string in_unit, std::string to_unit, double input){
    // find index of to and from unit
    // find path between them
    return 0;

}

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
    stream_in >> from_num;
    stream_in >> from_unit;
    stream_in >> to_unit;

    printf("converting from %f (%s) to (%s)\n", from_num, from_unit.c_str(), to_unit.c_str());
    return 0;
}
