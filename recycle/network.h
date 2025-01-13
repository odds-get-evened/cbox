#ifndef ODDS_NET_H
#define ODDS_NET_H

#include <iostream>
#include <vector>

using namespace std;

namespace odds {
    using NetVec = vector<vector<float>>;

    class Network {
        public:
            Network();
        private:
            int rows;
            int cols;
    };
}

#endif  // ODDS_NET_H