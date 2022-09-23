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
#define MAX_ROW 1024
#define NUM_PART_DETAILS 4

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

int isCharInArray(char arr[], int arrSize, char symbol)
{
	for (int i = 0; i < arrSize; i++)
	{
		if (arr[i] == symbol)
		{
			return 1;
		}
	}
	return 0;
}

void checkPartDetails(char part[], char **details)
{
//	char *partDetails[NUM_PART_DETAILS];
//	printf("\n(( %p )) --[%p  %p %p %p] \n", details, details[0], details[1], details[2],
//		   details[3]);

	char *detPtr = strtok(part, "\n");
//	printf("%s", detPtr);

	char *ptr = strtok(detPtr, ",");
	int idx = 0;
	while (ptr != NULL)
	{
		details[idx] = (char *)malloc(strlen(ptr) * sizeof(char));
//		printf("\n(( %p )) --[%p  %p %p %p] \n", details, details[0], details[1], details[2],
//			   details[3]);

//		printf("size of (%s) = %lu\n", ptr, strlen(ptr));
//		printf("size of = %lu\n", sizeof(partDetails));
		strncpy(details[idx], ptr, strlen(ptr));
//		printf("%s", partDetails[idx]);
//		partDetails[idx] = *ptr;
		ptr = strtok(NULL, ",");
		idx++;
	}
	printf("[%s][%s][%s][%s]\n", details[0], details[1], details[2], details[3]);

//	printf("%s", ptr);
//	return details;
}

//int helperCheckPartDetails(char detail, int idx, char joints[])
//{
//	if (idx == 0 || idx == 1)
//	{
//		if (! isCharInArray(joints, NUM_PART_DETAILS, detail))
//		{
//			return 0;
//		}
//	}
//	else if (idx == 2 || idx == 3)
//	{
//		char
//		int num = checkInteger(detail)
//	}
//}

int checkDetails(FILE *file)
{
	char input[MAX_ROW];

	int railLen;
	int numJoints = 0;
	char *kindsJoints = NULL;
	char *partDetails[NUM_PART_DETAILS];
//	railPart *partsCollection = NULL;

	int line = 1;
	while (fgets(input, sizeof(input), file) != NULL)
	{
		if (line == 1)
		{
			railLen = checkInteger(input);
			if (railLen < 0)
			{
				errorMessage(INVALID_INPUT_ERROR, line);
				return 0;
			}
			printf("LINE (%d) -> %d\n", line, railLen);
		}

		else if (line == 2)
		{
			numJoints = checkInteger(input);
			if (numJoints <= 0)
			{
				errorMessage(INVALID_INPUT_ERROR,line);
				return 0;
			}
			printf("LINE (%d) -> %d\n", line, numJoints);
		}

		else if (line == 3)
		{
			if (! checkJoints(input, numJoints))
			{
				errorMessage(INVALID_INPUT_ERROR,line);
				return 0;
			}
			kindsJoints = jointsArray(input, numJoints);
			printf("LINE (%d) -> %s\n", line, kindsJoints);
//			printf("LINE (%d) -> %s", line, input);
		}

		else
		{
//			int capacity = INITIAL_ALLOC;
//			partsCollection = (railPart *)malloc(capacity * sizeof(railPart));
//			if (partsCollection == NULL)
//			{
//				return 0;
//			}
//			int numParts = line - 3;
//			if (numParts == capacity)
//			{
//				capacity += INITIAL_ALLOC;
//				partsCollection = (railPart *)reallocf(partsCollection, capacity * sizeof(railPart));
//			}

			printf("LINE (%d) -> ", line);
			checkPartDetails(input, partDetails);
			printf("[%s][%s][%s][%s]\n", partDetails[0], partDetails[1], partDetails[2], partDetails[3]);
//			*partDetails = (char *) checkPartDetails(input, partDetails);
//			printf("\n (%s)", *partDetails);
//			printf(" (%s) \n", *partDetails);
//			for (int i = 0; i < NUM_PART_DETAILS; i++)
//			{
//				printf("%s", partDetails[i]);
//			}

//			charInArray(railjoints, )

//			partsCollection[numParts].start;
//			partsCollection[numParts].end;
//			partsCollection[numParts].length;
//			partsCollection[numParts].price;

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