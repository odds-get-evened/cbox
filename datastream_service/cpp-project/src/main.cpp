#include <iostream>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

// generates random float values in a defined range
double gen_rand_val(double min, double max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dist(min, max);

    return dist(gen);
}

// simulated sensor data stream
void stream_sensor_data() {
    while(true) {
        // generate simulated temp and humidity
        double temp = gen_rand_val(15.0, 35.0);      // celsius
        double humid = gen_rand_val(30.0, 80.0);      // percentage

        cout << "temp: " << temp << "C; humidity: " << humid << "%" << endl;

        // delay stream
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main(int argc, char *argv[]) {
    thread sensor_thread(stream_sensor_data);
    // let it run indefinitely (or modify to stop after a condition)
    sensor_thread.join();

    return 0;
}