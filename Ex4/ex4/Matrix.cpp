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
Matrix::Matrix(const int rows, const int cols)
		: _dim(MatrixDims())
{
	if (rows <= 0 || cols <= 0)
	{
		std::cerr << ERR_INIT_MAT_DIMS << std::endl;
		exit(EXIT_FAILURE);
	}
	_dim.rows = rows;
	_dim.cols = cols;
	_size = rows * cols;
	_matrix = new float[_size]();
}

/**
 * @brief Constructor.
 */
Matrix::Matrix(const Matrix &m)
		: _dim(MatrixDims())
{
	_dim = m._dim;
	_size = getRows() * getCols();
	_matrix = new float[_size]();
	for (int i = 0; i < _size; i++)
	{
		_matrix[i] = m._matrix[i];
	}
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
	_dim.rows = _size;
	_dim.cols = 1;
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
	_dim = rhs._dim;
	_size = rhs._size;
	delete[] _matrix;
	_matrix = new float[_size]();
	for (int i = 0; i < _size; i++)
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
	Matrix newMatrix(getRows(), rhs.getCols());
	for (int i = 0; i < newMatrix.getRows(); i++)
	{
		for (int j = 0; j < newMatrix.getCols(); j++)
		{
			float coordinate = 0;
			for (int k = 0; k < getCols(); k++)
			{
				coordinate += _matrix[(i * getCols()) + k] *
							  rhs._matrix[j + (rhs.getCols() * k)];
			}
			newMatrix._matrix[(i * newMatrix.getCols()) + j] = coordinate;
		}
	}
	return newMatrix;
}

/**
 * @brief Right scalar multiplication.
 */
Matrix Matrix::operator*(const float &scalar) const
{
	Matrix newMatrix(getRows(), getCols());
	for (int i = 0; i < newMatrix._size; i++)
	{
		newMatrix._matrix[i] = _matrix[i] * scalar;
	}
	return newMatrix;
}

/**
 * @brief Left scalar multiplication.
 */
Matrix operator*(const float &scalar, const Matrix &m)
{
	return m * scalar;
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
	Matrix newMatrix(getRows(), getCols());
	for (int i = 0; i < newMatrix._size; i++)
	{
		newMatrix._matrix[i] = _matrix[i] + rhs._matrix[i];
	}
	return newMatrix;
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
	for (int i = 0; i < _size; i++)
	{
		_matrix[i] += rhs._matrix[i];
	}
	return *this;
}

/**
 * @brief Gets the element in the given index.
 */
float &Matrix::operator[](const int index)
{
	if (index < 0 || index > _size - 1)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[index];
}

/**
 * @brief Gets the element in the given index.
 */
const float &Matrix::operator[](const int index) const
{
	if (index < 0 || index > _size - 1)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[index];
}

/**
 * @brief Gets the element in the given row and column.
 */
float &Matrix::operator()(const int row, const int col)
{
	if (row < 0 || col < 0 || row > getRows() - 1 || col > getCols() - 1)
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
	if (row < 0 || col < 0 || row > getRows() - 1 || col > getCols() - 1)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[(row * getCols()) + col];
}

/**
 * @brief Loding data
 */
std::istream &operator>>(std::istream &is, const Matrix &m)
{
	for (int i = 0; i < m.getCols() * m.getRows(); i++)
	{
		if (is.good())
		{
			is.read((char *) &(m._matrix[i]), sizeof(float));
		}
		else
		{
			break;
		}
	}
	if (!is.good() || is.peek() != EOF)
	{
		std::cerr << ERR_READING_FILE << std::endl;
		exit(EXIT_FAILURE);
	}
	return is;
}

/**
 * @brief Outputs data
 */
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			if (m(i, j) <= 0.1f)
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