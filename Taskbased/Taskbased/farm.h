#pragma once
#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

// FIXME - You will need to add #includes here (probably <mutex> at least)

/** A collection of tasks that should be performed in parallel. */
class Farm {

public:

	std::mutex farm_m;

	// DO NOT CHANGE the public interface of this class.
	// You only need to implement the existing methods.

	/** Add a task to the farm.
		The task will be deleted once it has been run. */
	void add_task(Task* task);

	/** Run all the tasks in the farm.
		This method only returns once all the tasks in the farm
		have been completed. */
	void run();

private:
	// FIXME - You will need to add private member variables here



	std::queue<Task*>mytasks;

	float empty;
	float front;
	float push;
	float pop;



};

#endif