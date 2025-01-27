#include <iostream>
#include <random>
#include <vector>

using namespace std;

namespace odds {
    vector<double> gen_rand_temps(int len, double r[], double i[]) {
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

int main(int argc, char *argv[]) {
    double rng[] = {-21.0, 95.0};
    double inc_rng[] = {-1.0, 1.0};
    vector<double> a = odds::gen_rand_temps(48, rng, inc_rng);

    for(int i=0; i<a.size(); i++) {
        cout << a.at(i) << " ";
    }
    cout << endl;

    return 0;
}