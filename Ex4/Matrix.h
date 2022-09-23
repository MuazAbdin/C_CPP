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
     * @param cols: cols of the matrix
     */
	Matrix(const int rows, const int cols);

	/**
     * @brief Constructor.
     */
	Matrix() : Matrix(1, 1)
	{}

	/**
     * @brief Copy constructor..
     */
	Matrix(const Matrix &mat);

	/**
     * @brief Destructor.
     */
	~Matrix();

	/**
     * @brief Getter for Rows.
     */
	int getRows() const
	{ return _dims.rows; }

	/**
     * @brief Getter for Cols.
     */
	int getCols() const
	{ return _dims.cols; }

	/**
     * @brief vectorize matrix into a vecetor.
     */
	Matrix &vectorize();

	/**
	 * @brief Prints the matrix
	 */
	void plainPrint() const;

	/**
	 * @brief Assignment operator
	 */
	Matrix& operator=(const Matrix &rhs);

	/**
	 * @brief Matrix Multiplication
	 */
	Matrix operator*(const Matrix &rhs) const;

	/**
	 * @brief Right scalar multiplication.
	 */
	Matrix operator*(const float &scalar) const;

	/**
	 * @brief Left scalar multiplication.
	 */
	friend Matrix operator*(const float &scalar, const Matrix &mat);

	/**
	 * @brief Matrix addition.
	 */
	Matrix operator+(const Matrix &rhs) const;

	/**
	 * @brief Matrix addition accumulation.
	 */
	Matrix &operator+=(const Matrix &rhs);

	/**
	 * @brief Gets the element in the given index
	 */
	float &operator[](const int idx);

	/**
	 * @brief Gets the element in the given index
	 */
	const float &operator[](const int idx) const;

	/**
	 * @brief Gets the element in the given row and column.
	 */
	float &operator()(const int row, const int col);

	/**
	 * @brief Gets the element in the given row and column.
	 */
	const float &operator()(const int row, const int col) const;

	/**
	 * @brief Loading data to matrix
	 */
	friend std::istream &operator>>(std::istream &inputFile, const Matrix &mat);

	/**
	 * @brief Outputs data
	 */
	friend std::ostream &operator<<(std::ostream &os, const Matrix &mat);
};


#endif //MATRIX_H
