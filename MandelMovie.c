/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
   	double track_scale = initialscale;
       	for(int i = 1; i <= framecount; i++) {
	    Mandelbrot(threshold, max_iterations, center, track_scale, resolution, output[i-1]);
    	    track_scale = initialscale * pow(finalscale / initialscale, (double) i / (framecount - 1));
	}
}	

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/
	if (argc != 11) {
		return 1;
	}
	if (atof(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atof(argv[5]) <= 0 || atof(argv[6]) <= 0) {
		return 1;
	}
	if(atoi(argv[7]) <= 0 || atoi(argv[7]) > 10000) {
		return 1;
	}
	if(atoi(argv[8]) < 0) {
		return 1;
	}
	// TODO colorfile is not properly formatted
	if(atoi(argv[7]) == 1 && (atof(argv[5]) != atof(argv[6]))) {
		return 1;
	}
	int* colorcount = (int *) malloc(sizeof(int));
       	double threshold = atof(argv[1]);
	u_int64_t  maxiterations = (u_int64_t)  atoi(argv[2]);
	ComplexNumber* center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	double initialscale = atof(argv[5]);
	double finalscale = atof(argv[6]);
	int framecount = atoi(argv[7]);
	u_int64_t resolution = (u_int64_t) atoi(argv[8]);
	FILE* output_folder = fopen(argv[9], "w+");
	uint8_t** colormap = FileToColorMap(argv[10], colorcount);
	char* output_folder_string = argv[9];
	char* P6_file_name;

	if (colormap == NULL) {
		free(colorcount);
		freeComplexNumber(center);
		fclose(output_folder);
		return 1;
	}

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	u_int64_t** output = (u_int64_t **) malloc(sizeof(u_int64_t*) * framecount);

	if(output == NULL) {
		free(output);
		
		for(int i = 0; i < *colorcount; i++) {
			free(colormap[i]);
		}
		free(colormap);
		free(colorcount);
		return 1;
	}
	
	for(int i = 0; i < framecount; i++) {
		output[i] = (u_int64_t *) malloc(sizeof(u_int64_t) * pow(2 * resolution + 1, 2));
	}

	MandelMovie(threshold, maxiterations, center, initialscale, finalscale, framecount, resolution, output);

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	
	//	fprintf(output, "P6 %d %d 255\n", 2 * resolution + 1, 2 * resolution + 1)/;

	for(int i = 0; i < framecount; i++) {
		P6_file_name = calloc(100 ,sizeof(char));
		sprintf(P6_file_name, "%s/frame%05d.ppm", output_folder_string, i);
		FILE* P6_output = fopen(P6_file_name, "w+");	
		fprintf(P6_output, "P6 %lu %lu 255\n", 2 * resolution + 1, 2 * resolution + 1);
		for(int j = 0; j < pow(2 * resolution + 1, 2); j++) {
			int iterations = output[i][j];
			if(!iterations) {
				u_int64_t rgb[3] = {0, 0, 0};
				fwrite(rgb, sizeof(uint8_t), 3, P6_output);
			} else if (iterations % *colorcount == 0) {
			 	u_int64_t* rgb = (u_int64_t *) colormap[*colorcount - 1];
			  	fwrite(rgb, sizeof(uint8_t), 3, P6_output);	
			} else {
				u_int64_t* rgb = (u_int64_t *) colormap[iterations % *colorcount - 1];
				fwrite(rgb, sizeof(uint8_t), 3, P6_output);
			}
		}
		free(P6_file_name);
	}
	for(int i = 0; i < *colorcount; i++) {
		free(colormap[i]);
	}
	free(colormap);
	free(colorcount);
	free(center);
	for(int i = 0; i < framecount; i++) {
		free(output[i]);
	}
	free(output);	
}
