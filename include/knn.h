#ifndef KNN_H
#define KNN_H

#include "matrix.h"

//Calculate the euclidean distance between row1 of matrix1 and row2 of matrix2
//Both matrices must have the same number of columns
//Returns NAN if a matrix is NULL, the numbers of columns differ or a row does not exist
double euclidean_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2);

//Find the k rows of train that are closest to row query_row of query (euclidean distance)
//indices must have room for k values: it receives the row numbers of train, from the closest to the farthest, and ties go to the smaller row number
//Returns 0 on success, or -1 if a matrix or indices is NULL, the numbers of columns differ, query_row does not exist, k is 0 or larger than the rows of train, or memory allocation fails
int knn_find_neighbors(const Matrix *train, const Matrix *query, size_t query_row, size_t k, size_t *indices);

#endif