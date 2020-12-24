#include <iostream>
#include <thread>
#include<random>
#include <mutex>
#include <string>
using namespace std;
int fork = 5;
std::mutex eating_mutex;
std::mutex thinking_mutex;
void think(string name) {
    thinking_mutex.lock();
    srand(0);
    cout << name << " is thinking\n";
    thinking_mutex.unlock();
    for (int i = 0; i < rand() % 100000000 + 100000000; i++) {
        //Great thinks do not interrupt
    }
}

[[noreturn]] void eat(string name) {
    srand(0);

        eating_mutex.lock();
        if(fork>1){
            fork-=2;
            eating_mutex.unlock();


        }else{
            while(fork==1){

            }
            fork-=2;
            eating_mutex.unlock();
        }
        cout<<name<<" is eating\n";
        for(int i=0;i<rand()%50000000+50000000;i++);
        fork+=2;


}
[[noreturn]] void run(string name){
while (1){
    think(name);
    eat(name);
}
}



int main()
{
    thread first(run,"Aristotel");
    thread second(run,"Socrat");
    thread third(run,"Diogen");
    thread fourth(run,"Pythagor");
    thread fifth(run,"Arhimed");
    first.join();
    second.join();
    third.join();
    fourth.join();
    fifth.join();


}
