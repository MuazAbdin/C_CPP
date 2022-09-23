//Dense.h
#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * @class Dense
 */
class Dense
{
private:
	Matrix _weights;
	Matrix _bias;
	Activation _activation;
public:
	/**
	 * @brief Constructor
	 * @param layerWeights: layerWeights Matrix
	 * @param layerBias: layerBias Matrix
	 * @param layerActivationType: layer Activation Type
	 */
	Dense(const Matrix& layerWeights, const Matrix& layerBias, ActivationType layerActivationType);

	/**
	 * @brief GetWeights function.
	 */
	Matrix getWeights() const;

	/**
	 * @brief GetBias function.
 	 */
	Matrix getBias() const;

	/**
	 * @brief Get function.
	 */
	Activation getActivation() const;

	/**
	 * @brief Multiply the weight by the given matrix then adding bias.
	 * @param layerInput: layerInput
	 */
	Matrix operator()(const Matrix& layerInput) const;
};


#endif //DENSE_H
