#ifndef EDGE_HPP
#define EDGE_HPP

#include "Actor.hpp"
#include <vector>
#include <string>

using namespace std;

class Edge {
  public:
    int weight;
	  string movieName;
	  int year;
    std::vector<Actor*> listOfActors;

    Edge(string movieName) {
      this.movieName = movieName;
      weight = 0;
      year = 0;
    }
};
#endif
