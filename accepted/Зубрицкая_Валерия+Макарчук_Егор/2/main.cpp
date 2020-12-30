#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>

#include "Point.h"

std::condition_variable cv;
std::mutex mutex;
bool notified_result_listener = false;
bool notified_logger = false;
bool done = false;
int function_result = 1;
int current_function_value = 1;
clock_t function_result_time, record_result_time;
std::fstream result_file;

void init_file(char filename[]) {
    result_file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    result_file.close();
}

void record_to_file(char filename[], std::string result) {
    result_file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    result_file << result << "\n";
    result_file.close();
}

void result_writer() {
    const char filename[] = "../results.txt";

    init_file(const_cast<char*>(filename));

    std::unique_lock<std::mutex> lock(mutex);

    while (!done) {
        while (notified_result_listener) {
            record_to_file(const_cast<char*>(filename), std::to_string(function_result));
            record_result_time = clock();
            notified_result_listener = false;
            notified_logger = true;
            cv.notify_all();
        }
        cv.wait(lock);
    }
}

void log_writer() {
    const char filename[] = "../log.txt";
    init_file(const_cast<char*>(filename));

    std::unique_lock<std::mutex> lock(mutex);

    while (!done) {
        while (notified_logger) {
            notified_logger = false;
            std::shared_ptr<Point> current_point(new Point(function_result, std::to_string(((float)function_result_time) / CLOCKS_PER_SEC), std::to_string(((float)record_result_time) / CLOCKS_PER_SEC)));
            record_to_file(const_cast<char*>(filename), current_point->log_value());
        }
        cv.wait(lock);
    }
}

void square_function(int digit) {
    for (; current_function_value <= digit; current_function_value++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        function_result = pow(current_function_value, 2);
        function_result_time = clock();
        notified_result_listener = true;
        cv.notify_all();
    }

    done = true;
    cv.notify_all();
}

int main() {
    std::thread function(square_function, 10);
    std::thread result_of_function(result_writer);
    std::thread log(log_writer);

    function.join();
    result_of_function.join();
    log.join();

    std::cout << "Congrats, it's worked!";
    return 0;
}
