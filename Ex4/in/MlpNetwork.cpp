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

// ------------------------------ includes ------------------------------------------

#include "Matrix.h"
#include "Activation.h"
#include "Dense.h"
#include "Digit.h"
#include "MlpNetwork.h"

// ------------------------------ macros & constants --------------------------------

#define LAST_LAYER 3
#define IMG_VEC_LEN 784
#define ERR_IMG_VEC "Error: Image vector contains values other than [0, 1]."

// ------------------------------ functions implementation ---------------------------

/* Constructor */
MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[]):
_weights(weights), _biases(biases) {}

/* Operator */
/**
 * @brief activates the 4 MLP network layers.
 * @param imgVector: vector represents the image.
 * @return the probability and the value.
 */
const Digit MlpNetwork::operator()(const Matrix& imgVector) const
{
	for (int i = 0; i < IMG_VEC_LEN; ++i)
	{
		if (!(imgVector[i] <= 1 || imgVector[i] >= 0))
		{
			std::cerr << ERR_IMG_VEC << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	ActivationType activationType = Relu;
	Matrix layerIntput = imgVector;
	for (int i = 0; i < MLP_SIZE; ++i)
	{
		if (i == LAST_LAYER)
		{
			activationType = Softmax;
		}

		Dense layer(_weights[i], _biases[i], activationType);
		layerIntput = layer(layerIntput);
	}

	int inputLen = layerIntput.getRows() * layerIntput.getCols();
	if (inputLen != 10)
	{
		std::cerr << "Error: inputLen == " << inputLen << " != 10" << std::endl;
		exit(EXIT_FAILURE);
	}
	Digit output = {0, layerIntput[0]};
	for (int j = 0; j < inputLen; ++j)
	{
		if (layerIntput[j] > output.probability)
		{
			output.value = j;
			output.probability = layerIntput[j];
		}
	}
	
	return output;
}


#endif //MLPNETWORK_CPP
