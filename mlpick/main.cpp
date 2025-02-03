#include <iostream>
#include <zlib.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/linear_regression/linear_regression.hpp>

using namespace std;

using namespace arma;

int main(int argc, char *argv[]) {
    // arma::vec a = arma::linspace(0, 5, 6);
    // a.print("");

    mat dataset(10, 10, arma::fill::randu);           // fill 1000 points of randon uniform values frm 0 to 1
    rowvec res = randn<rowvec>(10);
    dataset.print("real data: ");
    res.print("responses to real data for training: ");

    mat test_dataset(10, 5, arma::fill::randu);       // fill 500 ...
    test_dataset.print("test data: ");

    // step 1. create the model
    mlpack::LinearRegression lr;
    // step 2. train model
    lr.Train(dataset, res);
    rowvec predictions;
    // step 3. use the model to predict
    lr.Predict(test_dataset, predictions);
    predictions.print("predictions: ");

    return 0;
}