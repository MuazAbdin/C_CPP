//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
								  {64,  128},
								  {20,  64},
								  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
							   {64,  1},
							   {20,  1},
							   {10,  1}};

/**
 * @class Mlpnetwork
 */
class MlpNetwork
{
private:

	Matrix *_weights;
	Matrix *_biases;
public:
	/**
	 * @brief Constructor.
	 */
	MlpNetwork(Matrix weights[], Matrix biases[]) : _weights(weights), _biases(biases)
	{}

	/**
	 * @brief activates the 4 steps on image then returns the probability and the value.
	 * @param Matrix: given matrix-image.
	 */
	const Digit operator()(const Matrix &imgVector) const;
};

#endif // MLPNETWORK_H
