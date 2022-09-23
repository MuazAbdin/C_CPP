#include "Matrix.h"
#include <stdlib.h>

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
		std::cerr << "Error: INVALID MATRIX" << std::endl;
		exit(EXIT_FAILURE);
	}
	_dim.rows = rows;
	_dim.cols = cols;
	_size = rows * cols;
	_matrix = new float[_size]();
}

/**
 * @brief Constructor.
 * @param Matrix: the given matrix to make a copy of it.
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
 * @brief Transforms matrix into a vecetor.
 */
Matrix &Matrix::vectorize()
{
	_dim.rows = _size;
	_dim.cols = 1;
	return *this;
}

/**
 * @brief Prints the matrix in order
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
 * @brief Assign matrix to another matrix
 * @param Matrix: right hand Matrix.
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
 * @brief Multiply Matrix by matrix
 * @param Matrix: right hand Matrix.
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
	if (getCols() != rhs.getRows())
	{
		std::cerr << "Error: CAN'T MULTIPLY THESE SIZES" << std::endl;
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
 * @brief Multiply Matrix by scalar.
 * @param Scalar: (float) scalar.
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
 * @brief Multiply scalar by Matrix.
 * @param Scalar: (float) scalar.
 * @param Matrix: right hand Matrix.
 */
Matrix operator*(const float &scalar, const Matrix &m)
{
	return m * scalar;
}

/**
 * @brief Sum two matrices.
 * @param Matrix: right hand Matrix.
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << "Error: CAN'T SUM THESE SIZES" << std::endl;
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
 * @brief Add a matrix to teh current matrix.
 * @param Matrix: right hand Matrix.
 */
Matrix &Matrix::operator+=(const Matrix &rhs)
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << "Error: CAN'T SUM THESE SIZES" << std::endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < _size; i++)
	{
		_matrix[i] += rhs._matrix[i];
	}
	return *this;
}

/**
 * @brief Return the [index] element in the matrix-able to change the result.
 * @param Index: the given index
 */
float &Matrix::operator[](const int index)
{
	if (index < 0 || index > _size - 1)
	{
		std::cerr << "Error: INVALID INDEX" << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[index];
}

/**
 * @brief Return the [index] element in the matrix-not able to change the result.
 * @param Index: the given index
 */
const float &Matrix::operator[](const int index) const
{
	if (index < 0 || index > _size - 1)
	{
		std::cerr << "Error: INVALID INDEX" << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[index];
}

/**
 * @brief Return the i,j element in the matrix-able to change the result.
 * @param Row:i
 * @param Col: j
 */
float &Matrix::operator()(const int row, const int col)
{
	if (row < 0 || col < 0 || row > getRows() - 1 || col > getCols() - 1)
	{
		std::cerr << "Error: INVALID INDEX" << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[(row * getCols()) + col];
}

/**
 * @brief Return the i,j element in the matrix-not able to change the result.
 * @param Row:i
 * @param Col: j
 */
const float &Matrix::operator()(const int row, const int col) const
{
	if (row < 0 || col < 0 || row > getRows() - 1 || col > getCols() - 1)
	{
		std::cerr << "Error: INVALID INDEX" << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[(row * getCols()) + col];
}

/**
 * @brief Inputs data
 * @param Istream: input stream.
 * @param Matrix: the matrix to input.
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
		std::cerr << "Error: INVALID INPUT STREAM/INVALID PIXELS SIZE" << std::endl;
		exit(EXIT_FAILURE);
	}
	return is;
}

/**
 * @brief Outputs data
 * @param Ostream: output stream.
 * @param Matrix: the matrix to print.
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