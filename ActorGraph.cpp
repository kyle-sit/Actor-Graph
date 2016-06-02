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
#include "Edge.cpp"
#include "Actor.cpp"
#include "Actor.hpp"
#include "Edge.hpp"
#include <sstream>
#include "ActorGraph.hpp"

#define HYPHENS "--"
#define FINAL_ARROW "-->"
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define Max 1000000

using namespace std;

ActorGraph::ActorGraph() {}

bool ActorGraph::loadFromFile(const char* file_name, bool use_weighted_edges) {
    
    //Two vectors used
    //std::vector<Actor*> actors;
    //std::vector<Edge*> movies;
    
    ifstream infile(file_name);    

    //Check for header
    bool have_header = false;
 
    //Keep position in while loop
    //std::ios_base::seekdir pos = infile.beg; 

    // keep reading lines until the end of file is reached
    while (infile) {
        //infile.seekg(0, pos);
      
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        //save position
        //pos = infile.cur;
        
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
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    

        // we have an actor/movie relationship, now what?
        Edge currMovie = Edge(movie_title, movie_year);
      
      //FIX:: HASH MAP, does this movie string exist, if so, return a pointer
      //if it doesn't exist, just insert it
      //so you don't have to search through entire vector
      //SAME WITH ACTORS

        std::vector<Edge*>::iterator it;
        //is the movie we just processed already in our vector of movies?
        for (it = movies.begin(); it != movies.end(); ++it) {
          if( currMovie == *(*it) ) {
            break;
          }
        }


        //Two cases if movie is new and if it is not new
        if( it == movies.end() ) {
          
          //Allocate new edge and push it to global list
          Edge* newMovie = new Edge(movie_title, movie_year);
          movies.push_back(newMovie);
          
          //Check if associated actor is new
          std::vector<Actor*>::iterator it2;
          for(it2 = actors.begin(); it2 != actors.end(); ++it2) {
            if( actor_name == (*it2)->actorName ) {
              break;
            }
          }

          //Two cases if actor is new and if it is not new
          if( it2 == actors.end() ) {
            Actor* newActor = new Actor(actor_name);
            actors.push_back(newActor);
            connections[newMovie].push_back(newActor);
            (newActor->edges).push_back(newMovie);
          }
          //Actor is not new
          else {
            connections[newMovie].push_back(*it2);
            ((*it2)->edges).push_back(newMovie);
          }
        }
        //movie is not new
        else{
          //Check if associated actor is new
          std::vector<Actor*>::iterator it2;
          for(it2 = actors.begin(); it2 != actors.end(); ++it2) {
            if( actor_name == (*it2)->actorName ) {
              break;
            }
          }

          //Two cases if actor is new and if it is not new
          if( it2 == actors.end() ) {
            Actor* newActor = new Actor(actor_name);
            actors.push_back(newActor);
            connections[*it].push_back(newActor);
            (newActor->edges).push_back(*it);
          }
          //Actor is not new
          else {
            connections[*it].push_back(*it2);
            ((*it2)->edges).push_back(*it);
          }
        }
    }

    /*//Move pointers to set
    for(std::vector<Edge*>::iterator toSet = movies.begin(); toSet != movies.end(); ++toSet) {
      movieSet.insert(*toSet);
    }
    for(std::vector<Actor*>::iterator toSet2 = actors.begin(); toSet2 != actors.end(); ++toSet2) {
      actorSet.insert(*toSet2);
    }*/

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
        
        std::vector<Actor*>::iterator it;
        //Set all nodes distance to 0 and visited to false
        for(it = actors.begin(); it != actors.end(); ++it) {
          (*it)->visited = false;
          (*it)->distance = 0;
        }
        
        //find root node
        Actor* root;
        for(it = actors.begin(); it != actors.end(); ++it) {
          if( actor_one == (*it)->actorName ) {
            root = *it;
            break;
          }
        } 

        //find node to connect path
        Actor* last;
        for(auto secondIt = actors.begin(); secondIt != actors.end(); ++secondIt) {
          if( actor_two == (*secondIt)->actorName ) {
            last  = *secondIt;
            break;
          }
        } 

        //FIFO queue for BFS
        queue<Actor*> explore;
        
        //have a movie class 
        //check if we can also limit the double for loop stuff
        //construct Edges to and from in load from file 
        // move to laod from file cause we'll be calling BFS in a loop

        explore.push(root);
        while( !explore.empty() ) {
          Actor* next = explore.front();
          explore.pop();
          //FIX: when you pop you have to visit
          next->visited = true;
          std::vector<Edge*>::iterator edgeIt;
          //for the current node, go to all of its edges
          for(edgeIt = (next->edges).begin(); edgeIt != (next->edges).end(); ++edgeIt) {
            //for each of these edges, check to see if the actors its connected
            //to have been visited, if not, update them
            for(it = (connections[*edgeIt]).begin(); it != (connections[*edgeIt]).end(); it++) {
              if( (*it)->visited == false) {
                (*it)->visited = true;
                (*it)->distance = next->distance + 1;
                (*it)->prevActor = next;
                (*it)->prevMovie = *edgeIt;
                explore.push(*it);
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
      cerr << "Breaking because null previous movie" << endl;
      cerr << "The actor we're at is: " << tempA->actorName << endl;
      cerr << "The root is " << root->actorName << endl;
      cerr << "9999" << "\n";
      return -1;
    }
  }
  /*
  if( tempA != root ) {
    cerr << "
    outfile << "9999" << "\n";
    return -1;
  }
  */
  
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
        std::vector<Actor*>::iterator it;
        //Set all nodes distance to 0 and visited to false
        for(it = actors.begin(); it != actors.end(); ++it) {
          (*it)->visited = false;
          (*it)->distance = Max;
          (*it)->prevActor = nullptr;
          (*it)->prevMovie = nullptr;
        }

        //find root node
        Actor* root;
        for(it = actors.begin(); it != actors.end(); ++it) {
          if( actor_one == (*it)->actorName ) {
            root = *it;
            break;
          }
        } 

        //find node to connect path
        Actor* last;
        for(auto secondIt = actors.begin(); secondIt != actors.end(); ++secondIt) {
          if( actor_two == (*secondIt)->actorName ) {
            last  = *secondIt;
            break;
          }
        }

        /*//Begin dijkstra alg
        std::priority_queue<Edge*, std::vector<Edge*>, EdgePtrComp> minHeap;
        Edge* temp = (root->edges).front();
        minHeap.push(start);
        while( !minHeap.empty() ) {
          temp = minHeap.front();
          
        }*/

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
            
            std::vector<Edge*>::iterator edgeIt;
            //for each of the movies this actor is in 
            for(edgeIt = (temp->edges).begin(); edgeIt != (temp->edges).end(); ++edgeIt) {
              //visit each of those movie's actors, if the path is the shortest
              //it's seen for that actor, update actor's fields and push it 
              //onto our priority queue
              for(it = (connections[*edgeIt]).begin(); it != (connections[*edgeIt]).end(); it++) {
                int c = temp->distance + (*edgeIt)->weight;
                cerr << "Attempting to set: " << (*it)->actorName << endl;
                cerr << "The attempted new distance is " << c << endl;
                cerr << "Only insert if its less than: " << (*it)->distance;
                //If the node we're at has a better potential distance, OR it
                //hasn't even been visited yet, set it's fields and then push it
                //onto the PQ
                if( c < (*it)->distance || (*it)->distance == Max) {
                  cerr << "Setting: " << (*it)->actorName << "PrevMovie is: " << (*edgeIt)->movieName << endl;
                  (*it)->distance = c; 
                  (*it)->prevActor = temp;
                  (*it)->prevMovie = *edgeIt;
                  minHeap.push(*it);
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
