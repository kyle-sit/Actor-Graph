/*
 * DisjointSet.cpp
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
#include <sstream>
#include "DisjointSet.hpp"

#define HYPHENS "--"
#define FINAL_ARROW "-->"
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define Max 1000000

using namespace std;

DisjointSet::DisjointSet() {}

bool DisjointSet::loadSets(const char* file_name) {
  
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
        std::unordered_map<string,ActorNode*>::iterator ait;
        std::unordered_map<int,Movie*>::iterator yit;

        ait = DJactors.find(actor_name);
        if( ait == DJactors.end() ) {
          DJactor[actor_name] = new ActorNode(actor_name);
        }

        yit = (yearList[year]).find(movie_title);
        if( yit == (yearList[year]).end() ) {
          (yearList[year])[movie_title] = new Movie(movie, year);
        }

        ((yearList[year])[movie_title])->actorList[actor_name] = DJactors[actor_name];
}

bool actorUnion(int year) {
        std::unordered_map<int,Movie*>::iterator yit;
        yit = yearList.find(year);
        
        if( yit == yearList.end() ) {
          return false;
        }
 
        std::unordered_map<string,ActorNode*> ait;
        std::unordered_map<string,Movie*>::iterator mit;
        for( mit = yearList[year].begin(); mit != yearList[year].end(); ++mit ) {
          ait = (mit->second)->actorNList.begin();
          ActorNode* temp = ait->second;
          ++ait;
          for( ; ait != (mit->second)->actorNList.end(); ++ait ) {
            (ait->second)->parent = temp;
          }  
        }

        return true;
}


bool find(ActorNode* first, ActorNode* second) {
  ActorNode* temp = first;
  ActorNode* temp2 = second;

  while( temp->parent ) {
    temp = temp->parent;
  }

  while( temp2->parent ) {
    temp2 = temp2->parent;
  }

  if( temp == temp2 ) {
    return true;
  }

  return false;
}


