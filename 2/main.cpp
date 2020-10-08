//
// Created by illfate on 10/8/20.
//

#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include "consumer_producer.h"


struct Point {
    double x;
    double y;
};

struct PointWithSysInfo {
    explicit PointWithSysInfo(Point p) : p(p) {}

    Point p;
    std::chrono::system_clock::time_point accept_time;
    std::chrono::system_clock::time_point write_time;
};


class FunctionValuesCalculator {
    ConsumersProducer<Point> calculator = ConsumersProducer<Point>(1);
    ConsumersProducer<PointWithSysInfo> sys_info = ConsumersProducer<PointWithSysInfo>(1);
    std::thread values_writer;
    std::thread logging;
public:
    FunctionValuesCalculator() = default;

    void Process() {
        std::ofstream of("/home/illfate/CLionProjects/operating-systems/2/1.txt");

        values_writer = std::thread(subscribe, std::ref(calculator), std::ref(sys_info), std::ref(of));
        logging = std::thread(write_, std::ref(sys_info), std::ref(std::cout));
        functionValuesProducer(calculator);
        calculator.StopPublishing();
        values_writer.join();
        sys_info.StopPublishing();
        logging.join();
    }

private:

    static void subscribe(ConsumersProducer<Point> &cq, ConsumersProducer<PointWithSysInfo> &cq2, std::ostream &os) {
        auto foo = [&cq2, &os](Point &v) {
            PointWithSysInfo pointWithSysInfo(v);
            pointWithSysInfo.accept_time = std::chrono::system_clock::now();
            os << v.y << " " << v.x << "\n";
            pointWithSysInfo.write_time = std::chrono::system_clock::now();
            cq2.Publish(pointWithSysInfo);
        };
        cq.Subscribe(foo);
    }

    static void functionValuesProducer(ConsumersProducer<Point> &cp) {
        for (int i = 0; i < 10; i++) {
            Point p{};
            p.x = i;
            p.y = 10 * i * i;
            cp.Publish(p);
        }
    }

    static void write_(ConsumersProducer<PointWithSysInfo> &cq, std::ostream &os) {
        auto foo = [&os](PointWithSysInfo &v) {
            auto accept_time = std::chrono::system_clock::to_time_t(v.accept_time);
            auto write_time = std::chrono::system_clock::to_time_t(v.write_time);
            os << "point: y: " << v.p.y << ", x: " << v.p.x << ", accept time: " << std::ctime(&accept_time)
               << "write to file time: "
               << std::ctime(&write_time) << "\n";
        };
        cq.Subscribe(foo);
    }
};

int main() {
    FunctionValuesCalculator calculator = FunctionValuesCalculator();
    calculator.Process();
}