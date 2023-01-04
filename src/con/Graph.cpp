#include <sys/types.h>
#include <pwd.h>
// #include <uuid/uuid.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <deque>

#include "con/Graph.hpp"

Graph::Graph(){
    // read in the units.txt file and populate the graph
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;
    std::stringstream config;
    config << homedir;
    config << "/.config/con/units.txt";
    std::ifstream fin;
    fin.open(config.str().c_str(), std::fstream::in);
    
    if(!fin.is_open()){
        perror("couldn't open config file");
        exit(1);
    }

    std::stringstream buf;
    std::string tmpStr, unitFrom, multiplier, plussMin, offset, unitTo;
    Unit *from_unit, *to_unit;
    double offset_d;
    while(std::getline(fin, tmpStr)){
        buf.str("");
        buf.clear();
        buf << tmpStr;
        unitFrom = "";
        multiplier = "";
        plussMin = "";
        offset = "";
        unitTo = "";


        buf >> unitFrom >> multiplier >> plussMin >> offset >> unitTo;
        if(unitFrom == "") continue; // empty line
        if(offset == ""){
            unitTo = plussMin;
            offset_d = 0;
        }else{
            offset_d = -1;
        }
        
        if(unitFrom[0] == '#') continue; // comment

        std::map<std::string,Unit*>::const_iterator from;
        std::map<std::string,Unit*>::const_iterator to;
        from = unit_map.find(unitFrom);
        if(from == unit_map.end()){
            from_unit = new Unit();
            from_unit->name = unitFrom;
            units.push_back(from_unit);
            unit_map[unitFrom] = from_unit;
        }else{
            from_unit = from->second;
        }

        if(multiplier == "-"){
            // add this value as an alias to `to`
            continue;
        }

        to = unit_map.find(unitTo);
        if(to == unit_map.end()){
            to_unit = new Unit();
            to_unit->name = unitTo;
            units.push_back(to_unit);
            unit_map[unitTo] = to_unit;
        }else{
            to_unit = to->second;
        }
        

        // reuse the buf to parse string to double...not assuming c++11
        buf.clear();
        buf.str("");
        buf << multiplier;
        double mult_d;
        buf >> mult_d;

        if(offset_d == -1){
            buf.clear();
            buf.str("");
            buf << offset;
            buf >> offset_d;
            if(plussMin == "-")
                offset_d *= -1;
            else if (plussMin == "+");
            else{
                fprintf(stderr, "invalid operator (%s) in offset of %s -> %s\n", plussMin.c_str(), unitFrom.c_str(), unitTo.c_str());
                exit(1);
            }
        }

        from_unit->edges.push_back(Edge(mult_d,offset_d,false,from_unit,to_unit));
        to_unit->edges.push_back(Edge(mult_d,offset_d,true,to_unit,from_unit));
    }
        
    fin.close();
    if(units.size() == 0){
        fprintf(stderr, "no units in units.txt! Exiting\n");
        exit(2);
    }
}

Graph::~Graph(){
    for(size_t i =0; i < units.size(); i++){
        delete units[i];
    }
}

template <typename T, typename V>
typename std::multimap<T,V*>::iterator find_in_mm(std::multimap<T,V*> &map, const V* val){
    typename std::multimap<T,V*>::iterator it;
    it = map.begin();
    for(it = map.begin(); it != map.end(); it++){
        if(it->second == val){
            return it;
        }
    }
    return it;
}

int Graph::dijkstras(std::string from_str, std::string to_str){
    std::map<std::string,Unit*>::const_iterator from;
    std::map<std::string,Unit*>::const_iterator to;

    from = unit_map.find(from_str);
    if(from == unit_map.end()){
        fprintf(stderr, "'%s' is not a valid unit in units.txt. Don't know how to convert.\n", from_str.c_str());
        return 2;
    }
    to = unit_map.find(to_str);
    if(to == unit_map.end()){
        fprintf(stderr, "'%s' is not a valid unit in units.txt. Don't know how to convert.\n", to_str.c_str());
        return 3;
    }

    std::multimap<int,Unit*> front;
    std::multimap<int,Unit*>::iterator current_map, tmp_it;
    Unit *current, *n2;

    front.insert(std::make_pair(0, from->second));
    from->second->distance = 0;

    double test_dist;
    while(front.size() != 0){
        current_map = front.begin();
        current = current_map->second;
        front.erase(current_map);
        current->visited = true;
        
        for(size_t i = 0; i < current->edges.size(); i++){
            n2 = current->edges.at(i).to;
            test_dist = current->distance + 1;

            if((n2->distance == -1) || (test_dist < n2->distance)){
                tmp_it = find_in_mm(front, n2);

                // remove this node from the multimap if it was on it already
                if(tmp_it != front.end())
                    front.erase(tmp_it);

                n2->distance = test_dist;
                n2->backedge = current->edges.at(i);
                n2->backedge.invert();
                front.insert(std::make_pair(test_dist,n2));
            }
        }
    }

    end_unit = to;
    return to->second->backedge.valid ? 1 : 0;
}

double Graph::get_value(double input){
    double orig_input = input;
    std::stringstream path;
    
    std::vector<std::string> units;
    std::deque<Unit*> unit_path;
    
    Unit *current = end_unit->second;

    while(current->backedge.valid){
        unit_path.push_front(current);
        current = current->backedge.to;
    }

    for(size_t i = 0; i < unit_path.size(); i++){
        current = unit_path.at(i);
        path.str("");
        path.clear();

        if(!current->backedge.inverse){
            if(current->backedge.offset == 0){
                path << " -> (" << input << " 1/" << current->backedge.multiplier;
                input /= current->backedge.multiplier;
                path << ") = " << input;
            }else if(current->backedge.offset < 0){
                path << " -> ((" << input << " + " << -1*current->backedge.offset << ") x 1/" << current->backedge.multiplier;
                input -= current->backedge.offset;
                input /= current->backedge.multiplier;
                path << ") = " << input;
            }else{
                path << " -> ((" << input << " - " << current->backedge.offset << ") x 1/" << current->backedge.multiplier;
                input -= current->backedge.offset;
                input /= current->backedge.multiplier;
                path << ") = " << input;
            }
        }else{
            path << " -> (" << input << " x " << current->backedge.multiplier;
            input *= current->backedge.multiplier;

            if(current->backedge.offset == 0){
                path << ") = " << input;
            }else if(current->backedge.offset < 0){
                input += current->backedge.offset;
                path << " - " << -1*current->backedge.offset << ") = " << input;
            }else{
                input += current->backedge.offset;
                path << " + " << current->backedge.offset << ") = " << input;
            }
        } 


        path << current->name;
        units.push_back(path.str());
    }
    
    std::cout << orig_input;
    for(size_t i = 0; i < units.size(); i++){
        printf("%s", units.at(i).c_str());
    }
    putchar('\n');
    return input;

}

void Graph::reset_visited(){
    for(size_t i = 0; i < units.size(); i++){
        units.at(i)->visited = false;
    }
}

void visit_nodes(std::vector<Unit> &units, Unit *cur){
    units.push_back(*cur);
    cur->visited = true;
    for(size_t i = 0; i < cur->edges.size(); i++){
        if(cur->edges.at(i).to->visited) continue;
        visit_nodes(units, cur->edges.at(i).to);
    }
}

std::vector<Unit> Graph::list_all(std::string from_unit){
    reset_visited();
    std::vector<Unit> ret;
    std::map<std::string,Unit*>::iterator it;
    it = unit_map.find(from_unit);
    Unit *cur = (it->second);
    visit_nodes(ret, cur);
    return ret;
}

Unit::Unit(){
    distance = -1;
}

/*Unit::~Unit(){
    for(size_t i = 0; i < edges.size(); i++){
        delete edges[i];
    }
}*/


Edge::Edge(double mul, double off, bool inv, Unit* From, Unit* To){
    multiplier = mul;
    offset = off;
    inverse = inv;
    to = To;
    from = From;
    valid = true;
}
Edge::Edge(){
    multiplier = 0;
    offset = 0;
    valid = false;
    inverse = false;
    to = nullptr;
    from = nullptr;
}
void Edge::invert(){
    Unit *tmp;
    tmp = from;
    from = to;
    to = tmp;
    inverse = !inverse;
}
