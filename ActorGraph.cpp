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

using namespace std;


static bool retraceActor(Actor * root, Actor * last, const char * outfilename);
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
  
    //Check for header
    bool have_header = false;
 
    //Keep position in while loop
    //std::ios_base::seekdir pos = infile.beg; 

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

        queue<Actor*> explore;
        std::vector<Actor*>::iterator it;
        //Set all nodes distance to 0 and visited to false
        for(it = actors.begin(); it != actors.end(); ++it) {
          //(*it)->visited = false;
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

        Actor* last;
        for(auto secondIt = actors.begin(); secondIt != actors.end(); ++secondIt) {
          if( actor_two == (*secondIt)->actorName ) {
            last  = *secondIt;
            break;
          }
        } 
        /*
        explore.push(root);
        while( !explore.empty() ) {
          Actor* next = explore.front();
          explore.pop();
          std::vector<Edge*>::iterator edgeIt;
          for(edgeIt = (next->edges).begin(); edgeIt != (next->edges).end(); ++edgeIt) {
            for(it = (connections[*edgeIt]).begin(); it != (connections[*edgeIt]).end(); it++) {
              if( next->distance + 1 < (*it)->distance ) {
                (*it)->distance = next->distance + 1;
                (*it)->prevActor = next;
                (*it)->prevMovie = *edgeIt;
                explore.push(*it);
              }
            }
          }
        }
       */
       //it = explore[1];
       //

        explore.push(root);
        while( !explore.empty() ) {
          Actor* next = explore.front();
          explore.pop();
          //FIX: when you pop you have to visit
          next->visited = true;
          std::vector<Edge*>::iterator edgeIt;
          for(edgeIt = (next->edges).begin(); edgeIt != (next->edges).end(); ++edgeIt) {
            for(it = (connections[*edgeIt]).begin(); it != (connections[*edgeIt]).end(); it++) {
       //FIX: Was never pushing exploring any nodes because if condition to
       //enter loop was never true. changed so the distance check is only
       //performed when we know this is the node's second time getting updated      
              if( (*it)->visited == false) {
                if ((*it)->distance != 0) {
                  if ((next->distance + 1) >= (*it)->distance) {
                    continue;
                  }
                }
                (*it)->distance = next->distance + 1;
                (*it)->prevActor = next;
                (*it)->prevMovie = *edgeIt;
                explore.push(*it);
              }
            }
         }
       }
       bool pathFound = retraceActor(root, last, out_file);
       if (!pathFound) {
        return false;
       }
    }
    return true;
}

static bool retraceActor(Actor * root, Actor * last, const char * outfilename) {

  ofstream outfile(outfilename);

  outfile << HEADER << endl;

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
    tempM = tempA->prevMovie;
    movieYear = "";
    movieYear = to_string(tempM->year);
    moviePath.push_back(movieYear);
    moviePath.push_back(tempM->movieName);

    tempA = tempA->prevActor;
  }

  if( tempA != root ) {
    outfile << "9999" << "\n";
    return 0;
  }
  else {
    auto rActorIt = actorPath.rbegin();
    auto rMovieIt = moviePath.rbegin();
    for (int i = 1;(unsigned int) i <=  actorPath.size(); i++) {
        outfile << "(" << *rActorIt << ")";
        rActorIt++;
   
        outfile  << HYPHENS;
        //print out the movie title
         outfile << "[" << *rMovieIt;
         outfile << "#@";

         rMovieIt++;
         //print out the year
         outfile << *rMovieIt << "]";
         rMovieIt++;
         
         //last node, need special print then break
        if ((unsigned int)i + 1 == actorPath.size()) {
          outfile << FINAL_ARROW;
          outfile << "(" << *rActorIt << ")" << endl;
          return 1;
         }
         outfile  << HYPHENS;
    }
  }
  return 1;
}


