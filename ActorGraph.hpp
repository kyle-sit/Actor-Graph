/*
 * ActorGraph.hpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/16
 *
 * This file contains the design of our ActorGraph container
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

/* 
 * This class is a comparator class used to create a minHeap 
 */
class EdgePtrCmp {
  public:
    bool operator()(Edge* lhs, Edge* rhs) const {
      return (lhs->year > rhs->year);
    }
};

//Class definition
class ActorGraph {
public:
    //Constructor
    ActorGraph(void);
    //Member Variables
    unordered_map<string, Actor*> Aconnections;
    unordered_map<string, Edge*> Econnections;

    /*
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 
     * 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* file_name, bool use_weighted_edges);
    
    //BFS that searches through all edges and actors
    bool BreadthFirstSearch(const char* pairs_file, const char* out_file);
    
    //Searches through all edges and actors but modifies acccording to weight
    bool DijkstraSearch(const char* pairs_file, const char* out_file);

    //Print method
    bool retraceActor(Actor * root, Actor * last, std::ofstream& outfile);

    //Used in actorconnections to add a movie edge
    bool addMovie(Edge * movieAdded);

    //Second BFS used to search edges year by year
    bool BreadthFirstSearchRes(string actor_one, string actor_two);

    //Builds the edges year by year
    long buildGraph(std::ifstream & infile, 
         std::priority_queue <Edge*,std::vector<Edge*>,EdgePtrCmp> & moviePQ);
};

#endif // ACTORGRAPH_HPP
