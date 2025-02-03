#include "inc/odds.hpp"

int main(int argc, char *argv[]) {
    double r1[2] = {-21.0, 98.0};
    double i1[2] = {-1.0, 1.0};
    vector<double> vec1 = odds::gen_rand_temps(10, r1, i1);

    

    return 0;
}