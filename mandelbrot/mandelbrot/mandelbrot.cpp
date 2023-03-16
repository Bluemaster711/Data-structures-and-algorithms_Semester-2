
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <string>

#include "mandelbrot.h"
#include "mandelbrotask.h"
#include "mandelbrotfarm.h"

// Import things we need from the standard libraries.
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;
using std::thread;

// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;


// Image size
const int WIDTH = 3840;
const int HEIGHT = 2160;

// The number of times to iterate before we assume that a point isn't in the
// Mandelbrot set.
// (You may need to turn this up if you zoom further into the set.)
const int MAX_ITERATIONS = 1000;

// The image data.
// Each pixel is represented as 0xRRGGBB.
// uint32_t image[HEIGHT][WIDTH];


// Write the image to a TGA file with the given name.
// Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
void write_tga(const char *filename, uint32_t** image)
{
	ofstream outfile(filename, ofstream::binary);

	uint8_t header[18] = {
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};
	outfile.write((const char *)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] = {
				image[y][x] & 0xFF, // blue channel
				(image[y][x] >> 8) & 0xFF, // green channel
				(image[y][x] >> 16) & 0xFF, // red channel
			};
			outfile.write((const char *)pixel, 3);
		}
	}

	outfile.close();
	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		cout << "Error writing to " << filename << endl;
		exit(1);
	}
}


//void compute_thred(float left, float right, float top, float bottom, unsigned start, unsigned end) {
//	for (unsigned y = start; y < end; y++) {
//		//for (unsigned x = 0; x < WIDTH; x++)
//		//{
//		//	// Work out the point in the complex plane that
//		//		// corresponds to this pixel in the output image.
//		//	complex<float> c(left + (x * (right - left) / WIDTH),
//		//		top + (y * (bottom - top) / HEIGHT));
//		//	// Start off z at (0, 0).
//		//	complex<float> z(0.0, 0.0);
//		//	// Iterate z = z^2 + c until z moves more than 2 units
//		//	// away from (0, 0), or we've iterated too many times.
//		//	int iterations = 0;
//		//	while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
//		//	{
//		//		z = (z * z) + c;
//		//		++iterations;
//		//	}
//		//	if (iterations == MAX_ITERATIONS)
//		//	{
//		//		// z didn't escape from the circle.
//		//		// This point is in the Mandelbrot set.
//		//		image[y][x] = 0x000000; // black
//		//	}
//		//	else
//		//	{
//		//		// z escaped within less than MAX_ITERATIONS
//		//		// iterations. This point isn't in the set.
//		//		//image[y][x] = 0x89CFF0; // blue or could be white 0xFFFFFF
//
	/*			if (iterations <= 25)
				{
					image[y][x] = 0xFF0000;
				}
				else if (iterations <= 50)
				{
					image[y][x] = 0xFE6904;
				}
				else if (iterations <= 75)
				{
					image[y][x] = 0xFEE104;
				}
				else if (iterations <= 100)
				{
					image[y][x] = 0xB6FE04;
				}
				else if (iterations <= 125)
				{
					image[y][x] = 0x13FE04;
				}
				else if (iterations <= 150)
				{
					image[y][x] = 0x04FEA8;
				}
				else if (iterations <= 175)
				{
					image[y][x] = 0x04E1FE;
				}
				else if (iterations <= 200)
				{
					image[y][x] = 0x045BFE;
				}
				else if (iterations <= 250)
				{
					image[y][x] = 0x0904FE;
				}
				else if (iterations <= 300)
				{
					image[y][x] = 0x9404FE;
				}
				else if (iterations <= 400)
				{
					image[y][x] = 0xFE04E1;
				}
				else if (iterations <= 450)
				{
					image[y][x] = 0xFE047C;
				}
				else
				{
					image[y][x] = 0xFE0404;
				}*/
//		//	}
//		//}
//	}
//}
//
//
//// Render the Mandelbrot set into the image array.
//// The parameters specify the region on the complex plane to plot.
//void compute_mandelbrot(float left, float right, float top, float bottom, float start, float end)
//{
//	//amount of threads to use
//	const int THREADS = 10;
//
//	//create vector of threads by rendering_threads
//	std::vector<thread>render_threads;
//	//researve created threads
//	render_threads.reserve(THREADS);
//
//	//loop through compute_mandelbrot set
//	for (unsigned y = 0; y < HEIGHT; y += HEIGHT / THREADS) {
//		render_threads.emplace_back(&compute_thred, left, right, top, bottom, y, y + HEIGHT / THREADS);
//	}
//	for (auto& thread : render_threads) {
//		thread.join();
//	}
//}


int main(int argc, char *argv[])
{
	cout << "Please wait..." << endl;


	// This shows the whole set.
	//compute_mandelbrot(-2.0, 1.0, 1.125, -1.125, 200, 400);
	//compute_mandelbrot(-2.0, 1.0, 1.125, -1.125, 0, 1200); //takes 1215809700ns
	//
	//took 1740ms to finsh
	// getting 2994ms to finish next day
	//compute_mandelbrot(-1.0, 2.3, 2.0, -1.50);
	//took 969ms to finish

	// This zooms in on an interesting bit of detail.
	//compute_mandelbrot(-0.751085, -0.734975, 0.118378, 0.134488, 0, HEIGHT);

	//define the coordinates
	float left = -2.0;
	float right = 1.0;
	float top = 1.125;
	float bottom = -1.125;

	// [HEIGHT][WIDTH]
	//creating an array of pointer, pointing towards an array of pointers 
	uint32_t** image = new uint32_t* [HEIGHT];
	//loop through and add width pointer to image[i] pointer
	for (int i = 0; i < HEIGHT; i++) {
		image[i] = new uint32_t[WIDTH];
		//image[1] = image[1] -> [width]
	}

	//create farm f
	Farm f;

	//loop though the hright adding a task - using the coordinates
	for (int i = 0; i < HEIGHT; i++) {
		f.add_task(new mandelbrotask(left, right, top, bottom, i, (uint32_t**)image));
	}


	// Start timing
	the_clock::time_point start = the_clock::now();


	//run
	f.run();


	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	//would nanoseconds be more accuate?

	//could use nanoseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

	write_tga("nope.tga", (uint32_t**)image);

	return 0;
}
