#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

using namespace std;

int visitors_count = 15;
int chairs = 5;
int queue_count = 0;

queue<int> visitors_queue;
mutex m;
condition_variable c_v;

void visitor() 
{
	int i = 0;
	while (i < visitors_count)
	{
		this_thread::sleep_for(chrono::seconds(2));
		cout << "came visitor " << i << endl;

		unique_lock<mutex> lock(m);
		if (queue_count < chairs)
		{
			cout << "visitor " << i << " in queue" << endl;
			queue_count++;
			visitors_queue.push(i);
			c_v.notify_one();
		}
		else
		{
			cout << "no chairs: visitor " << i << " left" << endl;
		}
		lock.unlock();

		i++;
	}
}

void barber() 
{
	while (true) {
		unique_lock<mutex> lock(m);
		if (queue_count == 0)
		{
			cout << "barber sleeping" << endl;
			c_v.wait(lock);
			cout << "barber awake" << endl;
		}
		lock.unlock();

		cout << "starting cuting visitor " << visitors_queue.front() << endl;
		this_thread::sleep_for(chrono::seconds(5));

		lock.lock();
		cout << "visitor " << visitors_queue.front() << " cut" << endl;
		cout << "visitor " << visitors_queue.front() << " left" << endl;
		visitors_queue.pop();
		queue_count--;
		lock.unlock();
	}
}

int main() 
{
	thread stylist(barber);
	thread visitor(visitor);
	stylist.join();
	visitor.join();
	return 0;
}