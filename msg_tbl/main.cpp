#include <iostream>
#include <map>
#include <string>
#include <sstream>      // for generating dynamic messages
#include <iomanip>      // for setw, setfill
#include <algorithm>    // for transform
#include <fstream>      // for file operations
#include <filesystem>   // for filesystem operations

using namespace std;
namespace fs = filesystem;

namespace odds {
    struct Message {
        int id;     // message id
        string txt; // message
    };

    using MsgTbl = map<int, Message>;

    MsgTbl create_msg_table() {
        MsgTbl msg_tbl;

        // populate the message table
        msg_tbl.emplace(0x00, Message{0x00, "INIT"});
        msg_tbl.emplace(0x01, Message{0x01, "INIT_END"});
        msg_tbl.emplace(0x02, Message{0x02, "START_PROC"});
        msg_tbl.emplace(0x03, Message{0x03, "STOP_PROC"});
        msg_tbl.emplace(0x04, Message{0x04, "ERR_OVER_TEMP"});
        msg_tbl.emplace(0x05, Message{0x05, "ERR_VOLT_DOWN"});

        // add 10 more empty placeholder messages
        for(int i=0x06; i<0x10; i++) {
            msg_tbl.emplace(i, Message{i, ""});
        }

        return msg_tbl;
    }

    string msg_lookup(const MsgTbl &tbl, int id) {
        auto it = tbl.find(id);
        if (it != tbl.end()) {
            return it->second.txt;
        }

        return "ERR_UNKNOWN";
    }

    bool add_msg(MsgTbl &tbl, int id, const string &txt) {
        if(id >= 0x06 && id < 0x10) {
            tbl[id] = Message{id, txt};

            return true;
        }

        return false;
    }

    bool remove_msg(MsgTbl &tbl, int id) {
        if(id >= 0x06 && id < 0x10) {
            return tbl.erase(id) > 0;
        }

        return false;
    }

    void print_table(const MsgTbl &tbl) {
        // print the message table
        for(const auto &[id, msg] : tbl) {
            ostringstream id_buff;
            id_buff << "0x" << hex << setw(2) << setfill('0') << id;
            cout << "id: " << id_buff.str() << " msg: " << msg.txt << endl; 
        }
    }

    void save_table(const MsgTbl &tbl, const string &fname) {
        ofstream file(fname, ios::binary);

        if(file.is_open()) {
            for(const auto &[id, msg] : tbl) {
                file.write(reinterpret_cast<const char *>(&id), sizeof(id));
                size_t txt_size = msg.txt.size();
                file.write(reinterpret_cast<const char *>(&txt_size), sizeof(txt_size));
                file.write(msg.txt.c_str(), txt_size);
            }

            file.close();
        } else {
            cerr << "unable to open file for writing. " << fname << endl;
        }
    }

    MsgTbl load_table(const string &fname) {
        MsgTbl tbl;
        ifstream file(fname, ios::binary);

        if(file.is_open()) {
            while(file.peek() != EOF) {
                int id;
                file.read(reinterpret_cast<char *>(&id), sizeof(id));
                size_t txt_size;
                file.read(reinterpret_cast<char *>(&txt_size), sizeof(txt_size));
                string txt(txt_size, ' ');
                file.read(&txt[0], txt_size);
                tbl[id] = Message{id, txt};
            }
            file.close();
        } else {
            cerr << "unable to open file for reading. " << fname << endl;
        }

        return tbl;
    }
}

int main(int argc, char *argv[]) {
    odds::MsgTbl mt = odds::create_msg_table();

    // where are we saving this file?
    fs::path dest_dir = "C:/Users/chris/.databox";
    fs::path bin_file = dest_dir / "msg_tbl.bin";

    // create the directory if it doesn't exist
    if(!fs::exists(dest_dir)) {
        fs::create_directories(dest_dir);
    }

    // load the message table from a file if it exists
    if(fs::exists(bin_file)) {
        mt = odds::load_table(bin_file.string());
    }

    // check for command line arguments to add custom messages
    if(argc == 3) {
        int id = stoi(argv[1]);    // parse the ID as a hexidecimal number
        string txt = argv[2];                   // message text

        if(odds::add_msg(mt, id, txt)) {
            cout << "message added successfully." << endl;
        } else {
            cout << "failed to add a message. ID must be in the range 0x06 to 0x0F." << endl;
        }
    }

    // save the message table to the binary file.
    odds::save_table(mt, bin_file.string());

    // print the message table
    cout << "current message table:" << endl;
    odds::print_table(mt);


    return 0;
}