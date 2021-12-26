#include <vector>
#include <map>

class Unit;
class Edge{
    public:
        Edge(double mul, double off, bool inv, Unit *from, Unit *To); 
        Edge();
        
        void invert();

        double multiplier;
        double offset;
        bool inverse;
        Unit *to;
        Unit *from;
        bool valid;
};

class Unit{
    public:
        Unit();
        //~Unit();
        std::string name;

        Edge backedge;
        bool visited;
        int distance;
        std::vector<Edge> edges;
};

class Graph{
    public:
        Graph();
        ~Graph();

        bool dijkstras(std::string from, std::string to);
        double get_value(double input);

    private:
        std::vector<Unit*> units;
        std::map<std::string,Unit*> unit_map;

        std::map<std::string,Unit*>::const_iterator end_unit;
};

