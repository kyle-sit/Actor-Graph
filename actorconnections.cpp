#include "ActorGraph.hpp"
#include "Edge.hpp"
#include "Actor.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <queue>

#define HEADER "Actor1\tActor2\tYear"
using namespace std;

//int strcmp(const char * str1, const char * str2);




long ActorGraph::buildGraph(std::ifstream & infile, 
                              std::priority_queue <Edge*,std::vector<Edge*>,EdgePtrCmp> & moviePQ) {
  infile.clear();
  infile.seekg(0, ios::beg);
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
    string movie_name(record[1] + record[2]);
    string movie(record[1]);
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

    //if this is a new movie, push it back onto our priority queue
    //and we need each edge that's in the priority queue to know about 
    //it's ACTORS


    auto eit = Econnections.find(movie_name);
    if (eit == Econnections.end() ) {
      Econnections[movie_name] = new Edge(movie, movie_year);
      moviePQ.push(Econnections[movie_name]);
    }
    (Econnections[movie_name])->actorList[actor_name] = Aconnections[actor_name];

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
    std::priority_queue<Edge*,std::vector<Edge*>,EdgePtrCmp> moviePQ;


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
     //clears all the edge maps from all the actors 
      for( auto clear = myGraph.Aconnections.begin(); clear != myGraph.Aconnections.end(); ++clear ) {
        clear->second->movieList.clear();
      }
      //empties out the priority queue
      while (!moviePQ.empty()) {
        moviePQ.pop();
      }
      //clears our universal hash map of edges
      myGraph.Econnections.clear();
      
      //takes our cast file, populates a field of actor nodes, fills the
      //priority queue with populated edge nodes
      int beginYear = myGraph.buildGraph(castfile, moviePQ);

      while (true) {
      
        if (moviePQ.empty()) {
          outfile << "9999" << endl;
          outfile << "movie not found " ;
          break;
        }
        auto shouldWeAdd = moviePQ.top();
        //check out the top year on our PQ, if it matches the beginYear, we can
        //add that movie
        while (shouldWeAdd->year == beginYear) {
          myGraph.addMovie(shouldWeAdd);
          moviePQ.pop();
          shouldWeAdd = moviePQ.top();
          if (moviePQ.empty()) {
            break;
          }
        }

        bool weConnected = 0;
       // cerr << "BFSing with added year: " << beginYear << endl;
        weConnected = myGraph.BreadthFirstSearchRes(actor_one, actor_two);

        if (weConnected == 1) {
          outfile << actor_one << "\t";
          outfile << actor_two << "\t";
          outfile << beginYear << endl;
          break;
        }
        //ran through all years, still failed
        if (beginYear == 2015) {
          outfile << "9999" << endl;
          break;
        
        }
        beginYear++;
      }
    }
  }
  //UNION FIND CASE 
  else {
    
  }

  return 1;
}









      
