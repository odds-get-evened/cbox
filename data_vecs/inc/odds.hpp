#include <iostream>
#include <random>
#include <vector>
#include <stdexcept>

using namespace std;

namespace odds {
    vector<double> gen_rand_temps(int len, double r[], double i[]) {
        if(!r || !i) {
            throw invalid_argument("range and increment arrays cannot be null");
        }

        if(sizeof(r) / sizeof(r[0]) < 2 || sizeof(i) / sizeof(i[0]) < 2) {
            throw invalid_argument("range and increment arrays must have at least 2 elements");
        }

        random_device rnd;
        mt19937 gen(rnd());
        uniform_real_distribution<> distr(r[0], r[1]);
        uniform_real_distribution<> incr_distr(i[0], i[1]);
        
        vector<double> tmp;
        // initialize with the first random temperature
        double prev_tmp = distr(gen);
        tmp.emplace_back(prev_tmp);
        
        for(int i=1; i<len; i++) {
            double incr = incr_distr(gen);
            double next_tmp = prev_tmp + incr;

            // ensure next_tmp is within specified range
            if(next_tmp < r[0]) {
                next_tmp = r[0];
            } else if(next_tmp > r[1]) {
                next_tmp = r[1];
            }

            tmp.emplace_back(next_tmp);
            prev_tmp = next_tmp;
        }

        return tmp;
    }
}