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
