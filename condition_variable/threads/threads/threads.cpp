// Simple threading example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Import things we need from the standard library
using std::chrono::seconds;
using std::cout;
using std::endl;
using std::ofstream;
using std::this_thread::sleep_for;
using std::thread;
using std::mutex;

//the results could be atomic?
int result;
mutex result_mutex;

bool result_status = false;
std::condition_variable mycondition;


void consumer()
{
	std::unique_lock<mutex> lock(result_mutex);

	while (!result_status) {
		mycondition.wait(lock);
	}

	cout << "finished loop" << endl;
	cout << "result is " << result << endl;

}

void producer() {
	std::unique_lock<mutex>lock(result_mutex);
	result = 42;
	result_status = true;

	mycondition.notify_one();
	//without this the program is stuck in a loop or takes alot longer. Canceled operation due to time it was taking
}

int main(int argc, char *argv[])
{
	// At the moment our program is only running one thread (the initial one the operating system gave us).

	thread myThread(consumer);
	thread myThreadtwo(producer);

	// Now our program is running two threads in parallel (the initial one, and myThread).
	
	//cout << "I am main\n";

	// Wait for myThread to finish.
	myThread.join();
	myThreadtwo.join();

	// Now we just have the initial thread. So it's safe to exit.

	cout << "All done\n";
	sleep_for(seconds(3));

	return 0;
}
