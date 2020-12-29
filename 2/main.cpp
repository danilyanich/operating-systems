#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <cmath>
#include <string>
#include "Point.h"

using namespace std;
condition_variable my_condition_variable;
mutex my_mutex;
bool notified_result_listener = false;
bool notified_logger = false;
bool done = false;
float result = 1;
int current_function_value = 1;
clock_t result_time, record_result_time;
fstream my_file;

void write_to_file(char filename[], std::string result) {
	my_file.open(filename, fstream::in | fstream::out | fstream::app);
	my_file << result << "\n";
	my_file.close();
}

void result_writer() {
	const char filename[] = "../results.txt";
	my_file.open(filename, fstream::in | fstream::out | fstream::trunc);
	my_file.close();
	

	unique_lock<mutex> lock(my_mutex);

	while (!done) {
		while (notified_result_listener) {
			write_to_file(const_cast<char*>(filename), to_string(result));
			record_result_time = clock();
			notified_result_listener = false;
			notified_logger = true;
			my_condition_variable.notify_all();
		}
		my_condition_variable.wait(lock);
	}
}

void log_writer() {
	const char filename[] = "../log.txt";
	my_file.open(filename, fstream::in | fstream::out | fstream::trunc);
	my_file.close();
	

	unique_lock<mutex> lock(my_mutex);

	while (!done) {
		while (notified_logger) {
			notified_logger = false;
			shared_ptr<Point> current_point(new Point(result, to_string(((float)result_time) / CLOCKS_PER_SEC), to_string(((float)record_result_time) / CLOCKS_PER_SEC)));
			write_to_file(const_cast<char*>(filename), current_point->log_value());
		}
		my_condition_variable.wait(lock);
	}
}

void exponent_function(int value) {
	for (current_function_value = 1; current_function_value <= value; current_function_value++) {
		this_thread::sleep_for(chrono::seconds(1));
		result = exp(current_function_value);
		result_time = clock();
		notified_result_listener = true;
		my_condition_variable.notify_all();
	}

	done = true;
	my_condition_variable.notify_all();
}

int main() {
	thread exponent(exponent_function, 10);
	thread func_writer(result_writer);
	thread log(log_writer);

	exponent.join();
	func_writer.join();
	log.join();

	return 0;
}