/*
 * Edge.cpp
 * Author: Kyle Sit, Louis Leung
 * Date:6/2/16
 *
 * This file is contains design of edges used in ActorGraph
 */

#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

//forward declaration
class Actor;

//Class definition
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
