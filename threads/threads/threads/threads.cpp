// Simple threading example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// Import things we need from the standard library
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::ofstream;
using std::this_thread::sleep_for;
using std::thread;
using std::ref;


struct threadArgs {

	int id;
	int delay;

};


void myThreadFunc(threadArgs *Args)
{
	//sleep_for(seconds(3));
	//cout << "This is the first message, \n";

	for (int i = 0; i < 10; i++) {

		cout << Args->id << endl;


		sleep_for(milliseconds(Args->delay));


	}

}


int main(int argc, char *argv[])
{
	
	std::vector<thread>threadlist; //1-5


	for (int x = 0; x < 5; x++ ) {

		threadArgs* Athread = new threadArgs;

		Athread->id = x;
		Athread->delay = 200;

		threadlist.emplace_back(myThreadFunc, Athread);

	}

	for (auto& thread : threadlist) {


		thread.join();


	}





	// At the moment our program is only running one thread (the initial one the operating system gave us).

	//thread myThread(myThreadFunc, Athread);

	// Now our program is running two threads in parallel (the initial one, and myThread).
	
	//cout << "I am main\n";

	// Wait for myThread to finish.

	//cout << Athread->id << endl;

	// Now we just have the initial thread. So it's safe to exit.

	//cout << "All done\n";
	//sleep_for(seconds(3));

	return 0;
}
