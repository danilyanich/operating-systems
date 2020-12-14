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
constexpr  int communistsNumber = 5;

struct rifle {
    mutex mutex;
};

struct country {
    atomic<bool> ready{ false };
    array<rifle, communistsNumber> rifles;
};

struct communist {
private:
    string const name;
    country const& russia;
    rifle& firstRifle;
    rifle& secondRifle;
    thread lifethread;
    mt19937 rng{ random_device{}() };
public:
    communist(string_view n, country const& t, rifle& l, rifle& r) : name(n), russia(t), firstRifle(l), secondRifle(r), lifethread(&communist::revolt, this)
    {
    }

    ~communist()
    {
        lifethread.join();
    }

    void revolt() {
        //while (!russia.ready);
        do
        {
            establishCommunism();
            shoot();
        } while (russia.ready);
    }

    void print(std::string_view text) {
        lock_guard<mutex> cout_lock(g_lockprint);
        cout << name << text << endl;
    }

    void shoot() {
        lock(firstRifle.mutex, secondRifle.mutex);
        lock_guard<std::mutex> left_lock(firstRifle.mutex, adopt_lock);
        lock_guard<std::mutex> right_lock(secondRifle.mutex, adopt_lock);
        print(" started shooting reactionaries.");
        static thread_local uniform_int_distribution<> dist(1, 6);
        this_thread::sleep_for(chrono::milliseconds(dist(rng) * 50));
        print(" finished shooting reactionaries.");
    }

    void establishCommunism() {
        static thread_local uniform_int_distribution<> wait(1, 6);
        this_thread::sleep_for(chrono::milliseconds(wait(rng) * 150));
        print(" is raising class consciousness ");
    }
};

void revolt() {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Revolution started!" << endl;
    {
        country country;
        array<communist, communistsNumber> communists{
            {
                { "Marx", country, country.rifles[0], country.rifles[1] },
                { "Stalin", country, country.rifles[1], country.rifles[2] },
                { "Engels", country, country.rifles[2], country.rifles[3] },
                { "Lenin", country, country.rifles[3], country.rifles[4] },
                { "Trotsky", country, country.rifles[4], country.rifles[0] },
            } 
        };
        country.ready = true;
        this_thread::sleep_for(chrono::seconds(5));
        country.ready = false;
    }
    cout << "Communism established!" << endl;
}

int main() {
    revolt();
    system("pause");
    return 0;
}