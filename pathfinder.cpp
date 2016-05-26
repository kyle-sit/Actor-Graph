/*
 * Filename: pathfinder.cpp
 * Name: Kyle Sit, Louis Leung
 * Date: 5/23/16
 * Description: Implements the executable of our Six Degrees Program.
 */

#include "ActorGraph.hpp"
#include "Edge.hpp"
#include "Actor.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	//Check for Arguments
	if(argc != 5){
		cout << "Invalid number of arguments.\n" 
			<< "Usage: ./pathfinder <movie cast text file>" 
      << "< -u/w specifying unweighted/weighted edges>" 
      << "<text file containing pairs to find.>"
      << "<text file for output>\n";
		return -1;
	}

  //Check second argument first to avoid opening files
  bool weight = false;
  if( *argv[2] == 'u' ) {
    weight = false;
  }
  else if( *argv[2] == 'w' ) {
    weight = true;
  }
  else {
    cout << "Invalid second argument";
    return -1;
  }
  /*switch( argv[2] ) {
    case 'u' : weight = false;
                break;
    case 'w' : weight = true;
                break;
    default: cout << "Invalid second argument";
             return -1;
  }*/

  ActorGraph myGraph;
  bool loaded = myGraph.loadFromFile(argv[1], weight);

  if( !loaded ) {
    cout << "Failed to load graph" << "\n";
    return -1;
  }

  bool search = false;
  if( !weight ) {
     search = BreadthFirstSearch(argv[3], argv[4]); 
  }
  else {
  //djikstras
  }

  std::vector<Actor*>::iterator it;
  for(it = myGraph.actors.begin(); it != myGraph.actors.end(); ++it) {
    cout << (*it)->actorName << "\n";
  }

  std::vector<Edge*>::iterator it2;
  for(it2 = myGraph.movies.begin(); it2 != myGraph.movies.end(); ++it2) {
    cout << (*it2)->movieName << "\n";
  }
  
  //Print out unordered map stuff
  for ( auto it = myGraph.connections.begin(); it != myGraph.connections.end(); ++it) {
    cerr << "Edge Movie Title Name: " << it->first->movieName << endl;
    cerr << "Actors Are: ";
    for (auto actorIt = it->second.begin(); actorIt != it->second.end(); ++actorIt) {
      cerr << endl << (*actorIt)->actorName << endl;
    }
  }

  return 1;
}
