#include <iostream>
#include <cstdint>
#include <random>
#include <iomanip>

using namespace std;


namespace odds {
    struct Coord {
        float x, y, z;
    };
    
    float gen_rand_float(float min, float max) {
        static random_device rd;  // seed
        static mt19937 gen(rd());  // RNG
        uniform_real_distribution<float> dist(min, max);

        return dist(gen);
    }

    vector<vector<Coord>> gen_matrix(int y, int x, float min, float max) {
        vector<vector<Coord>> matrix(y, vector<Coord>(x));

        for(int i=0; i<y; ++i) {
            for(int j=0; j<x; ++j) {
                matrix[i][j] = {
                    gen_rand_float(min, max),
                    gen_rand_float(min, max),
                    gen_rand_float(min, max)
                };
            }
        }

        return matrix;
    }

    void print_matrix(const vector<vector<Coord>> &matrix) {
        for(const auto &y : matrix) {
            for(const auto &coord : y) {
                cout << fixed << setprecision(16)
                    << "(" << coord.x << ", " << coord.y
                    << ", " << coord.z << ")";
            }
            cout << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    int rows, cols;
    float min, max;

    cout << "enter number of rows: ";
    cin >> rows;
    cout << "enter number of columns: ";
    cin >> cols;
    cout << "enter low-end of range: ";
    cin >> min;
    cout << "enter high-end of range: ";
    cin >> max;

    auto matrix = odds::gen_matrix(rows, cols, min, max);
    cout << "generated matrix of 3D coordinates" << endl;
    odds::print_matrix(matrix);

    

    return 0;
}