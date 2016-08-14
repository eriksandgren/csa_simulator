//
//  Arrivals.hpp
//  CSA_Simulator
//
//  Created by Erik Sandgren on 27/12/15.
//  Copyright © 2015 Erik Sandgren. All rights reserved.
//

#ifndef GUARD_Arrivals_hpp
#define GUARD_Arrivals_hpp

class Arrivals {
    private:
        double mean_;
    public:
        Arrivals(double mean);
        int sample();
};
#endif /* Arrivals_hpp */
