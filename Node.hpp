//
//  Node.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 27/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Node_hpp
#define GUARD_Node_hpp
#include <stdio.h>
#include <vector>
class Node {
public:
    Node();
    ~Node();
    Node(unsigned long int toa);
    void setDegree(int newDegree);
    void resolve(Node* adr,unsigned long int time_step);
    void remove_edge(Node* adr);
    void printDegree();
    void printNeighbors();
    void letGoOffNeighbours(Node* adr);
    void letGoOffEdge(Node* adr);
    void addNeighbor(Node* newNeighbor);
    unsigned long int getTimeOfArrival();
    Node* getNeighbour(int i);
    int getDegree();
    int degree;
    int getDelay();
    bool getDecoded();
    void setDecoded();
    int getNumNeighbours();
    
private:
    bool decoded;
    int delay;
    unsigned long int timeOfArrival;
    std::vector<Node*> neighbours;
};

#endif /* Node_h */
