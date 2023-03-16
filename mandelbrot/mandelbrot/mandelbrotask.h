#pragma once

#ifndef MENDELBROTASK_H
#define MENDELBROTASK_H

#include "mandelbrot.h"
#include <queue>
#include <thread>
#include <mutex>
#include <complex>

class Farm{

public:

	std::mutex farm_m;

	void add_task(Task *task);

	void run();

	//extern "C" uint32_t image[HEIGHT][WIDTH];
	//uint32_t image[HEIGHT][WIDTH];

private:

	std::queue<Task*>aqueue;



};

#endif
