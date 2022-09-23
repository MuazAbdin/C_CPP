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

// ------------------------------ functions & structs ---------------------------

void errorMessage(char arr[], int lineIncluded);
int emptyFile(FILE *file);
int validFile(FILE *filename);



// ------------------------------ ################### -----------------------------

typedef struct railPart
{
	char start, end;
	int length, price;
} railPart;

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
//	printf("%d\n", (int) strlen(arr));
	for (int i = 0; i < (int) strlen(arr) - 1; i++)
	{
		if (! isdigit(arr[i]))
		{
//			printf("%d -- %c \n", arr[i], arr[i]);
			return 0;
		}
	}
	return 1;
}


int checkInteger(char arr[])
{
	int num;
//	char *lenPtr = (char *)malloc(strlen(arr) * sizeof(char));
//	sscanf(arr, "%[^'\n']", lenPtr);
	if (! checkDigit(arr))
	{
		return -1;
	}
	char *ptr;
	num = (int) strtol(arr, &ptr, 10);
//	free(lenPtr);
//	printf("%d", num);
	return num;
}

int checkJoints(char arr[], int numJoints)
{
	printf("%s%c -- ( %d )\n", arr, arr[9], (int) strlen(arr));
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
		printf("ERROR: \n");
		return 0;
	}
	int howChars = ((int) strlen(arr)) - count;
	printf("%d\n", howChars);
	return 1;
}

int checkDetails(FILE *file)
{
	char input[1024];
	int railLen;
	int numJoints = 0;
//	char *railJoints = NULL;
	int line = 1;
	while (fgets(input, sizeof(input), file) != NULL)
	{
		if (line == 1)
		{
			railLen = checkInteger(input);
			if (railLen >= 0)
			{
				printf("LINE (%d) -> %d\n", line, railLen);
			}
			else
			{
				errorMessage(INVALID_INPUT_ERROR, line);
				return 0;
			}
		}

		if (line == 2)
		{
			numJoints = checkInteger(input);
			if (numJoints <= 0)
			{
				errorMessage(INVALID_INPUT_ERROR,line);
				return 0;
			}
			else
			{
				printf("LINE (%d) -> %d\n", line, numJoints);
			}
		}

		if (line == 3)
		{

			printf("%d\n", (int) strlen(input));
			checkJoints(input, numJoints);
		}

		line++;
	}
	return 1;
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

	checkDetails(inputFile);
	fclose(inputFile);
	return 0;
}