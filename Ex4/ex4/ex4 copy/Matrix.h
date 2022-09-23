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
 * @brief Matrix that does all the known operations such as  +,*.
 */
class Matrix
{
private:
	MatrixDims _dim;
	float *_matrix;
	int _size;
public:
	/**
     * @brief Constructor.
     * @param Rows: the rows of the matrix
     * @param Cols: the cols of the matrix
     */
	Matrix(const int rows, const int cols);

	/**
     * @brief Constructor.
     */
	Matrix() : Matrix(1, 1)
	{}

	/**
     * @brief Constructor.
     * @param Matrix: the given matrix to make a copy of it.
     */
	Matrix(const Matrix &m);

	/**
     * @brief Destructor.
     */
	~Matrix();

	/**
     * @brief GetRows function.
     */
	int getRows() const
	{ return _dim.rows; }

	/**
     * @brief GetCols function.
     */
	int getCols() const
	{ return _dim.cols; }

	/**
     * @brief Transforms matrix into a vecetor.
     */
	Matrix &vectorize();

	/**
	 * @brief Prints the matrix in order
	 */
	void plainPrint() const;

	/**
	 * @brief Assign matrix to another matrix
	 * @param Matrix: right hand Matrix.
	 */
	Matrix &operator=(const Matrix &rhs);

	/**
	 * @brief Multiply Matrix by matrix
	 * @param Matrix: right hand Matrix.
	 */
	Matrix operator*(const Matrix &rhs) const;

	/**
	 * @brief Multiply Matrix by scalar.
	 * @param Scalar: (float) scalar.
	 */
	Matrix operator*(const float &scalar) const;

	/**
	 * @brief Multiply scalar by Matrix.
	 * @param Scalar: (float) scalar.
	 * @param Matrix: right hand Matrix.
	 */
	friend Matrix operator*(const float &scalar, const Matrix &m);

	/**
	 * @brief Sum two matrices.
	 * @param Matrix: right hand Matrix.
	 */
	Matrix operator+(const Matrix &rhs) const;

	/**
	 * @brief Add a matrix to teh current matrix.
	 * @param Matrix: right hand Matrix.
	 */
	Matrix &operator+=(const Matrix &rhs);

	/**
	 * @brief Return the [index] element in the matrix-able to change the result.
	 * @param Index: the given index
	 */
	float &operator[](const int index);

	/**
	 * @brief Return the [index] element in the matrix-not able to change the result.
	 * @param Index: the given index
	 */
	const float &operator[](const int index) const;

	/**
	 * @brief Return the i,j element in the matrix-able to change the result.
	 * @param Row:i
	 * @param Col: j
	 */
	float &operator()(const int row, const int col);

	/**
	 * @brief Return the i,j element in the matrix-not able to change the result.
	 * @param Row:i
	 * @param Col: j
	 */
	const float &operator()(const int row, const int col) const;

	/**
	 * @brief Inputs data
	 * @param Istream: input stream.
	 * @param Matrix: the matrix to input.
	 */
	friend std::istream &operator>>(std::istream &is, const Matrix &m);

	/**
	 * @brief Outputs data
	 * @param Ostream: output stream.
	 * @param Matrix: the matrix to print.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Matrix &m);
};


#endif //MATRIX_H
