#include "mandelbrot.h"
#include "mandelbrotask.h"
#include "mandelbrotfarm.h"
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <iostream>


void Farm::add_task(Task* task) {
	
	//add task to the queue
	aqueue.push(task);

}

void Farm::run() {


	//define cpu's
	const uint8_t CPU = std::thread::hardware_concurrency();
	std::cout << (uint16_t)CPU << " Threads Used" << std::endl;
	//const uint8_t CPU = 2;

	//created vector to add threads too
	std::vector<std::thread*>athread;

	for (int cont = 0; cont < CPU; cont++) {

		//add thread to list
		athread.push_back(new std::thread([&] {

		/*	if (aqueue.empty()) {
				return;
			}*/
			//else {

			//	//protect the shared queue - to avoid duplicate tasks
			//	farm_m.lock();
			//	//take the task
			//	Task* t = aqueue.front();
			//	//remove task
			//	aqueue.pop();
			//	//allow other threads to take a task
			//	farm_m.unlock();

			//	//run the tread
			//	t->run();
			//	delete t;

			//}

			while (true) {
				//return if no task's left
				if (aqueue.empty())
					return;

				//protect the shared queue - to avoid duplicate tasks
				farm_m.lock();
				//take the task
				Task* t = aqueue.front();
				//remove task
				aqueue.pop();
				//allow other threads to take a task
				farm_m.unlock();

				//run the tread
				t->run();
				delete t;
			}
		

			}));


		//join all threads
		athread[cont]->join();

	
	

	

	}

}