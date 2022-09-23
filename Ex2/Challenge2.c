#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int indexMeshgrid(int numRows, int numCols, int*** rowMatrix, int *** colMatrix);
void freeMeshgrid(int numRows, int*** rowMatrix, int *** colMatrix);


int indexMeshgrid(int numRows, int numCols, int*** rowMatrix, int *** colMatrix)
{
	*rowMatrix = (int **)malloc(numRows * sizeof(int));
	if (*rowMatrix == NULL)
	{
		return 0;
	}
	for (int r = 0; r < numRows; ++r)
	{
		*rowMatrix[r] = (int *)malloc(numCols * sizeof(int));
		if (*rowMatrix[r] == NULL)
		{
			free(*rowMatrix);
			*rowMatrix = NULL;
			return 0;
		}
		for (int c = 0; c < numCols; ++c)
		{
			*rowMatrix[r][c] = r;
		}
	}

	*colMatrix = (int **)malloc(numRows * sizeof(int));
	if (*colMatrix == NULL)
	{
		return 0;
	}
	for (int r = 0; r < numRows; ++r)
	{
		*colMatrix[r] = (int *)malloc(numCols * sizeof(int));
		if (*colMatrix[r] == NULL)
		{
			free(*colMatrix);
			*colMatrix = NULL;
			return 0;
		}
		for (int c = 0; c < numCols; ++c)
		{
			*colMatrix[r][c] = c;
		}
	}
	return 1;
}


void freeMeshgrid(int numRows, int*** rowMatrix, int *** colMatrix)
{
	if (*rowMatrix == NULL || *colMatrix == NULL)
	{
		return;
	}
	for (int i = 0; i < numRows; ++i)
	{
		free(*rowMatrix[i]);
		free(*colMatrix[i]);
		*rowMatrix[i] = NULL;
		*colMatrix[i] = NULL;
	}

	free(*rowMatrix);
	*rowMatrix = NULL;

	free(*colMatrix);
	*colMatrix = NULL;

}


int main()
{
	int** rowMat=NULL;
	int** colMat=NULL;
	printf("%d\n",indexMeshgrid(4,3,&rowMat,&colMat));
//	printMat(4,3,&rowMat);
//	printMat(4,3,&colMat);
//	freeMeshgrid(4,&rowMat,&colMat);

	return 0;
}
