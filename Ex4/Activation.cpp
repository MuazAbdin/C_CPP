// Activation.cpp

#ifndef ACTIVATION_CPP
#define ACTIVATION_CPP


/**
* @file Activation.cpp
* @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
* @ID 300575297
* @date 13 May 2020
*
*
* @section DESCRIPTION
* 			.
*/

// ------------------------------ includes ------------------------------------------

#include "Matrix.h"
#include "Activation.h"

#include <cmath>

// ------------------------------ functions implementation ---------------------------

/* Constructors */
/**
* @brief Constructor.
*/
Activation::Activation(ActivationType activationType): _activationType(activationType) {}

/* Methods */
/**
* @brief Getter.
*/
ActivationType Activation::getActivationType() const
{
	return _activationType;
}
/* Operators */
/**
 * @brief Activate the function on the given matrix.
 * @param Matrix: Matrix that needs to apply function on it.
 */
const Matrix Activation::operator()(const Matrix& mat) const
{
	Matrix activeMatrix(mat.getRows(), mat.getCols());
	int matLen = activeMatrix.getRows() * activeMatrix.getCols();
	if (getActivationType() == Relu)
	{
		for (int i = 0; i < matLen; ++i)
		{
			activeMatrix[i] = (mat[i] >= 0) ? mat[i] : 0;
		}
	}
	else if (getActivationType() == Softmax)
	{
		float expSum = 0;
		for (int i = 0; i < matLen; ++i)
		{
			expSum += std::exp(mat[i]);
		}
		for (int j = 0; j < matLen; ++j)
		{
			activeMatrix[j] = (1 / expSum) * std::exp(mat[j]);
		}
	}
	return activeMatrix;
}

#endif //ACTIVATION_CPP

