#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include <iostream>
#include <string>
#include "inc/outils.hpp"

using namespace std;

int main(int argc, char **argv) {
    try {
        Poco::Net::StreamSocket sock;
        sock.connect(Poco::Net::SocketAddress("127.0.0.1", 8008));

        Poco::Net::SocketStream str(sock);

        string cmd;
        while(true) {
            cout << "enter a command: ";
            getline(cin, cmd);

            str << cmd << endl;

            string res;
            getline(str, res);
            cout << "response from server: " << res << endl;

            // if exit code is given break loop and exit client
            if(odds::startswith(res, "X0000")) {
                break;
            }
        }
    } catch(Poco::Exception &e) {
        cerr << "error: " << e.displayText() << endl;
    }

    return 0;
}