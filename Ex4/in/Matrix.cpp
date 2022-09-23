// Matrix.cpp

#ifndef MATRIX_CPP
#define MATRIX_CPP


/**
 * @file Matrix.cpp
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @ID 300575297
 * @date 13 May 2020
 *
 */

#include "Matrix.h"
#include <cstdlib>


#define ERR_INIT_MAT_DIMS "Error: Rows and columns must be positive integers."
#define ERR_ALLOC_FAILED "Error: Allocating memory on heap failed."
#define ERR_OUT_OF_RANGE "Error: Index out of range."
#define ERR_MAT_MULTIPLICATION "Error: Columns of first matrix must equal rows of "\
                               "the second matrix."
#define ERR_MAT_ADDITION "Error: Matrices must be of same size (rows X cols)."
#define ERR_READING_FILE "Error: file not read successfully"



/**
 * @brief Constructor.
 * @param Rows: the rows of the matrix
 * @param Cols: the cols of the matrix
 */
Matrix::Matrix(const int rows, const int cols): _dims({rows, cols})
{
	if (rows <= 0 || cols <= 0)
	{
		std::cerr << ERR_INIT_MAT_DIMS << std::endl;
		exit(EXIT_FAILURE);
	}
	_matrix = new (std::nothrow) float[getRows() * getCols()];
	if (! _matrix)
	{
		std::cerr << ERR_ALLOC_FAILED << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Constructor.
 */
Matrix::Matrix(const Matrix& rhs): Matrix()
{
	this->operator=(rhs);
}

/**
 * @brief Destructor.
 */
Matrix::~Matrix()
{
	delete[] _matrix;
}

/**
 * @brief vectorize matrix.
 */
Matrix &Matrix::vectorize()
{
	_dims.rows = getRows() * getCols();
	_dims.cols = 1;
	return *this;
}

/**
 * @brief Prints the matrix
 */
void Matrix::plainPrint() const
{
	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getCols(); j++)
		{
			std::cout << _matrix[(i * getCols()) + j] << " ";
		}
		std::cout << std::endl;
	}
}

/**
 * @brief Assignment operator
 */
Matrix &Matrix::operator=(const Matrix &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}
	delete [] _matrix;
	_dims = {rhs.getRows(), rhs.getCols()};
	_matrix = new (std::nothrow) float[getRows() * getCols()];
	if (! _matrix)
	{
		std::cerr << ERR_ALLOC_FAILED << std::endl;
		exit(EXIT_FAILURE);
	}
	int matSize = getRows() * getCols();
	for (int i = 0; i < matSize; i++)
	{
		_matrix[i] = rhs._matrix[i];
	}
	return *this;
}

/**
 * @brief Matrix Multiplication
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
	if (getCols() != rhs.getRows())
	{
		std::cerr << ERR_MAT_MULTIPLICATION << std::endl;
		exit(EXIT_FAILURE);
	}
	Matrix product(getRows(), rhs.getCols());
	for (int i = 0; i < product.getRows(); i++)
	{
		for (int j = 0; j < product.getCols(); j++)
		{
			float coord = 0;
			for (int k = 0; k < getCols(); k++)
			{
				coord += _matrix[(i * getCols()) + k] *
							  rhs._matrix[j + (rhs.getCols() * k)];
			}
			product._matrix[(i * product.getCols()) + j] = coord;
		}
	}
	return product;
}

/**
 * @brief Right scalar multiplication.
 */
Matrix Matrix::operator*(const float &scalar) const
{
	Matrix product(getRows(), getCols());
	int matSize = product.getRows() * product.getCols();
	for (int i = 0; i < matSize; i++)
	{
		product._matrix[i] = _matrix[i] * scalar;
	}
	return product;
}

/**
 * @brief Left scalar multiplication.
 */
Matrix operator*(const float &scalar, const Matrix &mat)
{
	return mat * scalar;
}

/**
 * @brief Matrix addition
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << ERR_MAT_ADDITION << std::endl;
		exit(EXIT_FAILURE);
	}
	Matrix sumMat(getRows(), getCols());
	int matSize = sumMat.getRows() * sumMat.getCols();
	for (int i = 0; i < matSize; i++)
	{
		sumMat._matrix[i] = _matrix[i] + rhs._matrix[i];
	}
	return sumMat;
}

/**
 * @brief Matrix addition accumulation
 */
Matrix &Matrix::operator+=(const Matrix &rhs)
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << ERR_MAT_ADDITION << std::endl;
		exit(EXIT_FAILURE);
	}
	int matSize = getRows() * getCols();
	for (int i = 0; i < matSize; i++)
	{
		_matrix[i] += rhs._matrix[i];
	}
	return *this;
}

/**
 * @brief Gets the element in the given index.
 */
float &Matrix::operator[](const int idx)
{
	int rowIdx = idx / getCols();
	int colIdx = idx % getCols();
	if (rowIdx < 0 || colIdx < 0 || rowIdx >= _dims.rows || colIdx >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[rowIdx * getCols() + colIdx];
}

/**
 * @brief Gets the element in the given index.
 */
const float &Matrix::operator[](const int idx) const
{
	int rowIdx = idx / getCols();
	int colIdx = idx % getCols();
	if (rowIdx < 0 || colIdx < 0 || rowIdx >= _dims.rows || colIdx >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[rowIdx * getCols() + colIdx];
}

/**
 * @brief Gets the element in the given row and column.
 */
float &Matrix::operator()(const int row, const int col)
{
	if (row < 0 || col < 0 || row >= getRows() || col >= getCols())
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[(row * getCols()) + col];
}

/**
 * @brief Gets the element in the given row and column.
 */
const float &Matrix::operator()(const int row, const int col) const
{
	if (row < 0 || col < 0 || row >= getRows() || col >= getCols())
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[(row * getCols()) + col];
}

/**
 * @brief Loding data
 */
std::istream &operator>>(std::istream &inputFile, const Matrix &mat)
{
	inputFile.read((char *) mat._matrix, mat.getRows() * mat.getCols() * sizeof(float));
	if (!inputFile.good())
	{
		std::cerr << ERR_READING_FILE << std::endl;
		exit(EXIT_FAILURE);
	}
	if (inputFile.peek() != EOF)
	{
		std::cerr << ERR_READING_FILE << std::endl;
		exit(EXIT_FAILURE);
	}
	return inputFile;
}

/**
 * @brief Outputs data
 */
std::ostream &operator<<(std::ostream &os, const Matrix &mat)
{
	for (int i = 0; i < mat.getRows(); i++)
	{
		for (int j = 0; j < mat.getCols(); j++)
		{
			if (mat(i, j) <= 0.1f)
			{
				os << "  ";
			}
			else
			{
				os << "**";
			}
		}
		os << std::endl;
	}
	return os;
}

#endif //MATRIX_CPP