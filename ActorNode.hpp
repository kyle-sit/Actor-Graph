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

