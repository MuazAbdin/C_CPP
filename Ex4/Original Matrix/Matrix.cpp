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

// ------------------------------ includes ------------------------------------------

#include "Matrix.h"

#include <cstdlib>

// ------------------------------ macros & constants --------------------------------

#define ERR_INIT_MAT_DIMS "Error: Rows and columns must be positive integers."
#define ERR_ALLOC_FAILED "Error: Allocating memory on heap failed."
#define ERR_OUT_OF_RANGE "Error: Index out of range."
#define ERR_MAT_MULTIPLICATION "Error: Columns of first matrix must equal rows of "\
                               "the second matrix."
#define ERR_MAT_ADDITION "Error: Matrices must be of same size (rows X cols)."
#define ERR_READING_FILE "Error: file not read successfully"


// ------------------------------ functions implementation ---------------------------

/* Constructors & destructors */
/**
 * @brief Constructor.
 * @param rows: rows of the matrix
 * @param cols: columns of the matrix
 */
Matrix::Matrix(int rows, int cols): _dims({rows, cols})
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
Matrix::Matrix(): Matrix(1, 1) {}

/**
 * @brief Constructor.
 * @param rhs: the right hand side Matrix.
 */
Matrix::Matrix(const Matrix& rhs): Matrix()
{
	_dims = rhs._dims;
	int size = getRows() * getCols();
	_matrix = new float[size];
	for (int i = 0; i < size; ++i)
	{
		_matrix[i] = rhs._matrix[i];
	}
//	this->operator=(rhs);
}

/**
 * @brief Destructor.
 */
Matrix::~Matrix()
{
	delete [] _matrix;
}

/* Methods */
/**
 * @brief GetRows function.
 */
int Matrix::getRows() const
{
	return _dims.rows;
}

/**
 * @brief GetCols function.
 */
int Matrix::getCols() const
{
	return _dims.cols;
}

/**
 * @brief Vectorize matrix into a vecetor.
 */
Matrix& Matrix::vectorize()
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
	for (int i = 0; i < getRows(); ++i)
	{
		for (int j = 0; j < getCols(); ++j)
		{
			std::cout << (*this)(i, j) << " ";
		}
		std::cout << std::endl;
	}
}

/* Operators */
/**
 * @brief Assignment operator
 */
Matrix& Matrix::operator=(const Matrix& rhs)
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

	for (int i = 0; i < getRows(); ++i)
	{
		for (int j = 0; j < getCols(); ++j)
		{
			(*this)(i, j) = rhs(i, j);
		}
	}
	return *this;
}

/**
 * @brief Matrix Multiplication
 */
Matrix Matrix::operator*(const Matrix& rhs) const  // Matrix Multiplication
{
	if (getCols() != rhs.getRows())
	{
		std::cerr << ERR_MAT_MULTIPLICATION << std::endl;
		exit(EXIT_FAILURE);
	}

	Matrix product(getRows(), rhs.getCols());
	for (int i = 0; i < product.getRows(); ++i)
	{
		for (int j = 0; j < product.getCols(); ++j)
		{
			for (int k = 0; k < getCols(); ++k)
			{
				product(i, j) += (*this)(i, k) * rhs(k, j);
			}
		}
	}
	return product;
}


/**
 * @brief Matrix addition
 */
Matrix Matrix::operator+(const Matrix& rhs) const
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << ERR_MAT_ADDITION << std::endl;
		exit(EXIT_FAILURE);
	}
	Matrix newMatrix(getRows(), getCols());
	int size = getRows() * getCols();
	for (int i = 0; i < size; i++)
	{
		newMatrix[i] = _matrix[i] + rhs._matrix[i];
	}
	return newMatrix;
}

/**
 * @brief Matrix addition accumulation
 */
Matrix& Matrix::operator+=(const Matrix& rhs) // Matrix addition accumulation
{
	if (getRows() != rhs.getRows() || getCols() != rhs.getCols())
	{
		std::cerr << ERR_MAT_ADDITION << std::endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < getRows(); ++i)
	{
		for (int j = 0; j < getCols(); ++j)
		{
			(*this)(i, j) += rhs(i, j);
		}
	}
	return *this;
}

/**
 * @brief Gets the element in the given row and column
 */
const float& Matrix::operator()(const int row, const int col) const
{
	if (row >= _dims.rows || col >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[row * getCols() + col];
}

/**
 * @brief Gets the element in the given row and column
 */
float& Matrix::operator()(int row, int col)
{
	if (row >= _dims.rows || col >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[row * getCols() + col];
}

/**
 * @brief Gets the element in the given index
 */
const float& Matrix::operator[](const int idx) const
{
	int rowIdx = idx / getCols();
	int colIdx = idx % getCols();
	if (rowIdx >= _dims.rows || colIdx >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[rowIdx * getCols() + colIdx];
}

/**
 * @brief Gets the element in the given index
 */
float& Matrix::operator[](int idx)
{
	int rowIdx = idx / getCols();
	int colIdx = idx % getCols();
	if (rowIdx >= _dims.rows || colIdx >= _dims.cols)
	{
		std::cerr << ERR_OUT_OF_RANGE << std::endl;
		exit(EXIT_FAILURE);
	}
	return _matrix[rowIdx * getCols() + colIdx];
}

/* Friend operators */

/**
 * @brief Right scalar multiplication
 */
Matrix Matrix::operator*(const float& scalar) const  // Right scalar multiplication
{
//	Matrix product = (*this);
	Matrix product(getRows(), getCols());
//	int size = getRows() * getCols();
	for (int i = 0; i < getRows(); ++i)
	{
		for (int j = 0; j < getCols(); ++j)
		{
			product(i, j) = _matrix[i] * scalar;
		}
	}
	return product;
}

/**
 * @brief Left scalar multiplication
 */
Matrix operator*(const float& scalar, const Matrix& mat)
{
	return mat * scalar;
}


/**
 * @brief Outputs data
 * @param Ostream: output stream.
 * @param Matrix: the matrix to print.
 */
std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
	for (int i = 0; i < mat.getRows(); ++i)
	{
		for (int j = 0; j < mat.getCols(); ++j)
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

/**
 * @brief Inputs data
 * @param Istream: input stream.
 * @param Matrix: the matrix to input.
 */
std::istream& operator>>(std::istream &inputFile, Matrix& mat)
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



#endif //MATRIX_CPP
