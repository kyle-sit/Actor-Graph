/*
 * Disjoint.hpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/16
 *
 * This file contains the design of the container for our ufind operations. 
 * It holds the ActorNodes and Movies to be organized in disjoint sets
 * and then unioned.
 */


#ifndef DISJOINTSET_HPP
#define DISJOINTSET_HPP

#include "ActorNode.hpp"
#include "Movie.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class DisjointSet {
public:
    //Costructor
    DisjointSet(void);
  
    //Destructor
    ~DisjointSet();

    //member variables
    unordered_map<string, ActorNode*> DJactors;
    unordered_map<int, unordered_map<string,Movie*>> yearList;

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 
     * + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    int loadSets(std::ifstream& infile);
   
    //union method
    bool actorUnion(int year);
    
    //find method
    bool find(ActorNode* first, ActorNode* second);

    //more specific find of root
    ActorNode* ufind(ActorNode* temp, int& count);

};
#endif
