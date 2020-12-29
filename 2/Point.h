#include <string>
#include <iomanip>
#include <iostream>
using namespace std;
class Point {
public:
	float result;
	bool isdeletable = false;
	string result_time;
	string record_time;

	Point(float result, string result_time, string record_time) {
		this->result = result;
		this->result_time = result_time;
		this->record_time = record_time;
	}

	string log_value() {
		string record = "Exp = " + to_string(this->result) + " result time = " + this->result_time + " record time =" + this->record_time + " sec\n";
		make_deletable();
		return record;
	}

private:
	void make_deletable() {
		this->isdeletable = true;
	}
};