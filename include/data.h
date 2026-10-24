#ifndef DATA_H
#define DATA_H

#include "matrix.h"

//Normalize each column in the range of 0 to 1 (min-max). A column whose values are all equal becomes 0.0
//Returns new matrix, or NULL if input is NULL or memory allocation fails
Matrix *data_normalize(const Matrix *matrix);

//Shuffle rows, same seed gives the same order
//Returns the shuffled matrix, or NULL if input is NULL or memory allocation fails
Matrix *data_shuffle(const Matrix *matrix, unsigned int seed);

#endif