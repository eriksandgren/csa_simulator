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
Encoder::Encoder(int n,int n_rx,int d[], double Lambda[], int size)
{
    n_=n;
    n_rx_=n_rx;
    d_=d;
    Lambda_=Lambda;
	size_ = size;
	double cumsum = 0;
	cdf_.resize(size_);
	for (int i = 0; i < size_; i++)
	{
		cumsum += Lambda_[i];
		cdf_.at(i) = cumsum;
	}
    srand ( (unsigned int)time(NULL) );
}

// Simply implements a way of randomly picking a repetition rate according to a defined
// degree distribution.
int Encoder::getRepRate()
{
    double tmp;
    int repRate = 0;
    tmp = ((double) rand() / (RAND_MAX));
	while(repRate == 0){
    for(int i = size_ - 1; i >= 0; i--)
    {
		if (tmp < cdf_.at(i))
		{
			repRate = (int) d_[i];
        }
    }
	}
    return repRate;
}

// Chooses a degree and CNs for transmission for a VN replicas are placed uniformly in the 'n'
// following CNs
void Encoder::distributeRepsUniform(Node* VN, vector<Node*>*CNs ){
    int repRate=getRepRate();
    int tmp;
    vector<int> returnVec(repRate);
    bool invalidPlacement;
    Node* CN;
    for (int i = 0; i < repRate; i++) {
        do {
            invalidPlacement = false;
            tmp=rand() % n_;
            for (int ii = 0; ii < i; ii++) {
                if(tmp == returnVec[ii]) {invalidPlacement=true;};
            }
        } while (invalidPlacement);
        returnVec[i] = tmp;
    }
    
    // Connects the correct CNs to the VN and the VN to the CNs...
    for (int i = 0; i < repRate; i++)
    {
        CN=CNs->at(returnVec.at(i) + n_rx_);
        CN->addNeighbor(VN);
        VN->addNeighbor(CN);
    }
}

// Chooses a degree and CNs for transmission for a VN (a first replica is placed in the next CN)
void Encoder::distributeRepsFirstSlot(Node* VN, vector<Node*>*CNs)
{
    int repRate = getRepRate();
    int tmp;
	vector<int> returnVec(repRate);
	bool invalidPlacement;
    Node* CN;
    returnVec[0] = 0;
    for (int i = 1; i < repRate; i++) {
        do {
            invalidPlacement = false;
            tmp = rand() % (n_ - 1) + 1;
            for (int ii = 0; ii <i; ii++) {
                if(tmp == returnVec[ii]) {invalidPlacement = true;};
            }
        } while (invalidPlacement);   
        returnVec[i] = tmp;
    }
    
    // Connects the correct CNs to the VN and the VN to the CNs...
    for (int i = 0; i < repRate; i++)
    {
        CN = CNs->at(returnVec.at(i) + n_rx_);
        CN->addNeighbor(VN);
        VN->addNeighbor(CN);
    }
}

void Encoder::distributeRepsSC(Node* VN,vector<Node*>*CNs )
{
    int tmp;
    vector<int> returnVec;
    Node* CN;
    for (int i = 0; i < d_[0]; i++)
    {
        tmp=(rand() % n_ ) + i * n_;
        returnVec.push_back(tmp);
    }
    // Connects the correct CNs to the VN and the VN to the CNs...
    for (int i = 0; i < d_[0]; i++)
    {
        CN = CNs->at(returnVec.at(i) + n_rx_);
        CN->addNeighbor(VN);
        VN->addNeighbor(CN);
    }
}
