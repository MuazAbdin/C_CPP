#include "MlpNetwork.h"

/**
 * @brief activates the 4 steps on image then returns the probability and the value.
 * @param Matrix: given matrix-image.
 */
const Digit MlpNetwork::operator()(const Matrix &img) const
{
	Dense layer1(_weights[0], _biases[0], Relu);
	Matrix entrance = layer1(img);

	Dense layer2(_weights[1], _biases[1], Relu);
	Matrix m2 = layer2(entrance);

	Dense layer3(_weights[2], _biases[2], Relu);
	Matrix m3 = layer3(m2);

	Dense layer4(_weights[3], _biases[3], Softmax);
	Matrix exit = layer4(m3);

	float probability = 0;
	unsigned int value = 0;
	for (int i = 0; i < exit.getRows(); i++)
	{
		if (exit[i] > probability)
		{
			probability = exit[i];
			value = i;
		}
	}
	Digit digit;
	digit.value = value;
	digit.probability = probability;
	return digit;
}

