/*
 * Edge.cpp
 * Author: Kyle Sit, Louis Leung
 * Date:6/2/16
 *
 * This file is contains implementation of edges used in ActorGraph
 */

#include "Edge.hpp"
#include <vector>
#include <string>

using namespace std;

//Constructor
Edge::Edge(string movieName, int year) {
  this->movieName = movieName;
  this->weight = 1 + (2015 - year);
  this->year = year;
}

//Overloaded operator for minHeap
bool Edge::operator ==(const Edge& other) {
  if (this->movieName == other.movieName && this->year == other.year)
    return 1;
  else
    return 0;
}

