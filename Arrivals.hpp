//
//  Arrivals.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 27/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Arrivals_hpp
#define GUARD_Arrivals_hpp

#include <stdio.h>
#include <cmath>
#include <cstdlib>

class Arrivals {
    private:
        double mean;
    public:
        Arrivals(double new_mean);
        int sample();
};
#endif /* Arrivals_hpp */
