/*
 * ActorGraph.cpp
 * Author: Kyle Sit, Louis Leung
 * Date:6/2/16
 *
 * This file is contains implementation of actor node used in ufind
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
