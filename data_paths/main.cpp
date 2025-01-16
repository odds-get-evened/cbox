#include <iostream>
#include <zlib.h>
#include <cstdint>
#include <random>
#include <vector>
#include <mlpack/core.hpp>
#include <mlpack/methods/linear_regression/linear_regression.hpp>

using namespace std;
using namespace mlpack;

namespace odds {
    // define type for matrix of vectors
    using MatrixVec = vector<vector<double>>;

    MatrixVec gen_rand_matrix(size_t len) {
        // seed random number generator
        random_device rd;
        mt19937 gen(rd());
        // pick random numbers between -1 and 1
        uniform_real_distribution<double> dist(-1.0, 1.0);

        MatrixVec vec(len, vector<double>(3));

        // fill matrix with random numbers
        for(size_t i=0; i<len; i++) {
            for(size_t j=0; j<3; j++) {
                vec[i][j] = dist(gen);
            }
        }

        return vec;
    }

    vector<uint8_t> gen_rand_bytes(size_t len) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<uint8_t> dist(0, 255);

        vector<uint8_t> arr(len);
        for(size_t i=0; i<len; i++) {
            arr[i] = dist(gen);
        }

        return arr;
    }

    uLong crc32_hash(const vector<uint8_t> &d) {
        uLong c = crc32(0L, Z_NULL, 0); // initialize crc32
        c = crc32(c, d.data(), d.size());

        return c;
    }

    LinearRegression train_model(const vector<vector<double>> &d) {
        // create matrix from data
        arma::mat X(d.size(), 2);
        // create vector from data
        arma::rowvec y(d.size());

        // fill matrix and vector
        for(size_t i=0; i<d.size(); i++) {
            X(i, 0) = d[i][0];
            X(i, 1) = d[i][1];
            y(i) = d[i][2];
        }

        LinearRegression lr(X, y);

        return lr;
    }

    vector<vector<double>> predict_new_matrix(LinearRegression &lr, size_t len) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(-1.0, 1.0);

        vector<vector<double>> new_mtx(len, vector<double>(3));

        // fill matrix with random numbers
        arma::mat X(len, 2);

        for(size_t i=0; i<len; i++) {
            new_mtx[i][0] = dist(gen);
            new_mtx[i][1] = dist(gen);
            X(i, 0) = new_mtx[i][0];
            X(i, 1) = new_mtx[i][1];
        }

        arma::rowvec predictions;
        lr.Predict(X, predictions);

        for(size_t i=0; i<len; i++) {
            new_mtx[i][2] = predictions(i);
        }

        return new_mtx;
    }
    
    class InteCirc {
        private:
        uLong id;
        
        public:
        InteCirc() {
            id = crc32_hash(gen_rand_bytes(16));
        }
        uLong get_id() const { return id; }
    };
}



int main() {
    odds::InteCirc ic;
    cout << "InteCirc ID: " << hex << ic.get_id() << endl;

    return 0;
}