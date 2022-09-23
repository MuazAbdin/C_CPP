// MlpNetwork.cpp

#ifndef MLPNETWORK_CPP
#define MLPNETWORK_CPP

/**
* @file MlpNetwork.cpp
* @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
* @ID 300575297
* @date 13 May 2020
*
*
* @section DESCRIPTION
* 			.
*/


#include "MlpNetwork.h"

/**
 * @brief activates the 4 MLP network layers.
 * @param imgVector: vector represents the image.
 * @return the probability and the value.
 */
const Digit MlpNetwork::operator()(const Matrix &imgVector) const
{
	Dense layer1(_weights[0], _biases[0], Relu);
	Matrix r1 = layer1(imgVector);

	Dense layer2(_weights[1], _biases[1], Relu);
	Matrix r2 = layer2(r1);

	Dense layer3(_weights[2], _biases[2], Relu);
	Matrix r3 = layer3(r2);

	Dense layer4(_weights[3], _biases[3], Softmax);
	Matrix r4 = layer4(r3);

	int r4Len = r4.getRows() * r4.getCols();
	Digit output = {0, r4[0]};
	for (int j = 0; j < r4Len; ++j)
	{
		if (r4[j] > output.probability)
		{
			output.value = j;
			output.probability = r4[j];
		}
	}

	return output;
}

#endif //MLPNETWORK_CPP