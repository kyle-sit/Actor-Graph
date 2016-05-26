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
#include "ActorGraph.hpp"

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
        cout << "Enter while loop" << "\n";
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
          cout << "Enter second while loop" << "\n";  
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
    
        cout << "Starting our algorithm" << "\n";

        // we have an actor/movie relationship, now what?
        Edge currMovie = Edge(movie_title, movie_year);
      
        cout << "iterate through edges" << "\n";
        std::vector<Edge*>::iterator it;
        //is the movie we just processed already in our vector of movies?
        for (it = movies.begin(); it != movies.end(); ++it) {
          if( currMovie == *(*it) ) {
            break;
          }
        }

        cerr << "Post for loop";

        //Two cases if movie is new and if it is not new
        if( it == movies.end() ) {
          cout << "unique movie found" << "\n";
          
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
            cout << "unique actor found" << "\n";
            Actor* newActor = new Actor(actor_name);
            actors.push_back(newActor);
            connections[newMovie].push_back(newActor);
            (newActor->edges).push_back(newMovie);
          }
          //Actor is not new
          else {
            cout << "actor is not unique" << "\n";
            connections[newMovie].push_back(*it2);
            ((*it2)->edges).push_back(newMovie);
          }
        }
        //movie is not new
        else{
          cout << "movie is not new" << "\n";
          //Check if associated actor is new
          std::vector<Actor*>::iterator it2;
          for(it2 = actors.begin(); it2 != actors.end(); ++it2) {
            if( actor_name == (*it2)->actorName ) {
              break;
            }
          }

          //Two cases if actor is new and if it is not new
          if( it2 == actors.end() ) {
            cout << "unique actor found" << "\n";
            Actor* newActor = new Actor(actor_name);
            actors.push_back(newActor);
            connections[*it].push_back(newActor);
            (newActor->edges).push_back(*it);
          }
          //Actor is not new
          else {
            cout << "actor is not unique" << "\n";
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
  
    //Check for header
    bool have_header = false;
 
    //Keep position in while loop
    //std::ios_base::seekdir pos = infile.beg; 

    // keep reading lines until the end of file is reached
    while (infile) {
        cout << "Enter pairs loop" << "\n";
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
          cout << "Enter second pairs loop" << "\n";  
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

        explore.push(root);
        while( !explore.empty() ) {
          Actor* next = explore.front();
          exlpore.pop();

        }
    }
}


