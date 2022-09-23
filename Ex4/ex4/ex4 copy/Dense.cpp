#include "Dense.h"

/**
 * @brief Multiply the weight by the given matrix then adding bias to return it-to Activate.
 * @param Matrix: given matrix.
 */
const Matrix Dense::operator()(const Matrix &m) const
{
	Activation act(_func);
	Matrix newMatrix = getWeights() * m;
	newMatrix += getBias();
	return act(newMatrix);
}