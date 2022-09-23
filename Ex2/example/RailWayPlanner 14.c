/**
 * @file manageStudents.c
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @date 20 April 2020
 *
 * @brief Program that receives from user a list of students info:
 * 			<ID>,<name>,<grade>,<age>,<country>,<city>
 * 		  and executes on the valid input one of the following:
 *        [1] Shows the best student according to the ratio (grade:age).
 *        [2] Sorts the students in ascending order by their grades using MergeSort.
 *        [3] Sorts the students in ascending order by their names using QuickSort.
 *
 * @section DESCRIPTION
 * Program that analyzes students info and sorts it.
 * Input  : Student's info: <ID>,<name>,<grade>,<age>,<country>,<city>
 * Process: Responds to the input by according to the run argument in shell:
 * 			[1] best: shows the best student.
 * 			[2] merge: MergeSort students by grade in ascending order.
 * 			[3] quick: QuickSort students by name in ascending order.
 * Output : > If the input is not valid - print informative message
 *          > Else if the input is valid – print best student info, or sorted list
 *            of students according to run argument.
 */

// ------------------------------ includes --------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <regex.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

// ------------------------------ constants --------------------------------

#define OUTPUT_FILE "./railway_planner_output.txt"
#define ARGC_ERROR "Usage: RailWayPlanner <InputFile>"
#define NO_FILE_ERROR "File doesn't exists."
#define EMPTY_FILE_ERROR "File is empty."
#define INVALID_INPUT_ERROR "Invalid input in line: %d."
#define INITIAL_ALLOC 10
#define NO_LINE 0
#define MAX_ROW 1024
#define NUM_PART_DETAILS 4
#define INF INT_MAX

// ------------------------------ functions & structs ---------------------------

void errorMessage(char arr[], int lineIncluded);
int emptyFile(FILE *file);
int validFile(FILE *filename);
int helperCheckPartDetails(char detail[], int idx, char joints[]);



// ------------------------------ ################### -----------------------------

typedef struct railPart
{
	char start, end;
	int length, price;
} railPart;

typedef struct algorithmInput
{
	int railLen, numJoints, numParts;
	char *kindsJoints;
	railPart *partsCollection;
} AlgIn;

void errorMessage(char arr[], int lineIncluded)
{
	FILE *outputFile = fopen(OUTPUT_FILE, "w");
	if (outputFile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	if (! lineIncluded)
	{
		fprintf(outputFile, "%s", arr);
	}
	else
	{
		fprintf(outputFile, arr, lineIncluded);
	}
	fclose(outputFile);
}

int emptyFile(FILE *file)
{
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0)
	{
		return 1;
	}
	rewind(file);
	return 0;
}

int validFile(FILE *file)
{
	if (file == NULL)
	{
		errorMessage(NO_FILE_ERROR, NO_LINE);
		return 0;
	}
	if (emptyFile(file))
	{
		errorMessage(EMPTY_FILE_ERROR, NO_LINE);
		return 0;
	}
	return 1;
}

int checkDigit(char arr[])
{
	for (int i = 0; i < (int) strlen(arr) - 1; i++)
	{
		if (! isdigit(arr[i]))
		{
			return 0;
		}
	}
	return 1;
}

int checkInteger(char arr[])
{
	if (! checkDigit(arr))
	{
		return -1;
	}
	char *ptr;
	int num = (int) strtol(arr, &ptr, 10);
	return num;
}

int checkJoints(char arr[], int numJoints)
{
	// check if # of joint symbols exceeds # of joint kinds
	int count = 0;
	for (int i = 0; i < (int) strlen(arr) - 1; i++)
	{
		if (arr[i] == ',')
		{
			count++;
		}
	}
	if (count != (numJoints - 1))
	{
		return 0;
	}

	// check if there is joint symbols with more than one char
	int jointSymbols = (int) strlen(arr) - count - 1;
	if (jointSymbols != numJoints)
	{
		return 0;
	}
	return 1;
}

char *jointsArray(char arr[], int numJoints)
{
	char *railJoints = (char *)malloc(numJoints * sizeof(char) + 1);
	if (railJoints == NULL)
	{
		return 0;
	}
	char *ptr = strtok(arr, ",");
	for (int i = 0; i < numJoints; i++)
	{
		if (ptr != NULL)
		{
			railJoints[i] = *ptr;
			ptr = strtok(NULL, ",");
		}
	}
	return railJoints;
}


int checkPartDetails(char part[], char **details, char joints[])
{
	char *detPtr = strtok(part, "\n");
	char *ptr = strtok(detPtr, ",");
	int idx = 0;
	while (ptr != NULL)
	{
		if (! helperCheckPartDetails(ptr, idx, joints))
		{
			return 0;
		}
		details[idx] = (char *)malloc(strlen(ptr) * sizeof(char));
		strncpy(details[idx], ptr, strlen(ptr));
		ptr = strtok(NULL, ",");
		idx++;
	}
	return 1;
}

int helperCheckPartDetails(char detail[], int idx, char joints[])
{
	if (idx == 0 || idx == 1)
	{
		if (strchr(joints, detail[0]) == NULL)
		{
			return 0;
		}
	}
	else if (idx == 2 || idx == 3)
	{
		if (checkInteger(detail) <= 0)
		{
			return 0;
		}
	}
	return 1;
}

int addPartDetail(char **details, AlgIn railInfo, const int *capacity, int line)
{
	int numParts = line - 3;
	if (numParts == *capacity)
	{
		capacity += INITIAL_ALLOC;
		railInfo.partsCollection = (railPart *)realloc(railInfo.partsCollection,
													   *capacity * sizeof(railPart));
	}
	sscanf(details[0], "%c", &railInfo.partsCollection[numParts - 1].start);
	sscanf(details[1], "%c", &railInfo.partsCollection[numParts - 1].end);

	char *lenPtr, *pricePtr;
	railInfo.partsCollection[numParts - 1].length = (int) strtol(details[2], &lenPtr, 10);
	railInfo.partsCollection[numParts - 1].price = (int) strtol(details[3], &pricePtr, 10);


	return 1;
}

int checkDetails(FILE *file, AlgIn *railInfo)
{
	char input[MAX_ROW];
	char *partDetails[NUM_PART_DETAILS];
	int capacity = INITIAL_ALLOC;
	railInfo -> partsCollection = (railPart *)malloc(capacity * sizeof(railPart));
	if (railInfo -> partsCollection == NULL)
	{
		return 0;
	}

	int line = 1;
	while (fgets(input, sizeof(input), file) != NULL)
	{
		if (line == 1)
		{
			railInfo -> railLen = checkInteger(input);
			if (railInfo -> railLen < 0)
			{
				return line;
			}
		}

		else if (line == 2)
		{
			railInfo -> numJoints = checkInteger(input);
			if (railInfo -> numJoints <= 0)
			{
				return line;
			}
		}

		else if (line == 3)
		{
			if (! checkJoints(input, railInfo -> numJoints))
			{
				return line;
			}
			railInfo -> kindsJoints = jointsArray(input, railInfo -> numJoints);
		}

		else
		{
			if (! checkPartDetails(input, partDetails, railInfo -> kindsJoints))
			{
				return line;
			}
			addPartDetail(partDetails, *railInfo, &capacity, line);
		}
		line++;
	}
	railInfo -> numParts = line - 4;
	return 0;
}


int min(int argsCount, ...)
{
	va_list args;
	va_start(args, argsCount);

	int minValue = va_arg(args, int);
	int curValue;

	for (int i = 1; i < argsCount; i++)
	{
		if ((curValue = va_arg(args,int)) < minValue)
		{
			minValue = curValue;
		}
	}
	va_end(args);
	return minValue;
}

int sameEndPart(char joint, int length, AlgIn railInfo)
{
	int capacity = 1;
	railPart *partSameEnd = (railPart *)malloc(capacity * sizeof(struct railPart));
	if (partSameEnd == NULL)
	{
		return 0;
	}
	int count = 0;
	for (int i = 0; i < railInfo.numParts; i++)
	{
		if (railInfo.partsCollection[i].end == joint &&
			railInfo.partsCollection[i].length <= length)
		{
			if (capacity == count)
			{
				capacity++;
				partSameEnd = (railPart *)malloc(capacity * sizeof(struct railPart));
			}
			partSameEnd[count] = railInfo.partsCollection[i];
			count++;
		}
	}
	return count;
}

int jointIDX(char joint, AlgIn railInfo)
{
	for (int i = 0; i < railInfo.numJoints; i++)
	{
		if (joint == railInfo.kindsJoints[i])
		{
			return i;
		}
	}
	return -1;
}

int checkMin(char joint, int row, int table[], AlgIn railInfo)
{
	int minCost = INF;
//	printf(" %d ", table[row * railInfo.numJoints + col]);
	for (int i = 0; i < railInfo.numParts; i++)
	{
		if (railInfo.partsCollection[i].end == joint &&
			railInfo.partsCollection[i].length <= row)
		{
			int colIdx = jointIDX(railInfo.partsCollection[i].start, railInfo);
			int cost = railInfo.partsCollection[i].price +
					table[(row - railInfo.partsCollection[i].length) * railInfo.numJoints + colIdx];
			if (cost < minCost)
			{
				minCost = cost;
			}
		}
	}
	return minCost;
}


void tableBuilder(int rows, int columns, AlgIn railInfo)
{
	int *table = (int *)malloc(rows * columns * sizeof(int));

//	railPart *currentPart = NULL;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
//			table[r * columns + c] = 0;
			if (r == 0)
			{
				table[r * columns + c] = 0;
			}
			else
			{
				table[r * columns + c] = checkMin(railInfo.kindsJoints[c], r, table, railInfo);
			}
		}
	}

//	table[2 * columns + 2] = 5;
//	checkMin('%', 2, table, railInfo);
//	printf("\n\n");

	printf("     @     $     %%     &   \n    -------------------------------\n");
	for (int r = 0; r < rows; r++)
	{
		printf("(%d)", r);
		for (int c = 0; c < columns; c++)
		{
			printf("  %d  |", table[r * columns + c]);
		}
		printf("\n");
	}

}



int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		errorMessage(ARGC_ERROR, NO_LINE);
		return EXIT_FAILURE;
	}

	FILE *inputFile = fopen(argv[1], "r");

	if (! validFile(inputFile))
	{
		fclose(inputFile);
		return EXIT_FAILURE;
	}

	AlgIn inputDetails = {0};
	int line = checkDetails(inputFile, &inputDetails);
	if (line)
	{
		errorMessage(INVALID_INPUT_ERROR,line);
		return EXIT_FAILURE;
	}

//	minCostJoint(inputDetails.kindsJoints[3], 4, &inputDetails);

	tableBuilder(inputDetails.railLen + 1, inputDetails.numJoints, inputDetails);
//	printf("%d", min(4, 3,5,6,10));

//	printf("%d", inputDetails.numParts);

	fclose(inputFile);
	return EXIT_SUCCESS;
}