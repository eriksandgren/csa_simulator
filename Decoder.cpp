//
//  Decoder.cpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 06/01/16.
//  Copyright © 2016 Erik Sandgren. All rights reserved.
//
#include "Decoder.hpp"
using namespace std;
Decoder::Decoder(int iter, int steps, int n_in, int n_rx_in, int max_delay_in)
{
	delays_.resize(max_delay_in + 1);

    for (int i = 0; i < (int) delays_.size(); i++) {
        delays_.at(i) = 0;
    }
    maxDelay_ = max_delay_in;
    n_ = n_in;
    n_rx_ = n_rx_in;
    numDecodingIterations_ = iter;
    slotsBetweenDecoding_ = steps;
    lostPacketsCount_ = 0;
    sentPacketsCount_ = 0;
}

void Decoder::decode(vector<Node*>* CN, vector<Node*>* VN, unsigned long int timeStep)
{
    
    // Checks if it is time to decode
    if ( (timeStep % slotsBetweenDecoding_)==0 )
    {
        int iteration = 0;
        bool stopCondition;
		vector<Node*> VnsToResolve;
        do {
            // Finds all degree-1 CNs in the memory..
            for (int i = 0; i < n_rx_; i++)
            {
                if ( (*CN)[i] -> degree == 1)
                {
					VnsToResolve.push_back(CN->at(i)->getNeighbour(0));
                }
            }
           // Resolves all VNs corresponding to the found degree-1 CNs
           for (int i = 0; i < (int) VnsToResolve.size(); i++)
           {
                VnsToResolve.at(i) -> resolve(VnsToResolve.at(i), timeStep);
               VnsToResolve.at(i) -> setDecoded();
           }
            iteration++;
            // We continue if there were some degree one CNs in this iteration and if we have not reached the maximum number of iterations yet.
            stopCondition = (VnsToResolve.size() != 0 && iteration < numDecodingIterations_);
			VnsToResolve.clear();
        }
        while (stopCondition);
    }
}

void Decoder::decodeFrame(vector<Node*>* CN, vector<Node*>* VN, unsigned long int timeStep)
{
    int iteration = 0;
    vector<Node*> VnsToResolve;
    bool stopCondition;
        do
        {
            // Finds all degree-1 check nodes in the memory..
            for (int i = 0; i < n_ ; i++)
            {
                if ((*CN)[i] -> degree == 1)
                {
                    VnsToResolve.push_back(CN->at(i)->getNeighbour(0));
                }
            }
            // Resolving the degree-1 users found and their corresponding connections
            for (int i = 0; i < VnsToResolve.size(); i++)
            {
                VnsToResolve.at(i)->resolve(VnsToResolve.at(i), timeStep);
                VnsToResolve.at(i)->setDecoded();
            }
            iteration++;
            stopCondition = (VnsToResolve.size() != 0 && iteration < numDecodingIterations_);
            VnsToResolve.clear();
        }
        while (stopCondition);
}

void Decoder::countPacketsBoundaryEffect(vector<Node *> *VN, unsigned long timeStep)
{
    int i = 0;
    int len = (int) VN -> size();
    int timeSinceArrival;
    Node* temp;
    while (i < len)
    {
        timeSinceArrival = (int) (timeStep - (VN->at(0)->getTimeOfArrival()));
        if (timeSinceArrival > n_rx_ + n_)
        {
            temp = VN->at(0);
            i++;
            if(temp->getTimeOfArrival() > (unsigned long) ( 3 * n_)){
            sentPacketsCount_++;
            
            if(temp->getDecoded() && VN->at(0)->getDelay() <= maxDelay_){
                delays_.at(temp->getDelay())++;
            }
            
            else{
                lostPacketsCount_++;
            }
            }
            VN->erase(VN->begin());
            delete temp;
        }
        else
        {
            i=len;
        }
    }
}

void Decoder::countPacketsNoBoundaryEffect(vector<Node *> *VN, unsigned long timeStep)
{
    int i = 0;
    int len = (int) VN->size();
    int timeSinceArrival;
    Node* temp;
    
    
    while (i < len)
    {
        timeSinceArrival= (int)(timeStep - (VN->at(0)->getTimeOfArrival()));
        if (timeSinceArrival > n_rx_ + n_)
        {
            temp = VN->at(0);
            i++;
            // TODO: Send in a variable saying where we start counting packets instead of relying on this computation
            if(VN->at(0)->getTimeOfArrival() < (unsigned long) 2 * n_rx_ + n_)
            {
                VN->erase(VN->begin());
                delete temp;
            }
            else{
                sentPacketsCount_++;
                
                if(temp -> getDecoded() && temp -> getDelay() <= maxDelay_)
                {
                    delays_.at(temp->getDelay())++;
                }
                
                else{
                    lostPacketsCount_++;
                }
                VN->erase(VN->begin());
                delete temp;
            }
        }
        else
        {
            i = len;
        }
    }
}

void Decoder::countPacketsSC(vector<Node *> *VN, unsigned long timeStep, int rep)
{
    int len=(int) VN->size();
    int timeSinceArrival;
    Node* temp;
    while (len != 0)
    {
        timeSinceArrival = (int) (timeStep - (VN->at(0)->getTimeOfArrival()));
        if (timeSinceArrival > n_rx_ + n_)
        {
            temp = (*VN)[0];
            // TODO: Send in a variable saying where we start counting packets instead of relying on this computation
            if(temp->getTimeOfArrival() > (unsigned long) (rep * n_ + 5 * n_rx_))
            {
                sentPacketsCount_++;

                if(temp->getDecoded() && VN->at(0)->getDelay()<=maxDelay_)
                {
                    delays_.at(temp->getDelay())++;
                }
                else
                {
                    lostPacketsCount_++;
                }
            }
            VN->erase(VN->begin());
            len= VN->size();
        }
        else
        {
            len=0;
        }
    }
}



void Decoder::countPacketsFS(vector<Node *> *VN, unsigned long time_step)
{
    // For FS we simply count packets at the end of every frame! This empties also the old VN-vector.
    int i=0;
    int len=int(VN->size());
    Node* temp;
    while (i<len)
    {
            temp=VN->at(0);
            temp->letGoOffNeighbours(temp);
            i++;
            sentPacketsCount_++;
            if(temp->getDecoded())
            {
                delays_.at(temp->getDelay())++;
            }
            else
            {
                lostPacketsCount_++;
            }

            VN->erase(VN->begin());
            delete temp;
    }

}



int Decoder::getSentPackets()
{
    return sentPacketsCount_;
}

int Decoder::getLostPackets()
{
    return lostPacketsCount_;
}

vector<int> Decoder::getDelays()
{
    return delays_;
}

void Decoder::printFrame(vector<Node*> *CN)
{
    for (int i = 0; i < (int) CN->size(); i++) {
        printf("%d ", CN->at(i)->degree);
    }
    printf("\n");
}
