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

	//Open second file 
	ifstream pairs;
	pairs.open(argv[3], ios::binary);

	//Check if input file was actually opened
	if(!pairs.is_open()) 
	{
    cout<< "Invalid first input file. No file was opened. Please try again.\n";
		return -1;
	}
	
  //Open output file 
	ofstream out;
	out.open(argv[4], ios::binary);

	//Check if input file was actually opened
	if(!out.is_open()) 
	{
    pairs.close();
    cout<< "Invalid first input file. No file was opened. Please try again.\n";
		return -1;
	}

	//Check for empty file
	pairs.seekg(0, ios_base::end); 
	unsigned int len2 = pairs.tellg();
	if(len2==0) 
	{
    pairs.close();
    out.close();
    cout << "The second input file is empty. \n";
		return -1;
	}
	
  //Resets the stream to beginning of file
  pairs.seekg(0, ios_base::beg);

  ActorGraph myGraph;
  bool loaded = myGraph.loadFromFile(argv[1], weight);

  
  std::vector<Actor*>::iterator it;
  for(it = myGraph.actors.begin(); it != myGraph.actors.end(); ++it) {
    cout << (*it)->actorName << "\n";
  }

  std::vector<Edge*>::iterator it2;
  for(it2 = myGraph.movies.begin(); it2 != myGraph.movies.end(); ++it2) {
    cout << (*it2)->movieName << "\n";
  }
  
  //load failed
  if( !loaded ) {
    pairs.close();
    out.close();
    return -1;
  }

  //Close files and return
  pairs.close();
  out.close();
  return 1;
}
