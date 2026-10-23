#ifndef ACTIVATION_H
#define ACTIVATION_H

//Returns the sigmoid of x, a value between 0 and 1: 1 / (1 + e^(-x))
double sigmoid(double x);

//Returns x if x > 0 if not returns 0
double relu(double x);

#endif