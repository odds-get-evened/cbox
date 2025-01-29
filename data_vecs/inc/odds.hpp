#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

using namespace std;

using boost::asio::ip::tcp;

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

    void stream_temps(int len, double r[], double i[], tcp::socket &sock) {
        vector<double> temps = gen_rand_temps(len, r, i);

        for(const double &temp : temps) {
            string temp_str = to_string(temp) + " ";
            boost::asio::write(sock, boost::asio::buffer(temp_str));
        }

        boost::asio::write(sock, boost::asio::buffer(endl));
    }
}

int main(int argc, char *argv[]) {
    double rng[] = {-21.0, 95.0};
    double incr_rng[] = {-1.0, 1.0};

    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
    } catch(exception &e) {
        cerr << "exception: " << e.what() << endl;
    }

    return 0;
}