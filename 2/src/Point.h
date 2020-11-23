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


    std::string log_value() {
        std::string record = "Function result: " + std::to_string(this->function_result) + ", calculation time:" + this->function_result_time + " recording time" + this->record_result_time + "\n";
        make_deletable();
        return record;
    }

private:
    void make_deletable() {
        this->deletable = true;
    }
};
