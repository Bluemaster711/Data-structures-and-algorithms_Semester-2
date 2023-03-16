#ifndef MANDELBROT_H
#define MANDELBROT_H

class Task {

public:
	virtual ~Task() {

	}

	virtual inline void run() = 0;

};

#endif

