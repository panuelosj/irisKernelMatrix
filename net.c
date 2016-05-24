// stuff goes here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cpgplot.h"

#define MAX_LINE 30000					//max chars in a line

#define NUMB_ROWS 100		//150
#define NUMB_COLS 794
#define RESOLUTION 784

#define X_MIN 0.0
#define X_MAX 26.0
#define Y_MIN 0.0
#define Y_MAX 26.0

// making arrays and mem management
float** allocateArray2D(int lengthRow, int lengthCol);									// generic array allocation
float*** allocateArray3D(int dim1, int dim2, int dim3);

// importing data
float** readCSV(const char filename[], const int lengthRow, const int lengthCol);


int main()
{
	//============================================================================================
	//----------------------------------VARIABLE INITIALIZATION-----------------------------------
	//============================================================================================
	int i, j, k, a, b, c;
	int input;
	float sum, normal, minVal, maxVal;

	// arrays for reading/holding data set

	// arrays for passing in/out between functions
	float plotNumb[RESOLUTION];
	float imageOrig[28][28];
	float imageTransf[26][26];
	int err = 0;												//error flag
	float kernelMap[3][3];
	float TR[6] = {0, 1, 0, 0, 0, 1};

	printf("Arrays built\n");
	//system("cat nagato");

	//=========================================================================================
	//----------------------------------OTHER INITIALIZATIONS----------------------------------
	//=========================================================================================
	
	// initialize pgplot window
	if (!cpgopen("/XWINDOW"))
		return 1;
	cpgenv(X_MIN, X_MAX, Y_MIN, Y_MAX, 0, 1);

	//=========================================================================================
	//---------------------------------------ACTUAL CODE---------------------------------------
	//=========================================================================================

	// import data
	float** dataSet = readCSV("mnist_train_100_ok.csv", NUMB_ROWS, NUMB_COLS);

	while (42) {
		printf("Please input 3x3 kernel matrix, one row at a time, with space delimiters\n");
		for ( i = 0; i < 3; i++)
			scanf("%f %f %f", &kernelMap[i][0], &kernelMap[i][1], &kernelMap[i][2]);
		printf("Kernel saved!\n");
		
		normal = 0.0;
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				normal += abs(kernelMap[i][j]);
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				kernelMap[i][j] = kernelMap[i][j]/normal;
		
		input = 0;
		while (input != -1) {
			scanf("%i", &input);
			
			if (input != -1) {
				for (i = 0; i < 28; i++) {
					for (j = 0; j < 28; j++) {
						imageOrig[i][j] = (dataSet[input][i*28+j]);
					}
				}
				
				for (i = 1; i < 27; i++){
					for (j = 1; j < 27; j++) {
						sum = 0.0;
						for (a = -1; a < 2; a++)
							for (b = -1; b < 2; b++) {
								sum += ((float)(imageOrig[i+a][j+b])*kernelMap[a+1][b+1]);
							}
						imageTransf[i-1][j-1] = (sum);
					}
				}
				
				
				//pgplot
				minVal = imageTransf[0][0]; maxVal = imageTransf[0][0];
				for (i = 0; i < 26; i++) {
					for (j = 0; j < 26; j++) {
						plotNumb[i*26+j] = imageTransf[28-i][j];
						if (minVal > plotNumb[i*26+j])
							minVal = plotNumb[i*26+j];
						if (maxVal < plotNumb[i*26+j])
							maxVal = plotNumb[i*26+j];
					}
				}
				cpggray(plotNumb, 26, 26, 1, 26, 1, 26, minVal, maxVal, TR);
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//
//======================================================================================================================//
//--------------------------------------------FUNCTIONS-----------------------------------------------------------------//
//======================================================================================================================//
//----------------------------------------------------------------------------------------------------------------------//

float** readCSV(const char filename[], const int lengthRow, const int lengthCol) {
	float** arrayWrite = allocateArray2D(lengthRow, lengthCol);

	char line[MAX_LINE];
	int i, j;
	float temp;

	FILE *stream = fopen(filename, "r");						//input file, read-only

	if (stream == NULL)	{
		system("cat nagato");
		printf("\nFailed to open %s!\nPress ENTER to end program...\n", filename);
		getchar();
		exit(-1);
	}

	for (i = 0; i < lengthRow; i++) {
		fgets(line, MAX_LINE, stream);							//grabs next line of stream

		temp = atof(strtok(line, ",\n"));							//convert to float, everything before comma delimiter
		for (j = 0; j < (lengthCol - 1); j++) {
			arrayWrite[i][j] = temp;									//save float in matrix
			temp = atof(strtok(NULL, ",\n"));						//keep reading from last position
		}
		arrayWrite[i][j] = temp;										//save last line
	}
	fclose(stream);
	printf("%s read successful!\n", filename);
	return arrayWrite;
}

float** allocateArray2D(int lengthRow, int lengthCol) {
	int i, j;
	float** newArray;
	newArray = (float **)malloc(lengthRow*sizeof(float *));
	if (newArray == NULL)
		printf("Malloc failed!");
	for (i = 0; i < lengthRow; i++) {
		newArray[i] = (float *)malloc(lengthCol*sizeof(float));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
	}
	return newArray;
}
float*** allocateArray3D(int dim1, int dim2, int dim3) {
	int i, j, k;
	float*** newArray;
	newArray = (float ***)malloc(dim1*sizeof(float **));
	if (newArray == NULL)
		printf("Malloc failed!");
	for (i = 0; i < dim1; i++) {
		newArray[i] = (float **)malloc(dim2*sizeof(float *));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
		for (j = 0; j < dim2; j++) {
			newArray[i][j] = (float *)malloc(dim3*sizeof(float));
			if (newArray[i][j] == NULL)
				printf("Malloc failed!");
		}
	}
	return newArray;
}

