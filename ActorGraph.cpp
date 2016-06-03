/*
 * ActorGraph.cpp
 * Author: Kyle Sit, Louis Leung
 * Date:   6/2/16
 *
 * This file contains the implemention of our actual graph and acts as a 
 * container for all our actors and edges
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

//Default Constuctor
ActorGraph::ActorGraph() {}

//Destructor
ActorGraph::~ActorGraph(){
    std::unordered_map<string,Actor*>::iterator ait;
    std::unordered_map<string,Edge*>::iterator eit;

    for( ait = Aconnections.begin(); ait != Aconnections.end(); ++ait ) {
      delete ait->second;
    }

    for( eit = Econnections.begin(); eit != Econnections.end(); ++eit ) {
      delete eit->second;
    }
}

/*
 * loadFromFile method parses our input file and creates our graph using
 * two unordered_maps.  One map for actors and one map for edges.  These maps
 * are connected in a circular fashion so that all actors and edges are 
 * appriopriately connected.
 */
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

      //get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    //All of our data components
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

    //connecting actors to movie edges
    (Aconnections[actor_name])->movieList[movie_title] 
                                              = Econnections[movie_title];
    //connecting the edges to the corresponding actor
    (Econnections[movie_title])->actorList[actor_name] 
                                              = Aconnections[actor_name];
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << infile << "!\n";
    return false;
  }
  infile.close();

  return true;
}

/*
 * BreadFirstSearch is a method implementing a bfs by iterating through all
 * actors and edges setting the distances and previous actor nodes.  The 
 * visited check allows us to ignore nodes that we havea already seen.  
 * Eventually we will have explored all nodes in the map.
 */
bool ActorGraph::BreadthFirstSearch(const char* pairs_file, 
                                           const char* out_file) {
  //open the streams
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

      //get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 2) {
      // we should have exactly 3 columns
      continue;
    }

    //Our data components
    string actor_one(record[0]);
    string actor_two(record[1]);

    //Find the nodes we want the path for
    Actor* root = Aconnections[actor_one];
    Actor* last = Aconnections[actor_two];

    //Must set all of the nodes to false before new search
    auto ait = Aconnections.begin();
    for ( ; ait!= Aconnections.end(); ++ait ){
      (ait->second)->visited = false;
    }

    //FIFO queue for BFS
    queue<Actor*> explore;

    //BFS alg start
    Actor* temp = root;
    explore.push(temp);

    //Go until the queue is empty
    while( !explore.empty() ) {
      temp = explore.front();
      explore.pop();
      temp->visited = true;

      std::unordered_map<string,Edge*>::iterator eit;
      //for the current node, go to all of its edges
      for(eit = (temp->movieList).begin(); eit != (temp->movieList).end(); 
                                                                    ++eit) {
        //Go through all actors of the current edge
        for(ait = (eit->second)->actorList.begin(); 
                                ait != (eit->second)->actorList.end(); ait++) {
          //If the actor has not been seen then set its fields appropriately
          if( (ait->second)->visited == false) {
            (ait->second)->visited = true;
            (ait->second)->distance = temp->distance + 1;
            (ait->second)->prevActor = temp;
            (ait->second)->prevMovie = eit->second;
            explore.push(ait->second);
            if ((ait->second)->actorName == actor_two) {
              break;
            }
          }
        }
      }
    }
    //Call our print method
    bool pathFound = retraceActor(root, last, outfile);
    if (!pathFound) {
      return false;
    }
  }
  return true;
}

/*
 * retraceActor is a print method that traverses from the find node to the root
 * node stashing all actors and movies to be printed.  Once we have those all
 * stored in vectors we use those to print in reverse.
 */
bool ActorGraph::retraceActor(Actor * root, Actor * last, 
                                                    std::ofstream& outfile) {
  //vectors to store
  std::vector<string> actorPath;
  std::vector<string> moviePath;

  //initialize temps needed
  Actor* tempA = last;
  Edge* tempM;
  string movieYear;

  //While we have a previous actor to record
  while (tempA) {
    //insert new actor onto path
    actorPath.push_back(tempA->actorName);
    if (tempA == root) {
      break;
    }

    //insert new movie and year onto path
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

  //Make reverse iterators to go through vectors to print
  auto rActorIt = actorPath.rbegin();
  auto rMovieIt = moviePath.rbegin();
  for (int i = 1;(unsigned int) i <=  actorPath.size(); i++) {
    //print Actor
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

    //Print movie and year
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

/*
 * This method implements a DijkstraSearch which uses movie years as the basis
 * for weights.  We essentially use the universal algorithm with slight
 * tweak to meet our needs.
 */
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

      //get the next string before hitting a tab character and put it in 'next'
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

    //While our minHeap has more Nodes
    while( !minHeap.empty() ) {
      temp = minHeap.top();
      minHeap.pop();

      //if this node hasn't been visited yet 
      if( !(temp->visited) ) {
        temp->visited = true;

        std::unordered_map<string,Edge*>::iterator eit;
        //for the current node, go to all of its edges
        for(eit = (temp->movieList).begin(); eit != (temp->movieList).end();
                                                                       ++eit) {
          //for each of these edges, check to see if the actors its connected
          //to have been visited, if not, update them
          for(ait = (eit->second)->actorList.begin(); 
                                ait != (eit->second)->actorList.end(); ait++) {
            int c = temp->distance + (eit->second)->weight;
            //If the node we're at has a better potential distance, OR it
            //hasn't even been visited yet, set it's fields and then push it
            //onto the PQ
            if( c < (ait->second)->distance ) {
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

/*
 * This method is implemented in our BFS that finds the earliest year the two
 * actors are connected.  This takes a movie and connects it to the actors that
 * star in that same year
 */
bool ActorGraph::addMovie(Edge * newMovie) {
    //Specific movie + year for our unordered_map
    string movie_name(newMovie->movieName + to_string(newMovie->year));
    
    //Iterate through our map and set movies
    for (auto ait = newMovie->actorList.begin(); 
                                     ait != newMovie->actorList.end(); ++ait) {
        (Aconnections[ait->second->actorName])->movieList[movie_name] =
                                                      Econnections[movie_name];
      }

  return true;
}

/*
 * This is the actual BFS method that finds the earliest year the two actors
 * are connected.  We essentially wrote a very similar BFS to the first but
 * it has slight tweaks such as no outfile and different print methods
 * in order to increase runtime.  We also have different checks.
 */
bool ActorGraph::BreadthFirstSearchRes(string actor_one, string actor_two) {
  //Find the nodes we want the path for
  Actor* root = Aconnections[actor_one];
 
  //Set all nodes to false
  auto ait = Aconnections.begin();
  for ( ; ait!= Aconnections.end(); ++ait ){
    (ait->second)->visited = false;
  }
  
  //FIFO queue for BFS
  queue<Actor*> explore;

  //BFS alg start
  Actor* temp = root;
  explore.push(temp);
 
  //While our queue has nodes still in it
  while( !explore.empty() ) {
    temp = explore.front();
    explore.pop();
    temp->visited = true;

    std::unordered_map<string,Edge*>::iterator eit;
    //for the current node, go to all of its edges
    for(eit = (temp->movieList).begin(); eit != (temp->movieList).end();
                                                                    ++eit) {
      //for each of these edges, check to see if the actors its connected
      //to have been visited, if not, update them
        for(ait = (eit->second)->actorList.begin(); 
                                ait != (eit->second)->actorList.end(); ait++) {
          //If we havent seen the node then set its fields
          if( (ait->second)->visited == false ) {
            (ait->second)->visited = true;
            (ait->second)->distance = temp->distance + 1;
            (ait->second)->prevActor = temp;
            (ait->second)->prevMovie = eit->second;
            explore.push(ait->second);
            //Check to break from the loop if we've found our node
            if ((ait->second)->actorName == actor_two) {
              return 1;
            }
          }
        }
      }
    }
  return 0;
}

/*
 * This method builds the graph year by year for our second BFS.  We call BFS
 * in a loop that only has movies by year incrementally.  This build allows
 * us to build only actors and edges connected before a certain year.
 */
long ActorGraph::buildGraph(std::ifstream & infile, 
        std::priority_queue <Edge*,std::vector<Edge*>,EdgePtrCmp> & moviePQ) {
  //Make sure youre at the beginning of the file
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

      //get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    //Data components
    string actor_name(record[0]);
    string movie_name(record[1] + record[2]);
    string movie(record[1]);
    int movie_year = stoi(record[2]);

    //Acquiring the lowest year
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
    (Econnections[movie_name])->actorList[actor_name] =
                                                  Aconnections[actor_name];

  }
  return minYear;
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


