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
  
  //create graph
  ActorGraph myGraph;
  bool loaded = myGraph.loadFromFile(argv[1], weight);

  if( !loaded ) {
    cout << "Failed to load graph" << "\n";
    return -1;
  }
  
  //Call BFS or Dijkstra
  bool search = false;
  if( !weight ) {
     search = myGraph.BreadthFirstSearch(argv[3], argv[4]); 
  }
  else {
    search = myGraph.DijkstraSearch(argv[3], argv[4]);
  }

  if( !search ) {
    cout << "Failed to search for pairs" << "\n";
    return -1;
  }
  
  return 0;
}
