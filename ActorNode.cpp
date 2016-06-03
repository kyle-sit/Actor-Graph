#include "ActorNode.hpp"
#include <string>
#include <vector>

using namespace std;

//Constructor
ActorNode::ActorNode(string actorName){
  this->actorName = actorName;
  this->parent = nullptr;
}
