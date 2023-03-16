/* Sample code showing how to process strings using AMP.
   Adam Sampson <a.sampson@abertay.ac.uk>

   The list of data types that you can use in restrict(amp) code is
   pretty limited -- in particular, it doesn't include char.
   Which is annoying if you want to do string processing on the GPU.

   This code shows a couple of different ways of getting around this
   problem, using the toy example of capitalising a string.

   Please don't assume that this is particularly good AMP code!
*/

#include <algorithm>
#include <amp.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::copy;
using std::cout;
using std::vector;

/* Helper function to convert a character to uppercase. */
uint32_t uppercaseify(const uint32_t &c) restrict(amp) {
	if (c >= 'a' && c <= 'z') {
		return c - ('a' - 'A');
	} else {
		return c;
	}
}

/* The first way: convert the string into an array of 32-bit ints,
   where each int represents one character. */
void one_char_per_int() {
	string input("hello from the CPU to the GPU and back again!");
	cout << "Input string: '" << input << "'\n";

	// Copy the input string into a vector of uint32_ts (unsigned 32-bit ints).
	vector<uint32_t> input_v(input.size());
	for (unsigned i = 0; i < input.size(); ++i) {
		input_v[i] = uint32_t(input[i]);
	}
	concurrency::array_view<const uint32_t> input_av(input_v);

	// Create an output array of uint32_ts.
	vector<uint32_t> output_v(input.size());
	concurrency::array_view<uint32_t> output_av(output_v);
	output_av.discard_data();

	// Run the AMP code.
	concurrency::parallel_for_each(input_av.extent, [=](concurrency::index<1> i) restrict(amp) {
		output_av[i] = uppercaseify(input_av[i]);
	});

	// Wait for the results to be written back to the output array.
	output_av.synchronize();

	// Copy the output array back into a string.
	string output;
	for (unsigned i = 0; i < output_v.size(); ++i) {
		output.push_back(char(output_v[i]));
	}

	cout << "Output string: '" << output << "'\n";
}

/* The second and slightly more complex way: treat the string, which
   consists of an array of 8-bit chars:
   
   A B C D E F G H I J K L ...

   as if it was an array of 32-bit unsigned ints, with four chars in each uint32_t:

   ABCD EFGH IJKL ...

   This makes better use of the GPU, because we're not wasting 75% of each array item,
   and we're processing more characters with each operation.
*/
void four_chars_per_int() {
	string input("hello from the CPU to the GPU and back again!");
	cout << "Input string: '" << input << "'\n";

	// Remember how long the string was originally.
	const int orig_size = input.size();

	// Add padding characters at the end of the input so it's a multiple of 4 long.
	while ((input.size() % 4) != 0) {
		input.push_back('X');
	}
	const int num_ints = input.size() / 4;
	// We can now create the array_view directly from the string itself --
	// no need to copy it.
	concurrency::array_view<const uint32_t> input_av(num_ints, (const uint32_t *) input.data());

	// Similarly, we can do output directly into a string, provided we make it
	// the right size first.
	string output;
	output.resize(input.size());
	concurrency::array_view<uint32_t> output_av(num_ints, (uint32_t *) output.data());
	output_av.discard_data();

	// Run the AMP code as above...
	concurrency::parallel_for_each(input_av.extent, [=](concurrency::index<1> i) restrict(amp) {
		// ... except that each item we process now contains *four* characters.

		// Extract the individual characters.
		const uint32_t i0 = (input_av[i] >> 24) & 0xFF;
		const uint32_t i1 = (input_av[i] >> 16) & 0xFF;
		const uint32_t i2 = (input_av[i] >> 8) & 0xFF;
		const uint32_t i3 = input_av[i] & 0xFF;

		// Convert each to uppercase.
		uint32_t o0 = uppercaseify(i0);
		uint32_t o1 = uppercaseify(i1);
		uint32_t o2 = uppercaseify(i2);
		uint32_t o3 = uppercaseify(i3);

		// Pack the characters back together again.
		output_av[i] = (o0 << 24) | (o1 << 16) | (o2 << 8) | o3;

		// You could write all the above as one big expression, but
		// I've split it up for clarity's sake.

		// Remember that the last item you process will include the padding added above.
	});

	// Wait for the output string to be written.
	output_av.synchronize();

	// Remove the padding from the end of the output string.
	output.resize(orig_size);

	cout << "Output string: '" << output << "'\n";
}

int main(int argc, char *argv[]) {
	one_char_per_int();
	four_chars_per_int();
	return 0;
}