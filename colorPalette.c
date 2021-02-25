/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	FILE* output = fopen(outputfile, "w");
	int* numColors = malloc(sizeof(int));
	uint8_t** colorArray = FileToColorMap(colorfile, numColors);

	if(colorArray == NULL || width < 1 || heightpercolor < 1 || *numColors == 0) {
		free(numColors);
		return 1;
	}
		

	fprintf(output, "P3 %d %d 255\n", width, heightpercolor * *numColors);

	for (int i = 0; i < *numColors; i++) {
		for(int j = 0; j < heightpercolor; j++) {
			for(int k = 0; k < width - 1; k++) {
				 for (int val = 0; val <= 2; val++) {
					fprintf(output, "%d ", colorArray[i][val]);
			   	 }
    			 }
			for(int val = 0; val <=1; val++) {
				fprintf(output, "%d ", colorArray[i][val]);	
		}		fprintf(output, "%d\n", colorArray[i][2]); }
	}
	fclose(output);
	for(int i = 0; i < *numColors; i++) {
		free(colorArray[i]);
	}
	free(colorArray);
	free(numColors);
	
        return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	FILE* output = fopen(outputfile, "w");
	int* numColors = malloc(sizeof(int));
	uint8_t** colorArray = FileToColorMap(colorfile, numColors);

	if(colorArray == NULL || width < 1 || heightpercolor < 1) {
		return 1;
	}

	fprintf(output, "P6 %d %d 255\n", width, heightpercolor * *numColors);

	for(int i = 0; i < *numColors; i++) {
		for(int j = 0; j < heightpercolor; j++) {
			for(int k = 0; k < width; k++) {
				fwrite(colorArray[i], sizeof(uint8_t), 3, output);
			}
		}
	}
	fclose(output);
	
	for(int i = 0; i < *numColors; i++) {
		free(colorArray[i]);
	}
	free(numColors);
	free(colorArray);
			
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




