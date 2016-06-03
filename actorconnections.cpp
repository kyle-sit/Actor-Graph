#include "ActorGraph.hpp"
#include "Edge.hpp"
#include "Actor.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>

#define HEADER "Actor1  Actor2  Year"
using namespace std;

//int strcmp(const char * str1, const char * str2);

long ActorGraph::findMinYearCreateActorNodes(std::ifstream & infile) {
  //Check for header
  bool have_header = false;
  int minYear = 10000;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    int movie_year = stoi(record[2]);

    if (movie_year < minYear) {
      minYear = movie_year;
    }



    //iterator for the map find function
    std::unordered_map<string,Actor*>::iterator ait;
    ait = Aconnections.find(actor_name);
    //Actor doesn't exist in map
    if( ait == Aconnections.end() ) {
      Aconnections[actor_name] = new Actor(actor_name);

    }
  }
  return minYear;
}


int main(int argc, char* argv[])
{
	//Check for Arguments
	if(argc != 5){
		cout << "Invalid number of arguments.\n" 
			<< "Usage: ./actorconnections <movie cast text file>" 
      << "<pairs to search for actor connections>"
      << "<output file>"
      << "<bfs or ufind>";
		return -1;
	}

  //Check second argument first to avoid opening files
  bool BFS = true;
  if( strcmp(argv[4],"bfs") == 0) {
  	BFS = true;
  }
  else if( strcmp(argv[4],"ufind") == 0) {
    BFS = false;
  }
  else {
    cout << "Invalid second argument";
    return -1;
  }
  ifstream castfile(argv[1]);
  ifstream pairsfile(argv[2]);
  ofstream outfile(argv[3]);

  outfile << HEADER << endl;

  //here we're going to parse every line of the pairs file

  if (BFS == true) {
    //Check for header
    bool have_header = false;

    // keep reading lines until the end of file is reached

    ActorGraph myGraph;
    int beginYear = myGraph.findMinYearCreateActorNodes(castfile);
    bool weConnected = 0;

    while (pairsfile) {
      string s;

      // get the next line
      if (!getline( pairsfile, s )) break;

      if (!have_header) {
        // skip the header
        have_header = true;
        continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss) {
        string next;

        // get the next string before hitting a tab character and put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
      }

      if (record.size() != 2) {
        // we should have exactly 3 columns
        continue;
      }

      string actor_one(record[0]);
      string actor_two(record[1]);
      
      cerr << "Waiting for edges to clear: " << endl;
      for( auto clear = myGraph.Aconnections.begin(); clear != myGraph.Aconnections.end(); clear++ ) {
        clear->second->movieList.clear();
      }
      cerr << "Cleared all actors edges " << endl;
      myGraph.Econnections.clear();

      int newYear = beginYear;

      while (true) {
        cerr << "Waiting to load the new movies " << endl;
        (void) myGraph.loadFromFileRes(castfile, newYear);
        cerr << "Loaded new movies " << endl;

       // cerr << "BFSing with added year: " << beginYear << endl;
        cerr << "Waiting to BFS " << endl;
        weConnected = myGraph.BreadthFirstSearchRes(actor_one, actor_two);
        cerr << "Finished a BFS " << endl;

        if (weConnected == 1) {
          outfile << actor_one << "\t";
          outfile << actor_two << "\t";
          outfile << newYear << endl;
          break;
        }
        //ran through all years, still failed
        if (newYear == 2015) {
          outfile << "9999" << endl;
          break;
        }
        newYear++;
      }
    }
  }
  //UNION FIND CASE 
  else {
    
  }

  return 1;
}







      
