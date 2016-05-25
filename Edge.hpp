#ifndef EDGE_HPP
#define EDGE_HPP

#include "Actor.hpp"
#include <vector>
#include <string>

using namespace std;

class Edge {
	int weight;
	string movieName;
	int year;
  std::vector<Actor*> listOfActors;
};
#endif
