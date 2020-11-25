#pragma warning(disable : 4996)

#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>

#include "Logger.h"

using namespace std;



bool done = false;
int curr_func_value = 1;
bool notifier_of_data_listener = false;
bool logger = false;
int curr_arg_value = 1;
std::fstream function_result;
std::condition_variable cv;
std::mutex cv_m;



void print_func_result(char filename[], std::string result) {
    function_result.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    function_result << result << "\n";
    function_result.close();
}

void file_in(char filename[]) {
    function_result.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    function_result.close();
}

void result_listener() {
    const char filename[] = "../results.txt";

    file_in(const_cast<char*>(filename));

    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (notifier_of_data_listener) {
            print_func_result(const_cast<char*>(filename), std::to_string(curr_func_value));

            notifier_of_data_listener = false;
            logger = true;
            cv.notify_all();
        }
        cv.wait(lock);
    }
}







void log_writer() {
    const char filename[] = "../log.txt";
    file_in(const_cast<char*>(filename));

    std::unique_lock<std::mutex> lock(cv_m);

    while (!done) {
        while (logger) {
            logger = false;
            time_t now = time(0);
            std::string initialized_time = ctime(&now);
            std::shared_ptr<Logger> current_point(new Logger(curr_arg_value - 1, curr_func_value, initialized_time));
            print_func_result(const_cast<char*>(filename), current_point->log_value());
        }
        cv.wait(lock);
    }
}

void factorial(int n) {
    for (; curr_arg_value <= n; curr_arg_value++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(cv_m);

        curr_func_value *= curr_arg_value;

        notifier_of_data_listener = true;
        cv.notify_all();
    }

    done = true;
    cv.notify_all();
}

int main() {
    thread factorial(factorial, 5);
    thread result(result_listener);
    thread log(log_writer);
   

    factorial.join();
    result.join();
    log.join();

    cout << "done";
    return 0;
}
