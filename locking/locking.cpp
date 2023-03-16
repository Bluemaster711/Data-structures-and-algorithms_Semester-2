// Race conditions example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

#include "account.h"

// Import things we need from the standard library
using std::cout;
using std::endl;
using std::thread;

typedef std::chrono::steady_clock the_clock;

Account bill;
//std::mutex bill_m;

void adding() {

	for (int y = 0; y < 1000000; y++) 
	{
		//std::lock_guard<std::mutex>guard(bill_m);
		
		bill.add(17, 29);
		//std::mutex::unlock;
	}
}

//1729.00

void threads() {


	const int THREADS = 10;

	std::vector<std::thread>threadlist;

	threadlist.reserve(THREADS);
	




	for (int i = 0; i < THREADS; i++) {
		threadlist.emplace_back(&adding);
	}

	for (auto& thread : threadlist) {
		thread.join();
	}



}



int main(int argc, char *argv[])
{
	cout << "Initial: " << bill.total() << "\n";


	//the_clock::time_point start = the_clock::now();
	threads();
	//the_clock::time_point end = the_clock::now();

	

	//bill.add(4, 17);
	//bill.add(3, 0);
	//bill.add(10, 99);


	cout << "Total: " << bill.total() << "\n";

	//auto time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	//cout << "Time taken " << time_taken << " nanoseconds." << endl;

	return 0;
}



//first attempt without mutex 32million nanoseconds

//seconds attemp with mutex 400million nanoseconds

//12 times longer