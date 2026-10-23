#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>

//Stores rows * cols elements in row-major order
typedef struct{
    size_t rows;
    size_t cols;
    double *elements;
}Matrix;

//Creates a matrix. Returns NULL on failure
Matrix *matrix_create(size_t rows, size_t cols);

//Returns the value at (row, col)
double matrix_get(const Matrix *matrix, size_t row, size_t col);

//Sets the value at (row, col)
void matrix_set(Matrix *matrix, size_t row, size_t col, double value);

//Copies one matrix to a new one. Returns NULL on failure
Matrix *matrix_copy(const Matrix *matrix);

//Adds two matrices. Returns NULL if dimensions are incompatible
Matrix *matrix_add(const Matrix *a, const Matrix *b);

//Subtracts b from a. Returns a new matrix, or NULL if dimensions are incompatible
Matrix *matrix_subtract(const Matrix *a, const Matrix *b);

//Multiplies two matrices. Returns NULL if dimensions are incompatible
Matrix *matrix_multiply(const Matrix *a, const Matrix *b);

//Multiplies element by element of two matrices and returns new matrix. Returns NULL if dimensions are incompatible
Matrix *matrix_hadamard(const Matrix *a, const Matrix *b);

//Multiplies each element of the matrix by a scalar and returns new matrix. Returns NULL on failure
Matrix *matrix_scale(const Matrix *matrix, double scalar);

//Returns the transpose of a matrix
Matrix *matrix_transpose(const Matrix *matrix);

//Prints the matrix
void matrix_print(const Matrix *matrix);

//Frees matrix and its allocated memory
void matrix_free(Matrix *matrix);

#endif