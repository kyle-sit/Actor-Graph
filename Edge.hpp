#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include <string>

using namespace std;

class Edge {
  public:
    int weight;
	  string movieName;
	  int year;

    Edge(string movieName, int year); 

    bool operator ==(const Edge& other);
};
#endif
