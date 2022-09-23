#ifndef C_DENSE_H
#define C_DENSE_H

#include "Activation.h"

/**
 * @class Dense-layer.
 */
class Dense
{
private:
	Matrix _weight;
	Matrix _bias;
	ActivationType _func;
public:
	/**
	 * @brief Constructor.
	 * @param Matrix: weight.
	 * @param Matrix: bias.
	 * @param ActivationType: function.
	 */
	Dense(const Matrix &w, const Matrix &bias, const ActivationType func)
			: _weight(w), _bias(bias), _func(func)
	{}

	/**
	 * @brief GetWeights function.
	 */
	const Matrix getWeights() const
	{ return _weight; }

	/**
	 * @brief GetBias function.
 	 */
	const Matrix getBias() const
	{ return _bias; }

	/**
	 * @brief Get function.
	 */
	ActivationType getActivation() const
	{ return _func; }

	/**
	 * @brief Multiply the weight by the given matrix then adding bias to return it-to Activate.
	 * @param Matrix: given matrix.
	 */
	const Matrix operator()(const Matrix &m) const;

};

#endif //C_DENSE_H
