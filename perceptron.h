#ifndef PERCEPTRON_H_INCLUDED
#define PERCEPTRON_H_INCLUDED
#include "akmatrix.h"
#include "perconst.h"

/* Perceptron layer data */
typedef struct s_layer {
    int nrof_neurons;
    DEF_MAT(w,MAX_INPUTS,MAX_NEURONS);
    DEF_MAT(umbral,1,MAX_NEURONS);
    DEF_MAT(output,1,MAX_NEURONS);
} s_layer;

/* perceptron data */
typedef struct s_perceptron {
    int nrof_layers;
    int nrof_inputs;
    DEF_MAT(input,1,MAX_INPUTS); /* synapses */
    s_layer layer[MAX_LAYERS];
    double output;
} s_perceptron;


#endif // PERCEPTRON_H_INCLUDED
