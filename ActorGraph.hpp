/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */


#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include "Actor.hpp"
#include "Edge.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

class EdgePtrCmp {
  public:
    bool operator()(Edge* lhs, Edge* rhs) const {
      return (lhs->year > rhs->year);
    }
};

class ActorGraph {
protected:
  
    // Maybe add class data structure(s) here

public:
    ActorGraph(void);
    unordered_map<string, Actor*> Aconnections;
    unordered_map<string, Edge*> Econnections;

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

	bool BFSAvg(string desiredActor);
    
    bool DijkstraSearch(const char* pairs_file, const char* out_file);

    bool retraceActor(Actor * root, Actor * last, std::ofstream& outfile);

    bool addMovie(Edge * movieAdded);

    bool BreadthFirstSearchRes(string actor_one, string actor_two);

    long buildGraph(std::ifstream & infile, 
                      std::priority_queue <Edge*,std::vector<Edge*>,EdgePtrCmp> & moviePQ);
};




#endif // ACTORGRAPH_HPP
