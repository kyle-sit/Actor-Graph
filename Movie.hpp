/*
 * Movie.hpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/16
 *
 * This file contains the design of the movie class used in ufind
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

//forward declaration
class ActorNode;

//constructor
class Movie {
  public:
    string movieName;
    int year;
    
    Movie(string movieName, int year); 

    unordered_map<string, ActorNode*> actorNList;
};

#endif
