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
    degree=0;
    vector<Node*> tmp;
    neighbours=tmp;
    decoded=false;
}

Node::Node(unsigned long int toa) {
    degree=0;
    timeOfArrival=toa;
    vector<Node*> tmp;
    neighbours=tmp;
    neighbours.clear();
    decoded=false;
}
Node::~Node(){
    vector<Node*>().swap(neighbours);
}
// TODO: Add a break if the number of neighbors would exceed the max amount!
void Node::addNeighbor(Node* newNeighbor){
    neighbours.push_back(newNeighbor);
    degree++;
}

void Node::setDegree(int newDegree){
    degree=newDegree;
}

int Node::getDegree()
{
    return degree;
}


void Node::setDecoded()
{
    decoded=true;
    if(degree!=0){
      //  printf("what");
    }
}


bool Node::getDecoded()
{
    return decoded;
}



unsigned long int Node::getTimeOfArrival()
{
    return timeOfArrival;
}




void Node::printDegree(){
    printf("The node has degree %d \n",degree);
}



void Node::printNeighbors(){
    for (int i=0; i<degree; i++) {
        printf("Neighbour %d has address %p\n",i,neighbours.at(i));
    }
}


void Node::resolve(Node* adr,unsigned long int time_step)
{
    delay=(int)time_step-(int)timeOfArrival;
    while (neighbours.size()>0) {
        neighbours.at(0)->remove_edge(adr);
        neighbours.erase(neighbours.begin());
        degree--;
    }
    
  //  for (int i=0; i<nei_size; i++) {
   //     degree--;
    //    neighbours.at(i)->remove_edge(adr);
     //   neighbours.erase(neighbours.begin()+i);
   // }
    if (degree!=0) {
        printf("degree is: %d \n",degree);
    }
}



void Node::remove_edge(Node* adr){
    for (int i=0; i < (int) neighbours.size(); i++) {
        if (neighbours[i]==adr)
        {
            neighbours.erase(neighbours.begin()+i);
            degree--;
        }
    }
}

void Node::letGoOffEdge(Node* adr)
{
    for (int i=0; i < (int) neighbours.size(); i++) {
        if (neighbours[i]==adr)
        {
            neighbours.erase(neighbours.begin()+i);
            degree--;
        }
    }
}

Node* Node::getNeighbour(int i)
{
		return neighbours[i];
}


int Node::getDelay()
{
    return delay;
}
void Node::letGoOffNeighbours(Node* adr)
{
    for (int i=0; i<(int) neighbours.size(); i++) {
        neighbours[i]->letGoOffEdge(adr);
    }
    //vector<Node*>().swap(neighbours);
}
int Node::getNumNeighbours(){
    return (int)neighbours.size();
}
