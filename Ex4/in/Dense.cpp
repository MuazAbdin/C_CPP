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
/**
 * @brief Constructor
 * @param layerWeights: layerWeights Matrix
 * @param layerBias: layerBias Matrix
 * @param layerActivationType: layer Activation Type
 */
Dense::Dense(const Matrix& layerWeights, const Matrix& layerBias, ActivationType layerActivationType):
_weights(layerWeights), _bias(layerBias), _activation(layerActivationType) {}

/* Methods */
/**
 * @brief GetWeights function.
 */
Matrix Dense::getWeights() const
{
	return _weights;
}

/**
 * @brief GetBias function.
 */
Matrix Dense::getBias() const
{
	return _bias;
}
/**
 * @brief Get function.
 */
Activation Dense::getActivation() const
{
	return _activation;
}

/* Operator */
/**
 * @brief Multiply the weight by the given matrix then adding bias.
 * @param layerInput: layerInput
 */
Matrix Dense::operator()(const Matrix& layerInput) const
{
	Matrix outputMat = getWeights() * layerInput;
	outputMat += getBias();
	return getActivation()(outputMat);
}


#endif //DENSE_CPP
