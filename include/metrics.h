#ifndef METRICS_H
#define METRICS_H

#include "matrix.h"

//Returns the fraction of lines in which predicted and actual have the same value, from 0 to 1, 1 being 100% accurate
//Returns NAN if an argument is NULL or the matrices have different sizes
double accuracy(const Matrix *predicted, const Matrix *actual);

#endif