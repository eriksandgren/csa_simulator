//
//  Encoder.cpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 28/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#include "Encoder.hpp"
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;
Encoder::Encoder(int n_in,int n_rx_in,int d_in[], double Lambda_in[], int size_in)
{
    n=n_in;
    n_rx=n_rx_in;
    d=d_in;
    Lambda=Lambda_in;
	size = size_in;
	double cumsum = 0;
	cdf.resize(size);
	for (int i = 0; i < size; i++)
	{
		cumsum += Lambda[i];
		cdf.at(i) = cumsum;
	}
    srand ( (unsigned int)time(NULL) );
}

// Simply implements a way of randomly picking a repetition rate according to a defined
// degree distribution.
int Encoder::get_repetition_rate()
{
    double tmp;
    int repetition_rate=0;
    tmp = ((double) rand() / (RAND_MAX));
	while(repetition_rate==0){
    for(int i = size-1; i>=0; i--)
    {
		if (tmp < cdf.at(i))
		{
			repetition_rate = (int)d[i];
        }
    }
	}
    return repetition_rate;
}


void Encoder::distribute_repetitions(Node* VN, vector <Node*>*CNs,bool first_slot){
    if(first_slot)
    {
        distribute_repetitions_first_slot(VN, CNs);
    }
    else
    {
        distribute_repetitions_uniformly(VN, CNs);
    }
    
}


// Chooses a degree and CNs for transmission for a VN replicas are placed uniformly in the 'n'
// following CNs
void Encoder::distribute_repetitions_uniformly(Node* VN,vector<Node*>*CNs ){
    int repetition_rate=get_repetition_rate();
    int tmp;
    vector<int> return_vector(repetition_rate);
    bool not_okay_placement;
    Node* CN;
    for (int i=0; i<repetition_rate; i++) {
        do {
            not_okay_placement=false;
            tmp=rand()%(n);
            for (int ii=0; ii<i; ii++) {
                if(tmp== return_vector[ii]) {not_okay_placement=true;};
            }
        } while (not_okay_placement);
        return_vector[i]=tmp;
    }
    
    // Connects the correct CNs to the VN and the VN to the CNs...
    for (int i=0; i<repetition_rate; i++)
    {
        CN=CNs->at(return_vector.at(i)+n_rx);
        CN->addNeighbor(VN);
        VN->addNeighbor(CN);
    }
}

// Chooses a degree and CNs for transmission for a VN (a first replica is placed in the next CN)
void Encoder::distribute_repetitions_first_slot(Node* VN, vector<Node*>*CNs)
{
    int repetition_rate=get_repetition_rate();
    int tmp;
	vector<int> return_vector(repetition_rate);
	bool not_okay_placement;
    Node* CN;
    return_vector[0]=0;
    for (int i=1; i<repetition_rate; i++) {
        do {
            not_okay_placement=false;
            tmp=rand()%(n-1)+1;
            for (int ii=0; ii<i; ii++) {
                if(tmp==return_vector[ii]) {not_okay_placement=true;};
            }
        } while (not_okay_placement);   
        return_vector[i]=tmp;
    }
    
    // Connects the correct CNs to the VN and the VN to the CNs...
    for (int i=0; i<repetition_rate; i++)
    {
        CN=CNs->at(return_vector.at(i)+n_rx);
        CN->addNeighbor(VN);
        VN->addNeighbor(CN);
    }
}