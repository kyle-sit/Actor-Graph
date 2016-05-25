#include "Edge.hpp"
#include <vector>
#include <string>

using namespace std;

Edge::Edge(string movieName, int year) {
  this->movieName = movieName;
  this->weight = 1 + (2015 - year);
  this->year = year;
}

bool Edge::operator ==(const Edge& other) {
  if (this->movieName == other.movieName && this->year == other.year)
    return 1;
  else
    return 0;
}

