#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

const unsigned int VISITOR_COUNT = 20;
const unsigned int SEATS_COUNT = 5;
const unsigned int INIT_ID = 1;
const unsigned int MAX_TIME_BETWEEN_VISITORS = 4;
const unsigned int TIME_WORK_HAIRDRESSER = 2;

struct visitor {
    unsigned int visitorID = 0;
};

vector<visitor> visitors;
vector<int> clocks;

int counter = 0;
unsigned int actualVisitorID = INIT_ID;
bool work = true;

void clocksInit();

auto hairdresser = std::async([]() {
    //std::this_thread::sleep_for(5s);
    unsigned int visitorID = INIT_ID;
    for (;;) {
        if (counter <= 0) {
            if (work == true) std::this_thread::sleep_for(0.1s);
            else break;
        } else if (counter > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(TIME_WORK_HAIRDRESSER));
            int j = 0;
            for (; j < visitors.size(); j++) {
                if (visitorID == visitors[j].visitorID) break;
            }
            cout << "                                           Served " << visitors[j].visitorID << " visitor" << endl;
            visitors.erase(visitors.begin() + j);
            visitorID++;
            counter--;
        }
    }
    cout << endl << "                    Stop work" << endl;
});

auto flowOfVisitors = std::async([]() {
    unsigned int actualNumber = 0;
    for (int &clock : clocks) {
        std::this_thread::sleep_for(std::chrono::seconds(clock));
        if (counter < SEATS_COUNT) {
            visitor v = {actualVisitorID};
            visitors.push_back(v);
            actualVisitorID++;
            actualNumber++;
            counter++;
            cout << actualNumber << " Visitor " << v.visitorID << ", expect " << counter << endl;
        } else {
            actualNumber++;
            cout << actualNumber << " All seats are taken, visitor is gone" << endl;
        }
    }
    work = false;
});

int main(int argc, char **arg) {
    clocksInit();
    flowOfVisitors.get();
    hairdresser.get();

    system("pause");
    return 0;
}


void clocksInit() {
    for (int i = 0; i < VISITOR_COUNT; clocks.push_back(rand() % MAX_TIME_BETWEEN_VISITORS), i++);
}