//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
	Relu,
	Softmax
};

/**
 * @class Activation class
 */
class Activation
{
private:
	ActivationType _func;

public:
	/**
	 * @brief Constructor.
	 * @param ActivationType: function.
	 */
	explicit Activation(ActivationType actType) : _func(actType)
	{}

	/**
	 * @brief Get function.
	 */
	ActivationType getActivationType() const
	{ return _func; }

	/**
	 * @brief Activate the function on the given matrix.
	 * @param Matrix: Matrix that needs to apply function on it.
	 */
	const Matrix operator()(const Matrix &m) const;
};

#endif //ACTIVATION_H
