//
//  Decoder.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 06/01/16.
//  Copyright © 2016 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Decoder_hpp
#define GUARD_Decoder_hpp

#include <stdio.h>
#include "Node.hpp"
#include <vector>
class Decoder {
private:
 
    int n;
    int n_rx;
    int max_delay;
    std::vector<int> delays;
    int sent_packets;
    int lost_packets;
    int max_decoding_iterations;
    int steps_per_decoding;
public:
    Decoder(int iter,int steps,int n_in, int n_rx_in,int max_delay_in);
    void decode(std::vector<Node*> *CN,std::vector<Node*> *VN, unsigned long int time_step);
    void count_packets(std::vector<Node*> *VN, unsigned long int time_step,bool boundary_effect);
    void count_packets_boundary_effect(std::vector<Node *> *VN, unsigned long time_step);
    void count_packets_no_boundary_effect(std::vector<Node *> *VN, unsigned long time_step);
    
    int getSentPackets();
    int getLostPackets();
    std::vector<int> getDelays();
    void printFrame(std::vector<Node*>* CN);
};

#endif /* Decoder_hpp */
