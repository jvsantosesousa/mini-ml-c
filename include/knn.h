#ifndef KNN_H
#define KNN_H

#include "matrix.h"

//Calculate the euclidean distance between row1 of matrix1 and row2 of matrix2
//Both matrices must have the same number of columns
//Returns NAN if a matrix is NULL, the numbers of columns differ or a row does not exist
double euclidean_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2);

//Find the neighbor_count rows of train that are closest to row query_row of query (euclidean distance)
//indices must have room for neighbor_count values: it receives the row numbers of train, from the closest to the farthest, and ties go to the smaller row number
//Returns 0 on success, or -1 if a matrix or indices is NULL, the numbers of columns differ, query_row does not exist, neighbor_count is 0 or larger than the rows of train, or memory allocation fails
int knn_find_neighbors(const Matrix *train, const Matrix *query, size_t query_row, size_t neighbor_count, size_t *indices);

//Choose the label that appears most among the neighbors: the labels are read from labels at the rows listed in indices
//indices is sorted from the closest neighbor to the farthest (as knn_find_neighbors returns it): on a tie, the label of the closest neighbor wins
//Returns the winning label, or NAN if labels or indices is NULL, labels does not have a single column, neighbor_count is 0 or an index is not a row of labels
double knn_vote(const Matrix *labels, const size_t *indices, size_t neighbor_count);

#endif