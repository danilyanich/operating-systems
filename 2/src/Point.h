class Point {
public:
    int function_result;
    bool deletable = false;
    std::string function_result_time;
    std::string record_result_time;

    Point(int result_value, std::string function_result_time, std::string record_result_time) {
        this->function_result = result_value;
        this->function_result_time = function_result_time;
        this->record_result_time = record_result_time;
    }



};
