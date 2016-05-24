#ifndef EDGE_HPP
#define EDGE_HPP

#include "Actor.hpp"
#include <vector>

using namespace std;

class Edge {
	int weight;
	int movieName;
	int year;
	vector<Actor *> listOfActors;
};
#endif
