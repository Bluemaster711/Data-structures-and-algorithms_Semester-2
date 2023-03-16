#pragma once

#ifndef MANDELBROTFARM_H
#define MANDELBROTFARM_H
#include <string>
#include <mutex>

#include "mandelbrot.h"
#include "mandelbrotask.h"


		/** Task that prints a message to cout. */
class mandelbrotask : public Task
{
public:

	//uint32_t image[HEIGHT][WIDTH];
	uint32_t** locImage;

	mandelbrotask(float& left, float& right, float& top, float& bottom, int& line, uint32_t** image) :
		left_(left), right_(right), top_(top), bottom_(bottom), line_(line), locImage(image) {};

	void run();

private:


	float left_;
	float right_;
	float top_;
	float bottom_;
	int line_;

};

#endif