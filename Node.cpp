//
//  Node.cpp
//  CSA_Simulator
//  A class to represent the nodes in a graph, can be used for both the CN and VN in a bipartite graph.

//  Created by Erik Sandgren on 27/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//
#include "Node.hpp"
using namespace std;

Node::Node() {
    degree = 0;
    vector<Node*> tmp;
    neighbours_ = tmp;
    decoded_ = false;
    timeOfArrival_ = 0;
    delay_ = -1;
    countable_ = false;

}

Node::Node(unsigned long int toa) {
    degree = 0;
    timeOfArrival_ = toa;
    vector<Node*> tmp;
    neighbours_ = tmp;
    neighbours_.clear();
    decoded_ = false;
    delay_ = -1;
    countable_ = false;
}


Node::Node(unsigned long int toa, bool countable) {
    degree = 0;
    timeOfArrival_ = toa;
    vector<Node*> tmp;
    neighbours_ = tmp;
    neighbours_.clear();
    decoded_ = false;
    delay_ = -1;
    countable_ = countable;
}

Node::~Node(){
    vector<Node*>().swap(neighbours_);
}
void Node::addNeighbor(Node* newNeighbor){
    neighbours_.push_back(newNeighbor);
    degree++;
}

void Node::setDegree(int newDegree){
    degree = newDegree;
}

int Node::getDegree()
{
    return degree;
}

void Node::setDecoded()
{
    decoded_ = true;
}

bool Node::getDecoded()
{
    return decoded_;
}

unsigned long int Node::getTimeOfArrival()
{
    return timeOfArrival_;
}

void Node::printDegree(){
    printf("The node has degree %d \n", degree);
}

void Node::printNeighbors(){
    for (int i = 0; i < degree; i++) {
        printf("Neighbour %d has address %p\n", i, neighbours_.at(i));
    }
}

void Node::resolve(unsigned long int timeStep)
{
    delay_ = (int) timeStep - (int) timeOfArrival_;
    while (neighbours_.size() > 0) {
        neighbours_.at(0)->removeEdge(this);
        neighbours_.erase(neighbours_.begin());
        degree--;
    }
}

void Node::removeEdge(Node* adr){
    for (int i = 0; i < (int) neighbours_.size(); i++) {
        if (neighbours_[i] == adr)
        {
            neighbours_.erase(neighbours_.begin()+i);
            degree--;
        }
    }
}

Node* Node::getNeighbour(int i)
{
		return neighbours_[i];
}

int Node::getDelay()
{
    return delay_;
}

void Node::letGoOffNeighbours()
{
    for (int i = 0; i < (int) neighbours_.size(); i++) {
        neighbours_[i]->removeEdge(this);
    }
}

int Node::getNumNeighbours(){
    return (int) neighbours_.size();
}

bool Node::getCountable(){
	return countable_;
}

void Node::setCountable(){
	countable_ = true;
}
