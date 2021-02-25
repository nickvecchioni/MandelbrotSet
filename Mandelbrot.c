/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
	u_int64_t iter = 0;
	ComplexNumber* z = newComplexNumber(0, 0);

	while (ComplexAbs(z) < threshold && iter < maxiters) {
		ComplexNumber* z_squared = ComplexProduct(z, z);
		ComplexNumber* sum = ComplexSum(z_squared, point);
		
		freeComplexNumber(z);
		freeComplexNumber(z_squared);
	
		z = newComplexNumber(Re(sum), Im(sum));
		freeComplexNumber(sum);
		iter++;
	}

	freeComplexNumber(z);

	if (iter == maxiters) {
		return 0;
	} 
	return iter;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
	
	ComplexNumber* iterComplex; //newComplexNumber(Re(center), Im(center));
	if(resolution == 0) {
		output[0] = MandelbrotIterations(max_iterations, center, threshold);
		return;
	}
	double scale_resolution = scale / resolution;
	int iter = 0;
	double left_real = Re(center) - scale;
	double up_down_tracker = scale;
	double left_right_tracker = -scale;
	ComplexNumber* tempIter;
	
	for(int i = 0; i < 2 * resolution + 1; i++) {
		for(int j = 0; j < 2 * resolution + 1; j++) {
			ComplexNumber* tempNew = newComplexNumber(left_right_tracker, up_down_tracker);
			iterComplex = ComplexSum(center, tempNew);
			output[iter] = MandelbrotIterations(max_iterations, iterComplex, threshold);
			freeComplexNumber(iterComplex);
			iter++;
			left_right_tracker += scale_resolution;
			free(tempNew);
		}
	left_right_tracker = -scale;
	up_down_tracker -= scale_resolution;
	}	    
}
