/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <sstream>
#include "ActorGraph.hpp"
#include "Actor.cpp"
#include "Edge.cpp"

#define HYPHENS "--"
#define FINAL_ARROW "-->"
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define Max 1000000

using namespace std;

ActorGraph::ActorGraph() {}

bool ActorGraph::loadFromFile(const char* file_name, bool use_weighted_edges) {


  ifstream infile(file_name);    

  //Check for header
  bool have_header = false;

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
    string movie_title(record[1] + record[2]);
    string movie(record[1]);
    int movie_year = stoi(record[2]);

    //iterator for the map find function
    std::unordered_map<string,Actor*>::iterator ait;
    std::unordered_map<string,Edge*>::iterator eit;

    ait = Aconnections.find(actor_name);
    //Actor doesn't exist in map
    if( ait == Aconnections.end() ) {
      Aconnections[actor_name] = new Actor(actor_name);
    }

    eit = Econnections.find(movie_title);
    //Movie does not exist in actor nodes map
    if( eit == Econnections.end() ) {
      Econnections[movie_title] = new Edge(movie, movie_year);
    }

    (Aconnections[actor_name])->movieList[movie_title] = Econnections[movie_title];
    (Econnections[movie_title])->actorList[actor_name] = Aconnections[actor_name];


  }

  if (!infile.eof()) {
    cerr << "Failed to read " << infile << "!\n";
    return false;
  }
  infile.close();

  return true;

}

bool ActorGraph::BreadthFirstSearch(const char* pairs_file, const char* out_file) {
  ifstream infile(pairs_file);
  ofstream outfile(out_file);

  outfile << HEADER << endl;

  //Check for header
  bool have_header = false;

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

    if (record.size() != 2) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_one(record[0]);
    string actor_two(record[1]);


    //Find the nodes we want the path for
    Actor* root = Aconnections[actor_one];
    Actor* last = Aconnections[actor_two];

    auto ait = Aconnections.begin();
    for ( ; ait!= Aconnections.end(); ++ait ){
      (ait->second)->visited = false;
    }

    //FIFO queue for BFS
    queue<Actor*> explore;

    //BFS alg start
    Actor* temp = root;
    explore.push(temp);

    while( !explore.empty() ) {
      temp = explore.front();
      explore.pop();
      temp->visited = true;

      std::unordered_map<string,Edge*>::iterator eit;
      //for the current node, go to all of its edges
      for(eit = (temp->movieList).begin(); eit != (temp->movieList).end(); ++eit) {
        //cerr << eit->second->movieName <<endl;
        //for each of these edges, check to see if the actors its connected
        //to have been visited, if not, update them
        for(ait = (eit->second)->actorList.begin(); ait != (eit->second)->actorList.end(); ait++) {
          //cerr <<  ait->second->actorName << endl;
          if( (ait->second)->visited == false) {
            (ait->second)->visited = true;
            (ait->second)->distance = temp->distance + 1;
            (ait->second)->prevActor = temp;
            (ait->second)->prevMovie = eit->second;
            explore.push(ait->second);
          }
        }
      }
    }
    bool pathFound = retraceActor(root, last, outfile);
    if (!pathFound) {
      return false;
    }
  }
  return true;
}

bool ActorGraph::retraceActor(Actor * root, Actor * last, std::ofstream& outfile) {



  std::vector<string> actorPath;
  std::vector<string> moviePath;

  Actor* tempA = last;
  Edge* tempM;
  string movieYear;

  //FIX: was never pushing root node onto actor list, and the roots prevMovie was
  //getting pushed when it didn't have one
  while (tempA) {
    actorPath.push_back(tempA->actorName);
    if (tempA == root) {
      break;
    }

    if( tempA->prevMovie ) {
      tempM = tempA->prevMovie;
      movieYear = "";
      movieYear = to_string(tempM->year);
      moviePath.push_back(movieYear);
      moviePath.push_back(tempM->movieName);
      tempA = tempA->prevActor;
    }
    else {
      outfile << "9999";
      return -1;
    }
  }

  auto rActorIt = actorPath.rbegin();
  auto rMovieIt = moviePath.rbegin();
  for (int i = 1;(unsigned int) i <=  actorPath.size(); i++) {
    if (i == 1) { 
      outfile << "(" << *rActorIt << ")";
    }
    else {
      outfile << FINAL_ARROW;
      outfile << "(" << *rActorIt << ")";
    }
    rActorIt++;

    if ((unsigned int)i == actorPath.size()) {
      break;
    }

    outfile  << HYPHENS;
    //print out the movie title
    outfile << "[" << *rMovieIt;
    outfile << "#@";

    rMovieIt++;
    //print out the year
    outfile << *rMovieIt << "]";
    rMovieIt++;
  }
  outfile << endl;

  return 1;
}


bool ActorGraph::DijkstraSearch(const char* pairs_file, const char* out_file) {
  ifstream infile(pairs_file);
  ofstream outfile(out_file);
  outfile << HEADER << endl;

  //Check for header
  bool have_header = false;

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

    if (record.size() != 2) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_one(record[0]);
    string actor_two(record[1]);

    //First step of dijkstras setting 
    std::unordered_map<string,Actor*>::iterator ait;
    //Set all nodes distance to 0 and visited to false
    for(ait = Aconnections.begin(); ait != Aconnections.end(); ++ait) {
      (ait->second)->visited = false;
      (ait->second)->distance = Max;
      (ait->second)->prevActor = nullptr;
      (ait->second)->prevMovie = nullptr;
    }

    //Find the nodes we want the path for
    Actor* root = Aconnections[actor_one];
    Actor* last = Aconnections[actor_two];

    //Begin Dijkstra's
    //Root's distance is ZERO
    std::priority_queue<Actor*, std::vector<Actor*>, ActorPtrComp> minHeap;
    Actor* temp = root;
    root -> distance = 0;
    minHeap.push(temp);


    while( !minHeap.empty() ) {
      temp = minHeap.top();
      minHeap.pop();

      //if this node hasn't been visited yet 
      if( !(temp->visited) ) {
        temp->visited = true;

        std::unordered_map<string,Edge*>::iterator eit;
        //for the current node, go to all of its edges
        for(eit = (temp->movieList).begin(); eit != (temp->movieList).end(); ++eit) {
          //cerr << eit->second->movieName <<endl;
          //for each of these edges, check to see if the actors its connected
          //to have been visited, if not, update them
          for(ait = (eit->second)->actorList.begin(); ait != (eit->second)->actorList.end(); ait++) {
            int c = temp->distance + (eit->second)->weight;
            /*cerr << "Attempting to set: " << (ait->second)->actorName << endl;
              cerr << "The attempted new distance is " << c << endl;
              cerr << "Only insert if its less than: " << (ait->second)->distance;*/
            //If the node we're at has a better potential distance, OR it
            //hasn't even been visited yet, set it's fields and then push it
            //onto the PQ
            if( c < (ait->second)->distance ) {
              //cerr << "Setting: " << (ait->second)->actorName << "PrevMovie is: " << (eit->second)->movieName << endl;
              (ait->second)->distance = c; 
              (ait->second)->prevActor = temp;
              (ait->second)->prevMovie = eit->second;
              minHeap.push(ait->second);
            }
          }
        }
      }
    }
    //since all the nodes fields are filled now, retrace the path so
    //we can print to outfile
    bool pathFound = retraceActor(root, last, outfile);
    if (!pathFound) {
      return false;
    }
  }
  return true; 
}

bool ActorGraph::addMovie(Edge * newMovie) {

    string movie_name(newMovie->movieName + to_string(newMovie->year));

    for (auto ait = newMovie->actorList.begin(); ait != newMovie->actorList.end(); ++ait) {
        (Aconnections[ait->second->actorName])->movieList[movie_name] = Econnections[movie_name];
      }

  return true;
}

bool ActorGraph::BreadthFirstSearchRes(string actor_one, string actor_two) {


  //Find the nodes we want the path for
  Actor* root = Aconnections[actor_one];
  
  auto ait = Aconnections.begin();
  for ( ; ait!= Aconnections.end(); ++ait ){
    (ait->second)->visited = false;
  }
  

  //FIFO queue for BFS
  queue<Actor*> explore;

  //BFS alg start
  Actor* temp = root;
  explore.push(temp);
  
  while( !explore.empty() ) {
    temp = explore.front();
    explore.pop();
    temp->visited = true;

    std::unordered_map<string,Edge*>::iterator eit;
    //for the current node, go to all of its edges
    for(eit = (temp->movieList).begin(); eit != (temp->movieList).end(); ++eit) {
      //for each of these edges, check to see if the actors its connected
      //to have been visited, if not, update them
        for(ait = (eit->second)->actorList.begin(); ait != (eit->second)->actorList.end(); ait++) {
          //if( (ait->second)->distance > temp->distance + 1 || (ait->second)->distance == 0) {
            //if we've reached the second actor, return immediately
          if( (ait->second)->visited == false ) {
            
            if ((ait->second)->actorName == actor_two) {
              return 1;
            }


            (ait->second)->visited = true;
            (ait->second)->distance = temp->distance + 1;
            (ait->second)->prevActor = temp;
            (ait->second)->prevMovie = eit->second;
            explore.push(ait->second);
            
          }
        }
      }
    }
  //after populating data fields of hash maps, has the actor we're looking for
  //been touched? if so, return 1, if not return 0
  return 0;
}

bool ActorGraph::BFSAvg(string desiredActor) {

    //Find the nodes we want the path for
    Actor* root = Aconnections[desiredActor];

    auto ait = Aconnections.begin();
    for ( ; ait!= Aconnections.end(); ++ait ){
      (ait->second)->visited = false;
    }

    //FIFO queue for BFS
    queue<Actor*> explore;

    //BFS alg start
    Actor* temp = root;
    explore.push(temp);

    while( !explore.empty() ) {
      temp = explore.front();
      explore.pop();
      temp->visited = true;

      std::unordered_map<string,Edge*>::iterator eit;
      //for the current node, go to all of its edges
      for(eit = (temp->movieList).begin(); eit != (temp->movieList).end(); ++eit) {
        //for each of these edges, check to see if the actors its connected
        //to have been visited, if not, update them
        for(ait = (eit->second)->actorList.begin(); ait != (eit->second)->actorList.end(); ait++) {
          //cerr <<  ait->second->actorName << endl;
          if( (ait->second)->visited == false) {
            (ait->second)->visited = true;
            (ait->second)->distance = temp->distance + 1;
            (ait->second)->prevActor = temp;
            (ait->second)->prevMovie = eit->second;
            explore.push(ait->second);
          }
        }
      }
    }
  return true;
}


