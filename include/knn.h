#ifndef KNN_H
#define KNN_H

#include "matrix.h"

//Calculate eulcidean distance between one row to another in each matrix
//row1 and row2 must have same number of columns
//Returns NAN if any matrix is equal to NULL or different columns or row nonexistent
double euclidean_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2);

#endif