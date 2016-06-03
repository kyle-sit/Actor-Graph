#include "Actor.hpp"
#include <string>
#include <vector>

using namespace std;

//Constructor
Actor::Actor(string actorName){
  visited = false;
  distance = 0;
  this->actorName = actorName;
  this->prevActor = nullptr;
}

/* Overloaded operator for DictTrieNode */
bool Actor::operator<(const Actor& other) const {
  if( distance != other.distance ) {
    return distance > other.distance;
  }
  return false;
}
