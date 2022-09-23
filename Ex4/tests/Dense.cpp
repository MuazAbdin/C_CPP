// Dense.cpp

#ifndef DENSE_CPP
#define DENSE_CPP

/**
* @file Dense.cpp
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
#include "Dense.h"

// ------------------------------ functions implementation ---------------------------

/* Constructor */
Dense::Dense(const Matrix& layerWeights, const Matrix& layerBias, ActivationType layerActivationType):
_weights(layerWeights), _bias(layerBias), _activation(layerActivationType) {}

/* Methods */
Matrix Dense::getWeights() const
{
	return _weights;
}

Matrix Dense::getBias() const
{
	return _bias;
}

Activation Dense::getActivation() const
{
	return _activation;
}

/* Operator */
Matrix Dense::operator()(const Matrix& layerInput) const
{
	Matrix outputMat = getWeights() * layerInput;
	outputMat += getBias();
	return getActivation()(outputMat);
}


#endif //DENSE_CPP
