#include "mandelbrot.h"
#include "mandelbrotask.h"
#include "mandelbrotfarm.h"
#include <iostream>
#include <complex>

const int WIDTH = 3840;
const int HEIGHT = 2160;

const int MAX_ITERATIONS = 1000;

// extern "C" uint32_t image[HEIGHT][WIDTH];

void mandelbrotask::run() {

	//uint32_t image[HEIGHT][WIDTH]{};

	for (unsigned x = 0; x < WIDTH; x++)
	{

		std::complex<float> c(left_ + (x * (right_ - left_) / WIDTH),
			top_ + (line_ * (bottom_ - top_) / HEIGHT));

		std::complex<float> z(0.0, 0.0);

		int iterations = 0;
		while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
		{
			z = (z * z) + c;
			++iterations;
		}
		if (iterations == MAX_ITERATIONS)
		{

			locImage[line_][x] = 0x000000; // black
		}
		else
		{


			if (iterations <= 25)
			{
				locImage[line_][x] = 0xFF0000;
			}
			else if (iterations <= 50)
			{
				locImage[line_][x] = 0xFE6904;
			}
			else if (iterations <= 75)
			{
				locImage[line_][x] = 0xFEE104;
			}
			else if (iterations <= 100)
			{
				locImage[line_][x] = 0xB6FE04;
			}
			else if (iterations <= 125)
			{
				locImage[line_][x] = 0x13FE04;
			}
			else if (iterations <= 150)
			{
				locImage[line_][x] = 0x04FEA8;
			}
			else if (iterations <= 175)
			{
				locImage[line_][x] = 0x04E1FE;
			}
			else if (iterations <= 200)
			{
				locImage[line_][x] = 0x045BFE;
			}
			else if (iterations <= 250)
			{
				locImage[line_][x] = 0x0904FE;
			}
			else if (iterations <= 300)
			{
				locImage[line_][x] = 0x9404FE;
			}
			else if (iterations <= 400)
			{
				locImage[line_][x] = 0xFE04E1;
			}
			else if (iterations <= 450)
			{
				locImage[line_][x] = 0xFE047C;
			}
			else
			{
				locImage[line_][x] = 0xFE0404;
			}
		}
	}
	return;
}

