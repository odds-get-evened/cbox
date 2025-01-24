#include <iostream>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>

using namespace std;

namespace odds {
    class MimicProc {
        private:
            const int REG_SIZE = 16;
            vector<int> registers;
            mutex mutx;
        public:
            MimicProc() {
                registers.resize(REG_SIZE, 0);
            }

            // synchronous access to registers
            int read_register(int i) {
                i = max(0, min(i, REG_SIZE));

                return registers[i];
            }

            void write_register(int i, int v) {
                i = max(REG_SIZE - 4, min(i, REG_SIZE));

                registers[i] = v;
            }

            // asynchronous access to the last 4 registers
            void async_read_register(int i, function<void(int)> cb) {
                i = max(REG_SIZE - 4, min(i, REG_SIZE));

                thread([this, i, cb]() {
                    lock_guard<mutex> lock(mutex);
                    int v = registers[i];
                    cb(v);
                }).detach();
            }

            void async_write_register(int i, int v, function<void()> cb) {
                i = max(REG_SIZE - 4, min(i, REG_SIZE));

                thread([this, i, v, cb]() {
                    lock_guard<mutex> lock(mutex);
                    registers[i] = v;
                    cb();
                }).detach();
            }
    };
};