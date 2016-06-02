#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "Edge.cpp"
#include "Actor.cpp"
#include "Actor.hpp"
#include "Edge.hpp"
#include <sstream>
#include "ActorGraph.hpp"

#define HYPHENS "--"
#define FINAL_ARROW "-->"
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define Max 1000000

using namespace std;

ActorGraph::ActorGraph() {}

bool ActorGraph::loadFromFile(const char* file_name, bool use_weighted_edges) {
    
    //Two vectors used
    //std::vector<Actor*> actors;
    //std::vector<Edge*> movies;
    
    ifstream infile(file_name);    

    //Check for header
    bool have_header = false;
 
    //Keep position in while loop
    //std::ios_base::seekdir pos = infile.beg; 

    // keep reading lines until the end of file is reached
    while (infile) {
        //infile.seekg(0, pos);
      
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        //save position
        //pos = infile.cur;
        
        if (!have_header) {
