#ifndef KNN_H
#define KNN_H

#include "matrix.h"

//The distance that knn_find_neighbors and knn_predict use to measure how close two rows are
//KNN_EUCLIDEAN is euclidean_distance and KNN_MANHATTAN is manhattan_distance
typedef enum{
    KNN_EUCLIDEAN,
    KNN_MANHATTAN
} KnnMetric;

//Calculate the euclidean distance between row1 of matrix1 and row2 of matrix2
//Both matrices must have the same number of columns
//Returns NAN if a matrix is NULL, the numbers of columns differ or a row does not exist
double euclidean_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2);

//Calculate the manhattan distance (sum of the absolute differences) between row1 of matrix1 and row2 of matrix2
//Both matrices must have the same number of columns
//Returns NAN if a matrix is NULL, the numbers of columns differ or a row does not exist
double manhattan_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2);

//Find the neighbor_count rows of train that are closest to row query_row of query measured with the distance chosen by metric
//indices must have room for neighbor_count values: it receives the row numbers of train, from the closest to the farthest, and ties go to the smaller row number
//Returns 0 on success, or -1 if a matrix or indices is NULL, the numbers of columns differ, query_row does not exist, neighbor_count is 0 or larger than the rows of train, metric is not a KnnMetric, the distance returns NAN or memory allocation fails
int knn_find_neighbors(const Matrix *train, const Matrix *query, size_t query_row, size_t neighbor_count, KnnMetric metric, size_t *indices);

//Choose the label that appears most among the neighbors: the labels are read from labels at the rows listed in indices
//indices is sorted from the closest neighbor to the farthest (as knn_find_neighbors returns it): on a tie, the label of the closest neighbor wins
//Returns the winning label, or NAN if labels or indices is NULL, labels does not have a single column, neighbor_count is 0 or an index is not a row of labels
double knn_frequent_label(const Matrix *labels, const size_t *indices, size_t neighbor_count);

//Predict a label for every row of test using the neighbor_count nearest rows of train, measured with the distance chosen by metric (labels come from train_labels)
//train_labels must have a single column and one row for each row of train; train and test must have the same number of columns
//Returns a new matrix with one row for each row of test and a single column, or NULL if a matrix is NULL, train_labels is not a single column or its rows differ from train, the numbers of columns of train and test differ, neighbor_count is 0 or larger than the rows of train, metric is not a KnnMetric, the distance returns NAN or memory allocation fails
Matrix *knn_predict(const Matrix *train, const Matrix *train_labels, const Matrix *test, size_t neighbor_count, KnnMetric metric);

#endif