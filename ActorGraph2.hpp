#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include "Actor.hpp"
#include "Edge.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class ActorGraph {
protected:
  
    // Maybe add class data structure(s) here

public:
    ActorGraph(void);
    std::unordered_map<Edge*, std::vector<Actor*>> connections;
    std::vector<Actor*> actors;
    std::vector<Edge*> movies;
    //std::set<Actor*> actorSet;
    //std::set<Edge*> movieSet;

    // Maybe add some more methods here
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* file_name, bool use_weighted_edges);
    
    bool BreadthFirstSearch(const char* pairs_file, const char* out_file);
    
    bool DijkstraSearch(const char* pairs_file, const char* out_file);

    bool retraceActor(Actor * root, Actor * last, std::ofstream& outfile);
};


#endif // ACTORGRAPH_HPP
