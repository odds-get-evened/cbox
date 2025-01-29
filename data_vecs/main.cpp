#include <iostream>
#include "inc/odds.hpp"

using namespace std;

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