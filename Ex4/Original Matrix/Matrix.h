// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * @class Matrix
 * @brief Matrix that supports arithmetic operations.
 */
class Matrix
{
private:
	MatrixDims _dims;
	float *_matrix;
public:
	/**
	 * @brief Constructor.
	 * @param rows: rows of the matrix
	 * @param cols: columns of the matrix
	 */
	Matrix(const int rows, const int cols);

	/**
     * @brief Constructor.
     */
	Matrix();

	/**
     * @brief Constructor.
     * @param rhs: the right hand side Matrix.
     */
	Matrix(const Matrix& rhs);

	/**
     * @brief Destructor.
     */
	~Matrix();

	/* Methods */
	/**
     * @brief GetRows function.
     */
	int getRows() const;

	/**
     * @brief GetCols function.
     */
	int getCols() const;

	/**
     * @brief Vectorize matrix into a vecetor.
     */
	Matrix& vectorize();

	/**
	 * @brief Prints the matrix
	 */
	void plainPrint() const;

	/* Operators */
	/**
	 * @brief Assignment operator
	 */
	Matrix& operator=(const Matrix& rhs);

	/**
	 * @brief Matrix Multiplication
	 */
	Matrix operator*(const Matrix& rhs) const;

	/**
	 * @brief Right scalar multiplication
	 */
	Matrix operator*(const float& scalar) const;

	/**
	 * @brief Matrix addition
	 */
	Matrix operator+(const Matrix& rhs) const;

	/**
	 * @brief Matrix addition accumulation
	 */
	Matrix& operator+=(const Matrix& rhs);

	/**
	 * @brief Gets the element in the given row and column
	 */
	const float& operator()(const int row, const int col) const;

	/**
	 * @brief Gets the element in the given row and column
	 */
	float& operator()(int row, int col);

	/**
	 * @brief Gets the element in the given index
	 */
	const float& operator[](const int idx) const;

	/**
	 * @brief Gets the element in the given index
	 */
	float& operator[](int idx);

	/* Friend operators */

	/**
 	* @brief Left scalar multiplication
 	*/
	friend Matrix operator*(const float& scalar, const Matrix& mat);

	/**
	 * @brief Outputs data
	 * @param Ostream: output stream.
	 * @param Matrix: the matrix to print.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);

	/**
	 * @brief Inputs data
	 * @param Istream: input stream.
	 * @param Matrix: the matrix to input.
	 */
	friend std::istream& operator>>(std::istream &inputFile, Matrix& mat);
};

#endif //MATRIX_H
