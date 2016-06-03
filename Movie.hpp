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
