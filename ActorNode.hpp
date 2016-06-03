
/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class ActorNode {
  public:
    string actorName;
    ActorNode* parent;
    
    ActorNode(string actorName);
};
#endif 

