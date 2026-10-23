#include "activation.h"
#include <math.h>

double sigmoid(double x){
    return 1.0 / (1.0 + exp(-x));
}

double relu(double x){
    return x > 0.0 ? x : 0.0;
}