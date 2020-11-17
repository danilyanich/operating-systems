#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <iomanip>
#include <string_view>

using namespace std;

mutex g_lockprint;
constexpr  int no_of_philosophers = 5;

struct fork {
    mutex mutex;
};

struct table {
    atomic<bool> ready{ false };
    array<fork, no_of_philosophers> forks;
};

struct philosopher {
private:
    string const name;
    table const & dinnertable;
    fork& left_fork;
    fork& right_fork;
    thread lifethread;
    mt19937 rng{random_device{}() };
public:
    philosopher(string_view n, table const & t, fork & l, fork & r) : name(n), dinnertable(t), left_fork(l), right_fork(r), lifethread(&philosopher::dine, this)
    {
    }

    ~philosopher()
    {
        lifethread.join();
    }

    void dine() {
        while (!dinnertable.ready);
        do
        {
            think();
            eat();
        } while (dinnertable.ready);
    }

    void print(std::string_view text) {
        lock_guard<std::mutex> cout_lock(g_lockprint);
        cout << left << setw(10) << setfill(' ') << name << text << endl;
    }

    void eat() {
        lock(left_fork.mutex, right_fork.mutex);
        lock_guard<std::mutex> left_lock(left_fork.mutex, adopt_lock);
        lock_guard<std::mutex> right_lock(right_fork.mutex, adopt_lock);
        print(" started eating.");
        static thread_local uniform_int_distribution<> dist(1, 6);
        this_thread::sleep_for(chrono::milliseconds(dist(rng) * 50));
        print(" finished eating.");
    }

    void think() {
        static thread_local uniform_int_distribution<> wait(1, 6);
        this_thread::sleep_for(chrono::milliseconds(wait(rng) * 150));
        print(" is thinking ");
    }
};

void dine() {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Dinner started!" << endl;
    {
        table table;
        array<philosopher, no_of_philosophers> philosophers{
                        {{ "Aristotle", table, table.forks[0], table.forks[1] },
                                { "Platon",    table, table.forks[1], table.forks[2] },
                                { "Descartes", table, table.forks[2], table.forks[3] },
                                { "Kant",      table, table.forks[3], table.forks[4] },
                                { "Nietzsche", table, table.forks[4], table.forks[0] },
                        }};

        table.ready = true;
        this_thread::sleep_for(chrono::seconds(5));
        table.ready = false;
    }
    cout << "Dinner done!" << endl;
}

int main() {
    dine();
    return 0;
}