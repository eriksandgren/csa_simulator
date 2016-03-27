//
//  Arrivals.cpp
//  CSA_Simulator
//  Implements a Poisson arrival process.

//  Created by Erik Sandgren on 27/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#include "Arrivals.hpp"
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

Arrivals::Arrivals(double new_mean){
     mean=new_mean;
     srand ( (unsigned int)time(NULL) );
}

int Arrivals::sample(){
    int k=0;
    double p=1;
    double L=exp(-mean);
    do{
        k=k+1;
        // return a uniform number in [0,1].
        double u = rand() / (double)RAND_MAX;
        p=p*u;
    }   while (p>L);    
    return k-1;
}

