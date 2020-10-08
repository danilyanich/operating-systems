#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <chrono>
#include "catch.hpp"
#include "consumer_producer.h"
#include <fstream>
#include <iostream>


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

void subscribe(ConsumersProducer<Point> &cq, ConsumersProducer<PointWithSysInfo> &cq2, std::ostream &os) {
    auto foo = [&cq2, &os](Point &v) {
        PointWithSysInfo pointWithSysInfo(v);
        pointWithSysInfo.accept_time = std::chrono::system_clock::now();
        os << v.y << " " << v.x << "\n";
        pointWithSysInfo.write_time = std::chrono::system_clock::now();
        cq2.Publish(pointWithSysInfo);
    };
    cq.Subscribe(foo);
}

void functionValuesProducer(ConsumersProducer<Point> &cp) {
    for (int i = 0; i < 10; i++) {
        Point p{};
        p.x = i;
        p.y = 10 * i * i;
        cp.Publish(p);
    }
}

void write_(ConsumersProducer<PointWithSysInfo> &cq, std::ostream &os) {
    auto foo = [&os](PointWithSysInfo &v) {
        auto accept_time = std::chrono::system_clock::to_time_t(v.accept_time);
        auto write_time = std::chrono::system_clock::to_time_t(v.write_time);
        os << "point: y: " << v.p.y << ", x: " << v.p.x << ", accept time: " << std::ctime(&accept_time)
           << "write to file time: "
           << std::ctime(&write_time) << "\n";
    };
    cq.Subscribe(foo);
}

TEST_CASE("Test producer consumer") {
    ConsumersProducer<Point> cp(1);

    ConsumersProducer<PointWithSysInfo> cp2(1);

    std::ofstream of("/home/illfate/CLionProjects/operating-systems/2/1.txt");
    std::thread tr1(subscribe, std::ref(cp), std::ref(cp2), std::ref(std::cout));
    std::thread tr2(write_, std::ref(cp2), std::ref(of));
    functionValuesProducer(cp);
    cp.StopPublishing();
    tr1.join();
    cp2.StopPublishing();
    tr2.join();
}