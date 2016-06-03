/*
 * Actor.cpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/2016
 *
 * This file holds implementation of the Actors in the graph
 */

#include "Actor.hpp"
#include <string>
#include <vector>

using namespace std;

//Constructor for the Actor
Actor::Actor(string actorName){
  visited = false;
  distance = 0;
  this->actorName = actorName;
  this->prevActor = nullptr;
}

// Overloaded operator for Actor
bool Actor::operator<(const Actor& other) const {
  if( distance != other.distance ) {
    return distance > other.distance;
  }
  return false;
}
