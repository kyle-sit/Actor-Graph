
/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include "ActorNode.hpp"
#include <string>
#include <vector>

using namespace std;

//Constructor
ActorNode::ActorNode(string actorName){
  this->actorName = actorName;
  this->parent = nullptr;
}
