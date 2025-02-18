#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iomanip>
#include <random>
#include <fstream>
#include <filesystem>
#include <dlib/gui_widgets.h>

namespace fs = std::filesystem;

using namespace std;
using namespace dlib;

bool startswith(const string & s, const string &prefix) {
    return s.find(prefix) == 0;
}

namespace odds {
    class color_box : public draggable {
    private:
        unsigned char red, green, blue;

        void draw(const canvas &c) const {
            rectangle area = c.intersect(rect);
            if(area.is_empty() == true) {
                return;
            }

            fill_rect(c, rect, rgb_pixel(red, green, blue));
        }
    public:
        color_box(drawable_window &w, rectangle area, unsigned char r, unsigned char g, unsigned char b) : draggable(w), red(r), green(g), blue(b) {
            rect = area;
            set_draggable_area(rectangle(10, 10, 400, 400));

            enable_events();
        }

        ~color_box() {
            disable_events();

            parent.invalidate_rectangle(rect);
        }
    };

    class win : public drawable_window {
    public:
        win() : 
            c(*this), 
            b(*this), 
            cb(*this, rectangle(100, 100, 200, 200), 0, 0, 255),
            mbar(*this) {
                b.set_pos(10, 60);
                b.set_name("button");

                c.set_pos(b.left(), b.bottom()+5);

                b.set_click_handler(*this, &win::on_btn_clicked);
            }
        ~win() {
            close_window();
        }
    private:
        unsigned long counter;
        label c;
        button b;
        color_box cb;
        menu_bar mbar;

        void on_btn_clicked() {
            counter++;
            ostringstream sout;
            sout << "counter: " << counter;
            c.set_text(sout.str());
        }
    };

    void create_file(const fs::path &p) {
        if(!fs::exists(p)) {
            // create all parent directories if they do not exist
            fs::create_directories(p.parent_path());

            // create the file
            ofstream fh(p);
            if(fh) {
                cout << "file created: " << p << endl;
            } else {
                cerr << "failed to create file: " << p << endl;
            }
        } else {
            cout << "file already exists: " << p << endl;
        }
    }

    void gen_dataset(const int nrows, const int ncols, fs::path p) {
        try {
            create_file(p);

            ofstream fh(p);
            if(!fh) {
                cerr << "error opening file." << endl;
                return;
            }

            random_device rd;
            mt19937 gen(rd());

            // floating-point values
            uniform_real_distribution<double> float_dist(0.0001, 1.0);
            // binary values
            uniform_int_distribution<int> bin_dist(0, 1);
            // last column labels (1, 2, or 3)
            uniform_int_distribution<int> label_dist(1, 3);

            for(int i=0; i<nrows; i++) {
                // first column of float values
                fh << fixed << setprecision(5) << float_dist(gen);
                
                // generate binary columns
                for(int j=1; j<16; j++) {
                    fh << "," << bin_dist(gen);
                }

                // float value columns
                for(int j=16; j<ncols; j++) {
                    fh << "," << fixed << setprecision(5) << float_dist(gen);
                }

                // final lable column
                fh << "," << label_dist(gen) << "\n";
            }

            cout << "generated data file at " << p << endl;

            fh.close();
        } catch(const fs::filesystem_error &e) {
            cerr << "file system error: " << e.what() << endl;
        } catch(const exception &e) {
            cerr << "error: " << e.what() << endl;
        }
    }

    

    vector<string> proc_cmd_str(const string &cmd) {
        stringstream s;
        stringstream ss(cmd);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> vs(begin, end);

        // print it
        // copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
        
        return vs;
    }

    void exec_cmd(const string &cmd) {
        vector<string> ps = proc_cmd_str(cmd);

        /***** hi command *****/
        if(startswith(cmd, "hi")) {
            cout << "hi, there!" << endl;
            
        /***** genset command *****/
        } else if(startswith(cmd, "genset")) {
            // bail if not enough args are supplied
            if(ps.size() < 4) {
                cerr << "command is `genset <filename> <num_rows> <num_cols>`" << endl;
                cout << "*****" << endl;
            } else {
                // generate and save file
                string fname = ps.at(1);
                int nrows = stoi(ps.at(2));
                int ncols = stoi(ps.at(3));
                fs::path p = fs::path(fname);
                gen_dataset(nrows, ncols, p);
            }

        /***** UI *****/
        } else if(startswith(cmd, "win")) {
            // create a new window
            win my_window;

            // wait until user closes this window before terminating it.
            my_window.wait_until_closed();
            

        /***** default *****/
        } else {
            cout << "*****" << endl;
        }
    }
}



int main(int argc, char *argv[]) {
    string command;
    cout << "enter a command (type 'exit' to close application.): ";

    while(getline(cin, command)) {
        if(command == "exit") {
            break;
        }

        odds::exec_cmd(command);
        cout << "enter a command (type 'exit' to close application.): ";
    }

    return 0;
}