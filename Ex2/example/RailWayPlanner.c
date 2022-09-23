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

// ------------------------------ functions & structs ---------------------------

void errorMessage(char arr[]);
int emptyFile(FILE *file);
int validFile(FILE *filename;



// ------------------------------ ################### -----------------------------

void errorMessage(char arr[])
{
	FILE *outputFile = fopen(OUTPUT_FILE, "w");
	if (outputFile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	fprintf(outputFile, "%s", arr);
	fclose(outputFile);
}

int emptyFile(FILE *file)
{
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0)
	{
		return 1;
	}
	return 0;
}

int validFile(FILE *filename)
{
	FILE *inputFile = fopen(filename, "r");
	if (inputFile == NULL)
	{
		errorMessage(NO_FILE_ERROR);
		return 0;
	}
	if (emptyFile(inputFile))
	{
		errorMessage(EMPTY_FILE_ERROR);
		fclose(inputFile);
		return 0;
	}
	return 1;
}




int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		errorMessage(ARGC_ERROR);
		return EXIT_FAILURE;
	}

	FILE *inputFile = fopen(argv[1], "r");
	if (! validFile(inputFile))
	{
		return EXIT_FAILURE;
	}

	fclose(inputFile);
	return 0;
}