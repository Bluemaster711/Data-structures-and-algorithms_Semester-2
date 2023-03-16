#include "farm.h"
#include <thread>
#include <queue>
#include <mutex>
#include <vector>

// FIXME - You may need to add #includes here (e.g. <thread>)

void Farm::add_task(Task* task)
{
	//farm_m.lock();
	mytasks.push(task); //push task into queue
	//farm_m.unlock();
	
}
void Farm::run()
{

	// FIXME - Implement this
	const uint8_t CPU_threads = std::thread::hardware_concurrency();
	//collection of threads
	std::vector<std::thread*>threadie;


	//loop through the cpu list
	for (int i = 0; i < CPU_threads; i++) {

		//grab task and thread
		threadie.push_back(new std::thread([&] {
			while (true)
			{
				//if no tasks leave
				if (mytasks.empty())
					return;

				//protect the resoure so no duplicates are done
				farm_m.lock();
				//grab task with a thread
				Task* t = mytasks.front();
				//delete task from list
				mytasks.pop();

				farm_m.unlock();

				//run thread on task
				t->run();
				//delete "current" thread
				delete t;
			}
			}));

		//join the vector of threads with the task
		threadie[i]->join();

	}
	

}
