/* Names: Louis Leung, Kyle Sit
 * Description: This module prints out the average distance of one actor to any other actor after
 * BFSing the entire cast file
*/


#include "ActorGraph.hpp"
#include <iostream>
#include <fstream>

int main (int argc, char *argv[]) {
	float avgDist = 0;
	float totalDist = 0;
	int numActors = 0;
	//incorrect number of args
	if (argc != 4) {
		cerr << "Usage: <program> <casts_file> <actorLast, actorFirst>" << endl;
		cerr << "Note: You can only use this on actors with a strict first and last name, no additions" << endl;
		return -1;
	}
	
	//load the graph from our movie casts
	ActorGraph myGraph;
	myGraph.loadFromFile(argv[1], true);
	//construct a valid name according to how we stored info
	string actorLastName = argv[2];
	string actorFirstName = argv[3];
	string actorName = actorLastName + " " + actorFirstName;
	//prevent segfaults
	if (myGraph.Aconnections.find(actorName) == myGraph.Aconnections.end()) {
		cerr << actorName << endl;
		cerr << "Actor doesn't exist, type in this format LASTNAME, FIRSTNAME" << endl;
		return -1;
	}
	//BFS with that actor as root
	myGraph.BFSAvg(actorName); 

	//go through all the actors, add up their distances, keep a tally of num
	//actors
	for (auto actorIt = myGraph.Aconnections.begin(); actorIt != myGraph.Aconnections.end(); ++actorIt) {
		totalDist = totalDist + actorIt->second->distance;
		if (actorIt->second->distance != 0) {
			numActors++;
		}
	}
	//calculate average, print it out 
	avgDist = totalDist / numActors;
	cout << endl << "Average Distance between this actor and any other actor is: " << avgDist << endl;
}
	
