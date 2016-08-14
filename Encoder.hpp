//
//  Encoder.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 28/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Encoder_hpp
#define GUARD_Encoder_hpp

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include <vector>
#include "Node.hpp"
class Encoder {
private:
    int n_;
    int n_rx_;
    int* d_;
    double* Lambda_;
	int size_;
    std::vector<double> cdf_;

public:
    Encoder(int n,int n_rx, int d[], double Lambda[], int size);
    int getRepRate();
    void distributeReps(Node* VN, std::vector<Node*>*CN, bool firstSlot);
    void distributeRepsFirstSlot(Node* VN, std::vector <Node*>*CN);
    void distributeRepsUniform(Node* VN, std::vector<Node*>*CN);
    void distributeRepsSC(Node* VN, std::vector<Node*>*CNs );
};
#endif /* Encoder_hpp */
