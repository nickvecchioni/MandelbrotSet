/*********************
*i*  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE* file = fopen(colorfile, "r");
	uint8_t color1, color2, color3;
	int numOfColors = 0, i = 0;
	uint8_t** arrayOfColors;

	if(file == NULL) {
		return NULL;
	}
	
	int initial_scan_result = fscanf(file, "%d", &numOfColors);

	if(initial_scan_result == EOF || initial_scan_result == 0 || numOfColors <= 0)	{
		fclose(file);
		return NULL;
	} 	

	*colorcount = numOfColors;

	arrayOfColors = (uint8_t **) malloc(sizeof(uint8_t*) * numOfColors);

	for(int i = 0; i < numOfColors; i++) {
		arrayOfColors[i] = (uint8_t *) malloc(sizeof(uint8_t) * 3);
	}

	while(fscanf(file, "%hhu %hhu %hhu", &color1, &color2, &color3) != EOF && i < numOfColors) {
		arrayOfColors[i][0] = color1;
		arrayOfColors[i][1] = color2;
		arrayOfColors[i][2] = color3;
		i++;	
	}
	
	if(i != numOfColors) {
		for(int j = 0; j < i; j++) {
			free(arrayOfColors[j]);
		}
		free(arrayOfColors);
		fclose(file);
		return NULL;
	}
	*colorcount = i;

	fclose(file);
	
	return arrayOfColors;
}


