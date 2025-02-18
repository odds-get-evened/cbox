#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include <iostream>
#include <string>
#include "inc/outils.hpp"

using namespace std;
using namespace Poco::Net;

namespace odds {
    class OMessageConnection : public TCPServerConnection {
    public:
        OMessageConnection(const StreamSocket &s) : TCPServerConnection(s) {}

        void run() override {
            StreamSocket &ss = socket();
            SocketStream str(ss);
            string msg;
            
            while(true) {
                getline(str, msg);

                if(odds::startswith(msg, "hi")) {
                    str << "00000 hello there!" << endl;
                } else if(msg == "quit" || msg == "bye" || msg == "exit") {
                    str << "X0000 disconnecting..." << endl;
                    break;
                } else {
                    str << "E0000 unknown command" << endl;
                }
            }
        }
    };
}

int main(int argc, char **argv) {
    ServerSocket svs(8008);
    TCPServer srv(new TCPServerConnectionFactoryImpl<odds::OMessageConnection>(), svs);
    srv.start();
    cout << "server started on port 8008" << endl;

    // keep the server running
    while(true) {
        Poco::Thread::sleep(1000);
    }

    return 0;
}