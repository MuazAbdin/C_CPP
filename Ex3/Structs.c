#ifndef STRUCTS_C
#define STRUCTS_C

/**
 * @file Structs.c
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @ID 300575297
 * @date 26 May 2020
 *
 *
 * @section DESCRIPTION
 * 			Two concrete examples on implementation of RBTree.h library:
 * 			(1) In the first the data of the node is of type C string.
 * 			(2) In the second the data of the node is of type Vector.
 */

// ------------------------------ includes --------------------------------

#include "RBTree.h"
#include "Structs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// ------------------------------ macros & constants --------------------------------

#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)

// ------------------------------ addition functions --------------------------------

double vecNorm(const Vector *pVector);
int deepCopy(const Vector *source, Vector *target);

// ----------------------------------------------------------------------------------

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
	if (a == NULL || b == NULL)
	{
		return false;
	}
	char *firstString = (char *) a;
	char *secondString = (char *) b;

	return strcmp(firstString, secondString);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	// CHECK IN CASE OF OVERLAP STRINGS.
	if (word == NULL || pConcatenated == NULL)
	{
		return false;
	}
	char *firstString = (char *) pConcatenated;
	char *secondString = (char *) word;

	strcat(strcat(firstString, secondString), "\n");

	return true;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
	char *string = (char *)s;
	free(string);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	Vector *firstVec = (Vector *)a;
	Vector *secondVec = (Vector *)b;

	int minLen = (firstVec->len < secondVec->len) ? firstVec->len : secondVec->len;

	for (int i = 0; i < minLen; ++i)
	{
		double max = fmax(*((firstVec->vector) + i), *((secondVec->vector) + i));

		if (*((firstVec->vector) + i) < max)
		{
			return LESS;
		}
		else if (*((secondVec->vector) + i) < max)
		{
			return GREATER;
		}
	}

	if ((firstVec->len != secondVec->len)) // CHECK (&& len > 0)
	{
		return (firstVec->len < secondVec->len) ? LESS : GREATER;
	}

	return EQUAL;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
	Vector *vec = (Vector *)pVector;
	free(vec->vector);
	free(vec);  // free(pVector);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater than the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	if (pVector == NULL || pMaxVector == NULL)
	{
		return false;
	}

	Vector *vec = (Vector *)pVector;
	Vector *maxVec = (Vector *)pMaxVector;

	/* if there is no coordinates in pVector so it is not greater */
	if (vec->vector == NULL)
	{
		return true;
	}

//	if (maxVec->vector == NULL)
//	{
//		return deepCopy(vec, maxVec);
//	}

	double vectorNorm = vecNorm(pVector);
	double maxVectorNorm = vecNorm(pMaxVector);

	/* norm of pMaxVector is greater or equal to pVector */
	if (maxVectorNorm == fmax(vectorNorm, maxVectorNorm))
	{
		return true;
	}

	return deepCopy(vec, maxVec);
}

/**
 * @brief make a deep copy of a vector.
 * @param source: the vector to be copied.
 * @param target: the vector to copy to it.
 * @return a deep copy of a given vector
 */
int deepCopy(const Vector *source, Vector *target)
{
	/* (source != NULL && *target != NULL && source->vector != NULL) */

//	double *newVec = (double *)calloc(source->len, sizeof(double));
//	if (newVec == NULL)
//	{
//		return false;
//	}
//
//	free(target->vector);
//	target->vector = newVec;

	target->vector = realloc(target->vector, source->len * sizeof(double));

	target->len = source->len;
	for (int i = 0; i < source->len; ++i)
	{
		target->vector[i] = source->vector[i];
	}

	return true;
}

/**
 * @brief calculates the norm of a given vector.
 * @param pVector: the vector to calculate its norm.
 * @return the norm of the vector.
 */
double vecNorm(const Vector *pVector)
{
	if (pVector->vector == NULL)
	{
		/* if there is no coordinates in pVector then the nor, is ZERO */
		return 0;
	}
	double coordsSquaresSum = 0;
	for (int i = 0; i < pVector->len; ++i)
	{
		coordsSquaresSum += pow((*((pVector->vector) + i)) , 2);
	}
	return sqrt(coordsSquaresSum);
}

/**
 * @param tree a pointer to a tree of Vectors
 * 		  You must use copyIfNormIsLarger in the implementation!
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if (tree == NULL || tree->root == NULL)
	{
		return NULL;
	}
	Vector *maxVector = (Vector *)malloc(sizeof(Vector));
	forEachRBTree(tree, copyIfNormIsLarger, maxVector);

	return maxVector;
}

#endif  // STRUCTS_C
