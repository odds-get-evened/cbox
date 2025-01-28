#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

using ip::tcp;

int main(int argc, char *argv[]) {
    try {
        io_context io_ctx;

        tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), 8080));
        cout << "server is running. waiting for connections..." << endl;

        tcp::socket socket(io_ctx);
        acceptor.accept(socket);
        cout << "clinet connected." << endl;

        for(;;) {
            char data[128];
            boost::system::error_code err_code;
            size_t len = socket.read_some(buffer(data), err_code);

            if(err_code == error::eof) {
                break;      // connection closed cleanly by peer
            } else if(err_code) {
                throw boost::system::system_error(err_code);
            }

            cout.write(data, len);
            cout << endl;
        }
    } catch(exception &e) {
        cerr << "exception: " << e.what() << endl;
    }    
    
    return 0;
}