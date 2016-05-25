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

    Edge(string movieName, int year) {
      this.movieName = movieName;
      this. weight = 0;
      this.year = year;
    }

    bool Edge::operator ==(const Edge& other) {
      if (this.movieName == other.movieName && this.year == other.year)
        return 1;
      else
        return 0;
    }
};
#endif
