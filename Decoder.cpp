//
//  Decoder.cpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 06/01/16.
//  Copyright © 2016 Erik Sandgren. All rights reserved.
//
#include "Decoder.hpp"
using namespace std;
Decoder::Decoder(int iter,int steps,int n_in,int n_rx_in,int max_delay_in)
{
	delays.resize(max_delay_in+1);

    for (int i=0; i< (int) delays.size(); i++) {
        delays.at(i)=0;
    }
    max_delay=max_delay_in;
    n=n_in;
    n_rx=n_rx_in;
    max_decoding_iterations=iter;
    steps_per_decoding=steps;
    lost_packets=0;
    sent_packets=0;
}

void Decoder::decode(vector<Node*>* CN,vector<Node*>* VN, unsigned long int time_step, int &num_singleton)
{
    
    // Checks if it is time to decode
    if ((time_step%steps_per_decoding)==0)
    {
        int count_singletons=0;
        int iteration=0;
        vector<Node*>:: iterator iter;
        bool condition;
		vector<Node*> VNs_to_resolve;
        do {
            // Finds all degree-1 CNs in the memory..
            for (int i=0; i<n_rx; i++)
            {
                if ((*CN)[i]->degree==1)
                {
                    count_singletons++;
					VNs_to_resolve.push_back(CN->at(i)->getNeighbour(0));
                }
            }
           // Resolves all VNs corresponding to the found degree-1 CNs
           for (int i=0; i< (int)VNs_to_resolve.size(); i++)
           {
                VNs_to_resolve.at(i)->resolve(VNs_to_resolve.at(i),time_step);
               VNs_to_resolve.at(i)->setDecoded();
           }
            iteration++;
            // We continue if there were some degree one CNs in this iteration and if we have not reached the maximum number of iterations yet.
            condition= (VNs_to_resolve.size()!=0 && iteration <max_decoding_iterations);
			VNs_to_resolve.clear();
        }
        while (condition);
        num_singleton=count_singletons;
    }
}

void Decoder::count_packets(vector<Node *> *VN, unsigned long time_step,bool boundary_effect)
{
    if (boundary_effect)
    {
        count_packets_boundary_effect(VN,time_step);
    }
    else
    {
        count_packets_no_boundary_effect(VN,time_step);
    }
}

void Decoder::count_packets_boundary_effect(vector<Node *> *VN, unsigned long time_step)
{
    int i=0;
    int len=(int) VN->size();
    int time_since_arrival;
    Node* temp;
    while (i<len)
    {
        time_since_arrival= (int)(time_step- (VN->at(0)->getTimeOfArrival()));
        if (time_since_arrival>n_rx+n)
        {
            temp=VN->at(0);
            i++;
            if(temp->getTimeOfArrival()> (unsigned long) (3*n)){
            sent_packets++;
            
            if(temp->getDecoded() && VN->at(0)->getDelay()<=max_delay)
            {
                delays.at(temp->getDelay())++;
            }
            
            else{
                lost_packets++;
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


void Decoder::count_packets_no_boundary_effect(vector<Node *> *VN, unsigned long time_step)
{
    int i=0;
    int len=(int) VN->size();
    int time_since_arrival;
    Node* temp;
    
    
    while (i<len)
    {
        time_since_arrival= (int)(time_step- (VN->at(0)->getTimeOfArrival()));
        if (time_since_arrival>n_rx+n)
        {
            temp=VN->at(0);
            //temp->letGoOffNeighbours(temp);
            i++;
            
            if(VN->at(0)->getTimeOfArrival()<(unsigned long)2*n_rx+n)
            {
                VN->erase(VN->begin());
                delete temp;
            }
            else{
                sent_packets++;
                
                if(temp->getDecoded() && temp->getDelay()<=max_delay)
                {
                    delays.at(temp->getDelay())++;
                }
                
                else{
                    lost_packets++;
                }
                VN->erase(VN->begin());
                delete temp;
            }
        }
        else
        {
            i=len;
        }
    }
}


int Decoder::getSentPackets()
{
    return sent_packets;
}

int Decoder::getLostPackets()
{
    return lost_packets;
}

vector<int> Decoder::getDelays()
{
    return delays;
}



void Decoder::printFrame(vector<Node*>* CN)
{
    for (int i=0; i< (int) CN->size(); i++) {
        printf("%d ",CN->at(i)->degree);
    }
    printf("\n");
}