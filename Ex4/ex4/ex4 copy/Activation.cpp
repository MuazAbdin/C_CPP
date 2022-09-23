#include "Activation.h"
#include <cmath>

/**
 * @brief Activate the function on the given matrix.
 * @param Matrix: Matrix that needs to apply function on it.
 */
const Matrix Activation::operator()(const Matrix &m) const
{
	Matrix newMatrix(m.getRows(), m.getCols());
	if (getActivationType() == Relu)
	{
		for (int i = 0; i < m.getRows(); i++)
		{
			if (m[i] >= 0)
			{
				newMatrix[i] = m[i];
			}
			else
			{
				newMatrix[i] = 0;
			}
		}
	}
	else
	{
		float sum = 0;
		for (int k = 0; k < m.getRows(); k++)
		{
			sum += std::exp(m[k]);
		}
		for (int n = 0; n < m.getRows(); n++)
		{
			newMatrix[n] = (1 / sum) * std::exp(m[n]);
		}
	}
	return newMatrix;
}
