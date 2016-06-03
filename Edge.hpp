
/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Actor;

class Edge {
  public:
    string movieName;
    int weight;
    int year;
    
    Edge(string movieName, int year); 
    bool operator ==(const Edge& other);

    unordered_map<string, Actor*> actorList;
};

#endif
