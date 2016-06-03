
/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

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

