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
    int n;
    int n_rx;
    int* d;
    double* Lambda;
	int size;
    std::vector<double> cdf;
public:
    Encoder(int n_in,int n_rx_in, int d_in[], double Lambda_in[], int size_in);
    int get_repetition_rate();
    void distribute_repetitions(Node* VN, std::vector<Node*>*CN, bool first_slot);
    void distribute_repetitions_first_slot(Node* VN, std::vector <Node*>*CN);
    void distribute_repetitions_uniformly(Node* VN,std::vector<Node*>*CN);
};
#endif /* Encoder_hpp */
