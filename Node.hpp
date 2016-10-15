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
    Node(unsigned long int toa, bool countable);
    void setDegree(int newDegree);
    void resolve(unsigned long int timeStep);
    void removeEdge(Node* adr);
    void printDegree();
    void printNeighbors();
    void letGoOffNeighbours();
    void addNeighbor(Node* newNeighbor);
    unsigned long int getTimeOfArrival();
    Node* getNeighbour(int i);
    int getDegree();
    int degree;
    int getDelay();
    bool getDecoded();
    void setDecoded();
    int getNumNeighbours();
    bool getCountable();
    void setCountable();
private:
    bool decoded_;
    bool countable_;
    int delay_;
    unsigned long int timeOfArrival_;
    std::vector<Node*> neighbours_;
};

#endif /* Node_h */
