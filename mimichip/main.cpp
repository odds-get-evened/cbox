#include "inc/mimiproc.hpp"

using namespace std;
using namespace odds;

int main(int argc, char *argv[]) {
    MimicProc proc;

    // sync access
    proc.write_register(0, 42);
    cout << "register 0: " << proc.read_register(0) << endl;

    // async access
    proc.async_write_register(12, 84, []() {
        cout << "async write complete" << endl;
    });

    proc.async_read_register(12, [](int v) {
        cout << "async read value: " << v << endl;
    });

    // give time for async ops to complete
    this_thread::sleep_for(chrono::seconds(1));

    return 0;
}