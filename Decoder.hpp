//
//  Decoder.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 06/01/16.
//  Copyright © 2016 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Decoder_hpp
#define GUARD_Decoder_hpp

#include "Node.hpp"
#include <vector>
class Decoder {
private:
    int n_;
    int n_rx_;
    int maxDelay_;
    std::vector<int> delays_;
    int sentPacketsCount_;
    int lostPacketsCount_;
    int numDecodingIterations_;
    int slotsBetweenDecoding_;

public:
    Decoder(int iter, int slots, int n, int n_rx, int maxDelay);
    void decode(std::vector<Node*> *CN,std::vector<Node*> *VN, unsigned long int timeStep);
    void decodeFrame(std::vector<Node*>* CN,std::vector<Node*>* VN, unsigned long int timeStep);
    void countPacketsBoundaryEffect(std::vector<Node *> *VN, unsigned long timeStep);
    void countPacketsNoBoundaryEffect(std::vector<Node *> *VN, unsigned long timeStep);
    void countPacketsFS(std::vector<Node *> *VN, unsigned long timeStep);
    void countPacketsSC(std::vector<Node *> *VN, unsigned long timeStep, int rep);

    int getSentPackets();
    int getLostPackets();
    std::vector<int> getDelays();
    void printFrame(std::vector<Node*>* CN);
};

#endif /* Decoder_hpp */
