/*
 * ActorGraph.hpp
 * Author: Kyle Sit, Louis Leung
 * Date:6/2/16
 *
 * This file is contains design of actor node used in ufind
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

//Default constructor
class ActorNode {
  public:
    string actorName;
    ActorNode* parent;
    
    ActorNode(string actorName);
};
#endif 

