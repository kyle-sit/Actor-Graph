/*
 * DisjointSet.cpp
 * Author: Kyle Sit,  Louis Leung
 * Date:6/2/16
 *
 * This file implements the container we use as the basis for ufind.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "DisjointSet.hpp"
#include "ActorNode.cpp"
#include "Movie.cpp"

#define HYPHENS "--"
#define FINAL_ARROW "-->"
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define Max 1000000

using namespace std;

//Default constructor
DisjointSet::DisjointSet() {}

/*
 * loadSets takes our cast file and creates a bunch of disjoint sets of
 * actors based on their movie and year.  These disjoint sets will later
 * be unioned.
 */
int DisjointSet::loadSets(std::ifstream& infile) {
    //Check for header
    bool have_header = false;
    int smallestYear = 1000000;
 
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

        //Data components
        string actor_name(record[0]);
        string movie_title(record[1] + record[2]);
        string movie(record[1]);
        int movie_year = stoi(record[2]);

        //Obtain the smallest year
        if( movie_year < smallestYear ) {
          smallestYear = movie_year;
        }

        //iterator for the map find function
        std::unordered_map<string,ActorNode*>::iterator ait;
        std::unordered_map<string,Movie*>::iterator mit;

        //Allocate new actors
        ait = DJactors.find(actor_name);
        if( ait == DJactors.end() ) {
          DJactors[actor_name] = new ActorNode(actor_name);
        }

        //Allocate new movies
        mit = (yearList[movie_year]).find(movie_title);
        if( mit == (yearList[movie_year]).end() ) {
          (yearList[movie_year])[movie_title] = new Movie(movie, movie_year);
        }

        //Connect actors in certain movies in certain years
        ((yearList[movie_year])[movie_title])->actorNList[actor_name] = DJactors[actor_name];
    }
    return smallestYear;
}

/*
 * Union method that joins all actors casted in eacg novie that is 
 * specified within certain years.
 */
bool DisjointSet::actorUnion(int year) {
        //Check to see if the year actually contains movies
        std::unordered_map<int,unordered_map<string,Movie*>>::iterator yit;
        yit = yearList.find(year);
       
        if( yit == yearList.end() ) {
          return false;
        }
 
        //Iterate through the actor cast of certain movies in the year
        std::unordered_map<string,ActorNode*>::iterator ait;
        std::unordered_map<string,Movie*>::iterator mit;
        int count1 = 0;
        int count2 = 0;
        //for each movie in the year
        for( mit = yearList[year].begin(); mit != yearList[year].end(); ++mit ) {
          ait = (mit->second)->actorNList.begin();
          //Get the preexisting root
          ActorNode* root1 = ufind(ait->second, count1);
          ActorNode* root2;
          ++ait;
          //for each actor in the movie
          for( ; ait != (mit->second)->actorNList.end(); ++ait ) {
            //get the preexisting root of the actor
            root2 = ufind((ait->second), count2);
            //Connect smaller root to bigger root
            if( count1 > count2 ) {
              root2->parent = root1;
            }
            else {
              root1->parent = root2;
              root1 = root2;
            }
          }
        }

        return true;
}

/*
 * find method used to check if the roots of two actornodes
 * are the same and if so we have a connection
 */
bool DisjointSet::find(ActorNode* first, ActorNode* second) {
  ActorNode* temp = first;
  ActorNode* temp2 = second;

  //Get the root
  while( temp->parent ) {
    if( temp == temp->parent ) {
      break;
    }
    temp = temp->parent;
  }

  //Get the root
  while( temp2->parent ) {
    if( temp2 == temp2->parent ) {
      break;
    }
    temp2 = temp2->parent;
  }

  //compare the roots
  if( temp == temp2 ) {
    return true;
  }

  return false;
}

/*
 * A more specific find that just gets the root
 */
ActorNode* DisjointSet::ufind(ActorNode* temp, int& count) {
  count = 0;
  while( temp->parent ) {
    if( temp == temp->parent ) {
      break;
    }
    temp = temp->parent;
    count++;
  }
  return temp;
}

