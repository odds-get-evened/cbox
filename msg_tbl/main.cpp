#include <iostream>
#include <map>
#include <string>
#include <sstream>      // for generating dynamic messages
#include <algorithm>

using namespace std;

namespace odds {
    struct Message {
        int id;     // message id
        string txt; // message
    };

    using MsgTbl = map<int, Message>;

    MsgTbl create_msg_table() {
        MsgTbl msg_tbl;

        // populate the message table
        msg_tbl[0x00] = {0x00, "INIT"};
        msg_tbl[0x01] = {0x01, "INIT_END"};
        msg_tbl[0x02] = {0x02, "START_PROC"};
        msg_tbl[0x03] = {0x03, "STOP_PROC"};
        msg_tbl[0x04] = {0x04, "ERR_OVER_TEMP"};
        msg_tbl[0x05] = {0x05, "ERR_VOLT_DOWN"};

        return msg_tbl;
    }

    string msg_lookup(const MsgTbl &tbl, int id) {
        if(tbl.find(id) != tbl.end()) {
            return tbl.at(id).txt;
        }

        return "ERR_UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    odds::MsgTbl mt = odds::create_msg_table();

    return 0;
}