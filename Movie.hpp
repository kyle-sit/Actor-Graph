
/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class ActorNode;

class Movie {
  public:
    string movieName;
    int year;
    
    Movie(string movieName, int year); 

    unordered_map<string, ActorNode*> actorNList;
};

#endif
