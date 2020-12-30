
#ifndef INC_2_FUNCTION_CALCULATOR_H
#define INC_2_FUNCTION_CALCULATOR_H

#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <ctime>
#include "consumer_producer.h"

struct Point {
    double x;
    double y;
};

struct PointWithSysInfo {
    explicit PointWithSysInfo(Point p) : point(p) {}

    Point point;
    std::chrono::system_clock::time_point accept_time;
    std::chrono::system_clock::time_point write_time;
};

void functionValuesProducer(const std::function<void(const Point &p)> &func) {
    for (int i = 0; i < 10; i++) {
        Point p{};
        p.x = i;
        p.y = 3 * i * i * i;
        func(p);
    }
}

class FunctionValuesCalculator {
    ConsumersProducer<Point> calculator = ConsumersProducer<Point>(1);
    ConsumersProducer<PointWithSysInfo> sys_info = ConsumersProducer<PointWithSysInfo>(1);
    std::thread values_writer;
    std::thread logging_tr;
    std::function<void(std::function<void(const Point &p)>)> calc_function;
public:

    explicit FunctionValuesCalculator(std::function<void(std::function<void(const Point &p)>)> calc_function)
            : calc_function(std::move(calc_function)) {
    }

    void Process(std::ostream &value_write_os = std::cout, std::ostream &logging_write_os = std::cout) {
        values_writer = std::thread(calculatorSubscribe, std::ref(calculator), std::ref(sys_info),
                                    std::ref(value_write_os));
        logging_tr = std::thread(logging, std::ref(sys_info), std::ref(logging_write_os));
        calc_function([&](const Point &p) {
            calculator.Publish(p);
        });
        calculator.StopPublishing();
        values_writer.join();
        sys_info.StopPublishing();
        logging_tr.join();
    }

private:

    static void
    calculatorSubscribe(ConsumersProducer<Point> &subscribe,
                        ConsumersProducer<PointWithSysInfo> &publish,
                        std::ostream &os) {
        auto foo = [&publish, &os](Point &v) {
            PointWithSysInfo pointWithSysInfo(v);
            pointWithSysInfo.accept_time = std::chrono::system_clock::now();
            os << v.y << " " << v.x << "\n";
            pointWithSysInfo.write_time = std::chrono::system_clock::now();
            publish.Publish(pointWithSysInfo);
        };
        subscribe.Subscribe(foo);
    }

    static void logging(ConsumersProducer<PointWithSysInfo> &cq, std::ostream &os) {
        auto foo = [&os](PointWithSysInfo &v) {
            auto accept_time = std::chrono::system_clock::to_time_t(v.accept_time);
            auto write_time = std::chrono::system_clock::to_time_t(v.write_time);
            os << "point: y: " << v.point.y << ", x: " << v.point.x << ", accept time: " << std::ctime(&accept_time)
               << "write to file time: "
               << std::ctime(&write_time) << "\n";
        };
        cq.Subscribe(foo);
    }
};


#endif //INC_2_FUNCTION_CALCULATOR_H
