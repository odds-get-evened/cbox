#include <iostream>
#include <dlib/gui_widgets.h>
#include <dlib/image_transforms.h>
#include <cmath>
#include <vector>
#include <string>
#include <any>

using namespace std;
using namespace dlib;



namespace prac {
    std::vector<string> read_stropwords_from_file(const std::string fname) {
        std::vector<string> words;
        std::ifstream fh(fname);
        std::string word;

        if(fh.is_open()) {
            while(std::getline(fh, word)) {
                words.push_back(word);
            }

            fh.close();
        } else {
            cerr << "unable to open file: " << fname << endl;
            exit(-1);
        }

        return words;
    }

    void do_point_cloud() {
        // make a point cloud that looks like a 3D spiral
        std::vector<perspective_window::overlay_dot> points;
        dlib::rand rnd;

        for(double i=0; i<20; i+=0.001) {
            // get a point on the spiral
            dlib::vector<double> val(sin(i), cos(i), i/4);

            // now add some random noise
            dlib::vector<double> temp(rnd.get_random_gaussian(), rnd.get_random_gaussian(), rnd.get_random_gaussian());
            val += temp / 20;

            // pick a color based on how far we are along the spiral
            rgb_pixel color = colormap_jet(i, 0, 20);

            // add the point to the list of points we will display
            points.push_back(perspective_window::overlay_dot(val, color));
        }

        // display the point cloud
        perspective_window win;
        win.set_title("perspective window 3D point cloud");
        win.set_size(640, 480);
        
        win.add_overlay(points);
        win.wait_until_closed();
    }

    void print_1d_vec(const std::vector<std::any> &d) {
        for(auto &i : d) {
            cout << std::any_cast<string>(i) << endl;
        }
    }

    /**
     * encode a string, converting it to a 64-bit integer
     * (i.e., a string signature)
     */
    uint64_t bigend_int64(const std::string &s) {
        uint64_t res = 0;

        for(size_t i=0; i<s.size() && i<8; i++) {
            res = (res << 8) | static_cast<uint8_t>(s[i]);
        }

        return res;
    }

    /**
     * normalize a vector of numbers
     */
    std::vector<double> normalize_arr(const std::vector<unsigned long long> &arr, double t_min = -1, double t_max = 1) {
        std::vector<double> norm_arr;
        norm_arr.reserve(arr.size());

        double diff = t_max - t_min;
        unsigned long long min_val = *std::min_element(arr.begin(), arr.end());
        unsigned long long max_val = *std::max_element(arr.begin(), arr.end());
        unsigned long long diff_arr = max_val - min_val;

        for(const auto &i : arr) {
            double tmp;
            if(diff_arr != 0) {
                tmp = (((static_cast<double>(i) - min_val) * diff) / diff_arr) + t_min;
            } else {
                // use midpoint value for division by zero handling
                tmp = (t_min + t_max) / 2;
            }

            norm_arr.push_back(tmp);
        }

        return norm_arr;
    }

    void do_nlp(string s) {
        // split up string into words
        std::vector<string> ss = dlib::split(s);
        
        // std::vector<std::any> avec(ss.begin(), ss.end());
        // print_1d_vec(avec);
        
        // cout << bigend_int64(s) << endl;
        // vector<double> n = normalize_arr()
    }
}

namespace odds {
    
}

int main(int argc, char *argv[]) {
    // prac::do_point_cloud();
    prac::do_nlp("some stupid text because i hate looking for it...");

    return 0;
}