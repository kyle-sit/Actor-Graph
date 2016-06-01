#include "Actor.hpp"
#include <string>
#include <vector>

using namespace std;

/* Overloaded operator for DictTrieNode */
bool Actor::operator<(const Actor& other) const {
  if( distance != other.distance ) {
    return distance > other.distance;
  }
  return false;
}
