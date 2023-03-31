#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>
#include <string>
#include <vector>

#include <amp.h>
#include <amp_math.h>

#include "mandelbrot_GPGPU.h"


// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;

using namespace concurrency;

// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;

// The number of times to iterate before we assume that a point isn't in the
// Mandelbrot set.
// (You may need to turn this up if you zoom further into the set.)
const int MAX_ITERATIONS = 1000;


Mandelbrot::Mandelbrot() {
	image = std::vector<uint32_t>(HEIGHT * WIDTH, 0);
}


// using our own Complex number structure and definitions as the Complex type is not available in the Concurrency namespace
struct Complex1 {
	float x;
	float y;
};
Complex1 c_add(Complex1 c1, Complex1 c2) restrict(cpu, amp) // restrict keyword - able to execute this function on the GPUand CPU
{
Complex1 tmp;
float a = c1.x;
float b = c1.y;
float c = c2.x;
float d = c2.y;
tmp.x = a + c;
tmp.y = b + d;
return tmp;
} // c_add
float c_abs(Complex1 c) restrict(cpu, amp)
{
	return concurrency::fast_math::sqrt(c.x * c.x + c.y * c.y);
} // c_abs
Complex1 c_mul(Complex1 c1, Complex1 c2) restrict(cpu, amp)
{
	Complex1 tmp;
	float a = c1.x;
	float b = c1.y;
	float c = c2.x;
	float d = c2.y;
	tmp.x = a * c - b * d;
	tmp.y = b * c + a * d;
	return tmp;
} // c_mul


// Write the image to a TGA file with the given name.
// Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
//void write_tga(const char* filename)
//{
//	ofstream outfile(filename, ofstream::binary);
//
//	uint8_t header[18] = {
//		0, // no image ID
//		0, // no colour map
//		2, // uncompressed 24-bit image
//		0, 0, 0, 0, 0, // empty colour map specification
//		0, 0, // X origin
//		0, 0, // Y origin
//		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
//		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
//		24, // bits per pixel
//		0, // image descriptor
//	};
//	outfile.write((const char*)header, 18);
//
//	for (int y = 0; y < HEIGHT; ++y)
//	{
//		for (int x = 0; x < WIDTH; ++x)
//		{
//			uint8_t pixel[3] = {
//				image[y][x] & 0xFF, // blue channel
//				(image[y][x] >> 8) & 0xFF, // green channel
//				(image[y][x] >> 16) & 0xFF, // red channel
//			};
//			outfile.write((const char*)pixel, 3);
//		}
//	}
//
//	outfile.close();
//	if (!outfile)
//	{
//		// An error has occurred at some point since we opened the file.
//		cout << "Error writing to " << filename << endl;
//		exit(1);
//	}
//}


// Render the Mandelbrot set into the image array.
// The parameters specify the region on the complex plane to plot.

void Mandelbrot::compute_mandelbrot(float left, float right, float top, float bottom)
{


	//define a GPU image that points to the internal data of the image vector.
	uint32_t* GPU_image = image.data();	//&(((uint32_t**)image.data())[0][0]);

	//this is a 2d array that will take the CPU array and transforms it
	array_view<uint32_t, 2> a(HEIGHT, WIDTH, GPU_image);

	//tile size
	const int TS = 8;//24;			// height/TS = 9, width/TS = 16.

	//this defines write only varible for the GPU
	a.discard_data();

	the_clock::time_point start = the_clock::now();

	//a.extent will pass the number of data items of a to the GPU
	//concurrency is a namespace for amp

	//kernal function kernla is a GPU function
	parallel_for_each(a.extent.tile<TS, TS>(), [=](tiled_index<TS, TS> t_idx)
		//parallel_for_each(a.extent, [=](index<2> idx)
		restrict(amp)
		{

			//define two complex numbers
			Complex1 c;
			Complex1 z;



			//global index
			int idy = t_idx.global[0];
			int idx = t_idx.global[1];

			/*int y = idx[0];
			int x = idx[1];*/

			int y = t_idx.local[0] + TS * t_idx.tile[0];
			int x = t_idx.local[1] + TS * t_idx.tile[1];

			/*y = idy;
			x = idx;*/

			//This are thread Id's used to manipulate the complex plain in the CPU
			//n/a


			//apply a local 2d array
			//tile_static uint32_t t[2][2];

			//t[t_idx.local[0]][t_idx.local[1]] = a[t_idx.global];

			/*tile_static int x;
			tile_static int y;*/



			// Start off z at (0, 0).
			z.x = 0.0;
			z.y = 0.0;


			// Work out the point in the complex plane that
			//corresponds to this pixel in the output image.
			c.x = left + (x * (right - left) / WIDTH);
			c.y = top + (y * (bottom - top) / HEIGHT);


			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			//z =(z*z) + c
			int iterations = 0;
			while (c_abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{

				//this using complex1 function to multiply z by z and saves it as a varible
				Complex1 complex_mul = c_mul(z, z);
				//this will take the last varlible and add c to it
				Complex1 complex_add = c_add(c, complex_mul);

				//this defines Z as the varible
				z = complex_add;


				//continue on with iterations
				++iterations;
			}
			if (iterations == MAX_ITERATIONS) {
				a[y][x] = 0x000000;
			}
			else if (iterations <= 25)
			{
				a[y][x] = 0xFF0000;
			}
			else if (iterations <= 50)
			{
				a[y][x] = 0xFE6904;
			}
			else if (iterations <= 75)
			{
				a[y][x] = 0xFEE104;
			}
			else if (iterations <= 100)
			{
				a[y][x] = 0xB6FE04;
			}
			else if (iterations <= 125)
			{
				a[y][x] = 0x13FE04;
			}
			else if (iterations <= 150)
			{
				a[y][x] = 0x04FEA8;
			}
			else if (iterations <= 175)
			{
				a[y][x] = 0x04E1FE;
			}
			else if (iterations <= 200)
			{
				a[y][x] = 0x045BFE;
			}
			else if (iterations <= 250)
			{
				a[y][x] = 0x0904FE;
			}
			else if (iterations <= 300)
			{
				a[y][x] = 0x9404FE;
			}
			else if (iterations <= 400)
			{
				a[y][x] = 0xFE04E1;
			}
			else if (iterations <= 450)
			{
				a[y][x] = 0xFE047C;
			}
			else
			{
				a[y][x] = 0xFFFFFF;
			}

		});

	//this will wait for all the GPU threads, that are working on the a structure to finish before writting to the CPU and updating the GPU_image
	a.synchronize();

	the_clock::time_point end = the_clock::now();

	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

};

