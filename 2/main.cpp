#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include "catch.hpp"
#include "queque.h"

void boo(ConsumersProducer<int> &cq) {
    auto foo = [](int v) {
        std::cout << v<<std::endl;
    };
    cq.Subscribe(foo);
}

TEST_CASE("Test producer consumer") {
    ConsumersProducer<int> cq(2);

    std::thread tr1(boo, std::ref(cq));
    std::thread tr2(boo, std::ref(cq));
    cq.Publish(1);
    cq.StopPublishing();
    tr1.join();
    tr2.join();
}