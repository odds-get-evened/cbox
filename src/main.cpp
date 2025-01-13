#include <iostream>
#include <CLI/CLI.hpp>

using namespace std;

int main(int argc, char *argv[]) {
    CLI::App app {"my app"};

    int val = 0;
    app.add_option("-v, --value", val, "a value");

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        return app.exit(e);
    }

    cout << "value: " << val << endl;

    return 0;
}