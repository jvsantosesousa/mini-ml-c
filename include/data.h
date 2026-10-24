#ifndef DATA_H
#define DATA_H

#include "matrix.h"

//Normalize each column in the range of 0 to 1 (min-max). A column whose values are all equal becomes 0.0
//Returns new matrix, or NULL if input is NULL or memory allocation fails
Matrix *data_normalize(const Matrix *matrix);

//Shuffle rows, same seed gives the same order
//Returns the shuffled matrix, or NULL if input is NULL or memory allocation fails
Matrix *data_shuffle(const Matrix *matrix, unsigned int seed);

////Split the rows of a matrix in two new matrices, train and test, without shuffling: the first rows go to train
//train_ratio is the fraction of rows for train (0.8 = 80%) and must be between 0 and 1, and both parts need at least one row
//Returns 0 on success, or -1 on failure (and then train and test are set to NULL)
int data_split(const Matrix *matrix, double train_ratio, Matrix **train, Matrix **test);

//Read a numeric CSV file into a new matrix: each line of the file becomes a row
//If has_header is not 0, the first line is skipped (it holds the column names)
//Returns NULL if path is NULL, the file cannot be opened, a line has a different number of columns, a field is empty or not a number, or memory allocation fails
Matrix *data_load_csv(const char *path, int has_header);

//Separate a matrix in two new matrices: features gets all the columns except label_column, and labels gets only label_column (as a single column)
//label_column is the index of the label column, starting at 0. Both results have the same number of rows as matrix
//Returns 0 on success, or -1 if matrix or an output address is NULL, label_column does not exist, matrix has a single column or memory allocation fails (then features and labels are set to NULL)
int data_separate_labels(const Matrix *matrix, size_t label_column, Matrix **features, Matrix **labels);

#endif