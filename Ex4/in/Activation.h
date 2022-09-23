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
	ActivationType _activationType;
public:
	/**
	 * @brief Constructor.
	 * @param ActivationType: function.
	 */
	explicit Activation(ActivationType activationType);

	/**
	 * @brief Getter
	 */
	ActivationType getActivationType() const;

	/**
	 * @brief Activate the function on the given matrix.
	 */
	const Matrix operator()(const Matrix& mat) const;
};

#endif //ACTIVATION_H
