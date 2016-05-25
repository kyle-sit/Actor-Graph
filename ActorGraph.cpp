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
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(ifstream infile, bool use_weighted_edges) {
    
    //Check for header
    bool have_header = false;
 
    //Keep position in while loop
    int pos = infile.beg;

    // keep reading lines until the end of file is reached
    while (infile) {
        infile.seekg(0, pos);
      
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        //save position
        pos = infile.tellg();
        
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
            
        bool exists = false;
            
        //is the movie we just processed already in our vector of movies?
        for (std::vector<Edge*>::iterator it = movies.begin() ; it != movies.end(); ++it) {
          if( currMovie == *(*it) ) {
            exists = true;
          }
        }

        //movie is a unique movie
        if( !exists ){
          Edge* newMovie = new Edge(movie_title, movie_year);
          movies.push_back(newMovie);
          
          //if our actor isn't in our set of strings, add it into our set
          //and create a new Actor object from it and add to set of Actor *s
          exists = false;
          for (std::vector<Actor*>::iterator it = actors.begin() ; it != actors.end(); ++it) {
            if( actor_name == (*it)->actorName ) {
              exists = true;
            }
          }
          
          //Our actor has not been found so we appropriately create its node   
          if( !exists ) {
            Actor* newActor = new Actor( actor_name );
            actors.push_back( newActor );
            newMovie.listOfActors.push_back(newActor);
            newActor.edges.push_back(newMovie);
          }

          while (infile) {
            string s;
 
            // get the next line
            if (!getline( infile, s )) break;
  
            istringstream ss( s );
            vector <string> record;
  
            while (ss) {
              string next;
      
              // get the next string before hitting a tab character and put it in 'next'
              if (!getline( ss, next, '\t' )) break;
                record.push_back( next );
              }
  
              if (record.size() != 3) {
                //we should have exactly 3 columns
                continue;
              }
 
              string actorName(record[0]);
              string movieTitle(record[1]);
              int movieYear = stoi(record[2]);
          
              if( movie_title == movieTitle && movie_year == movieYear) {
                bool alsoExists = false;
                for (std::vector<Actor*>::iterator it = actors.begin() ; it != actors.end(); ++it) {
                  if( actorName == (*it)->actorName ) {
                    alsoExists = true;
                  }
                }
             
                if( !alsoExists ) {
                  Actor* newActor = new Actor( actorName );
                  actors.push_back( newActor );
                  newMovie.listOfActors.push_back(newActor);
                  newActor.edges.push_back(newMovie);
                }
              }
          }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
