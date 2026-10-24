#include <stdio.h>
#include <math.h>
#include "data.h"
#include "knn.h"
#include "metrics.h"

#define SEED 42
#define TRAIN_RATIO 0.8
#define NEIGHBOR_COUNT 5
#define SAMPLES_SHOWN 5

//Shuffles and splits the data, predicts the test rows with k-NN and prints a few of the predictions
//Returns the accuracy (from 0 to 1), or NAN if something fails
static double evaluate(const char *title, const Matrix *features, const Matrix *labels){
    double result = NAN;

    //Every matrix starts as NULL, so the cleanup at the end can free all of them (matrix_free(NULL) is safe)
    Matrix *shuffled_features = NULL;
    Matrix *shuffled_labels = NULL;
    Matrix *train_features = NULL;
    Matrix *test_features = NULL;
    Matrix *train_labels = NULL;
    Matrix *test_labels = NULL;
    Matrix *predicted = NULL;

    //The same seed gives the same order, so each row keeps its own label
    shuffled_features = data_shuffle(features, SEED);
    shuffled_labels = data_shuffle(labels, SEED);

    if(shuffled_features == NULL || shuffled_labels == NULL){
        printf("could not shuffle the data\n");
        goto cleanup;
    }

    //The same ratio on both, so the rows of features and labels still match
    if(data_split(shuffled_features, TRAIN_RATIO, &train_features, &test_features) == -1
       || data_split(shuffled_labels, TRAIN_RATIO, &train_labels, &test_labels) == -1){
        printf("could not split the data\n");
        goto cleanup;
    }

    predicted = knn_predict(train_features, train_labels, test_features, NEIGHBOR_COUNT);

    if(predicted == NULL){
        printf("could not predict\n");
        goto cleanup;
    }

    printf("--- %s ---\n", title);
    printf("train rows: %zu, test rows: %zu, neighbors: %d\n", train_features->rows, test_features->rows, NEIGHBOR_COUNT);

    for(size_t i = 0; i < SAMPLES_SHOWN && i < test_features->rows; i++){
        double guess = predicted->elements[i];
        double truth = test_labels->elements[i];

        printf("test row %zu: predicted %g, actual %g -> %s\n", i, guess, truth, guess == truth ? "right" : "WRONG");
    }

    result = accuracy(predicted, test_labels);

    printf("accuracy: %.2f%%\n\n", result * 100.0);

cleanup:
    matrix_free(shuffled_features);
    matrix_free(shuffled_labels);
    matrix_free(train_features);
    matrix_free(test_features);
    matrix_free(train_labels);
    matrix_free(test_labels);
    matrix_free(predicted);

    return result;
}

int main(void){
    int status = 1;

    Matrix *data = NULL;
    Matrix *features = NULL;
    Matrix *labels = NULL;
    Matrix *normalized_features = NULL;

    data = data_load_csv("data/wine.csv", 0);

    if(data == NULL){
        printf("could not load data/wine.csv\n");
        goto cleanup;
    }

    //In the Wine file the label is the first column
    if(data_separate_labels(data, 0, &features, &labels) == -1){
        printf("could not separate features and labels\n");
        goto cleanup;
    }

    normalized_features = data_normalize(features);

    if(normalized_features == NULL){
        printf("could not normalize the features\n");
        goto cleanup;
    }

    double raw = evaluate("without normalization", features, labels);
    double normalized = evaluate("with normalization", normalized_features, labels);

    if(isnan(raw) || isnan(normalized)){
        goto cleanup;
    }

    printf("normalization changed the accuracy from %.2f%% to %.2f%%\n", raw * 100.0, normalized * 100.0);

    status = 0;

cleanup:
    matrix_free(data);
    matrix_free(features);
    matrix_free(labels);
    matrix_free(normalized_features);

    return status;
}
