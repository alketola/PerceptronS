#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "akmatrix.h"

/* Define maximum dimensions */
/* Using fixed tables with fixed sizes, making life limited thus esier */
#define MAX_LAYERS 5
#define MAX_NEURONS 5
#define MAX_INPUTS 5

#define LINE_LEN 120

typedef struct s_neuron {
    double in[MAX_INPUTS];
    double out;
} s_neuron;

typedef struct s_layer {
    int nrof_neurons;
    double w[MAX_INPUTS][MAX_NEURONS];
    double umbral[MAX_NEURONS];
    s_neuron neuron[MAX_NEURONS];
} s_layer;

typedef struct s_perceptron {
    int nrof_layers;
    int nrof_inputs;
    double input[MAX_INPUTS]; /* synapses */
    s_layer layer[MAX_LAYERS];
    double output; /* axon single output value */
} s_perceptron;

struct s_perceptron p;

int readLine(FILE *fp, char *line, int length){
    size_t len = 0;
    int read;
    int success = 0;
    read = fgets(line,length,fp);//getline(&line, &len, fp);

    if ((read != NULL)){
        success = 1;
    }
    return success;
}

int readIntFromFileLine(FILE *fp) {
    char line[LINE_LEN]="Nothing";
    size_t len = 0;
    int ret_val=0;
    /* read int */
    readLine(fp,line,120);
    ret_val=atoi(line);

    return ret_val;
}

void readWeights(FILE *fp,s_perceptron *p, int current_layer, int current_nrof_inputs,int current_nrof_neurons) {
    char line[ MAX_INPUTS + 1 * 10 ];
    char *token;
    double wij;
    int layer_nr=0;

    readLine(fp, &line,LINE_LEN);
    printf("\nWeights line:%s",line);
    token = strtok(line," ");
    for(int i =0;i<current_nrof_inputs;i++){
        for(int j = 0; j<current_nrof_neurons;j++){
            if(token!=NULL){
                wij = atof(token);
                printf("\nw[%d][%d]=%1.6f",i,j,wij);
            }
            p->layer[current_layer].w[i][j]=wij;
            token = strtok(NULL," ");
        }
    }
}

void readUmbrales(FILE *fp, s_perceptron *p, int current_layer, int current_nrof_neurons) {
    char line[LINE_LEN];
    char *token;
    double umb;
    readLine(fp, &line,LINE_LEN);
    printf("\nUmbral line:%s",line);
    token = strtok(line," ");
    for(int j = 0; j<current_nrof_neurons;j++){
        if(token!=NULL){
            umb = atof(token);
            printf("\numbral[%d]=%1.6f",j,umb);
            p->layer[current_layer].umbral[j]=umb;
            token = strtok(NULL," ");
        }
    }


}

/* This function reads the configuration file and initializes coefficients etc */
void initPerceptron(s_perceptron *p, char *config_file_name) {
    FILE *config_file;
    char * line = NULL;
    size_t len = 0;
    int read;

    int par_n_layers = 0;
    int par_n_inputs = 0;
    int success = 0;

    config_file = fopen(config_file_name,"r");
    if (config_file == NULL) {
        printf("Error: Config File [%s] not found",config_file_name);
        exit(EXIT_FAILURE);
    }
    /* read number of layers */
    par_n_layers = readIntFromFileLine(config_file);
    par_n_layers = par_n_layers > MAX_LAYERS ? MAX_LAYERS : par_n_layers;
    /* read number of inputs */
    par_n_inputs = readIntFromFileLine(config_file);
    par_n_inputs = par_n_inputs > MAX_INPUTS ? MAX_INPUTS : par_n_inputs;

    /* read each layer data */
    printf("\nLoading Perceptron");
    printf("\n==================");
    printf("\nNumber of layers:%d", par_n_layers);
    printf("\nNumber of inputs:%d", par_n_inputs);
    p->nrof_layers=par_n_layers;
    p->nrof_inputs=par_n_inputs;
    int current_layer;
    int current_nrof_inputs = par_n_inputs;
    int current_nrof_neurons = 0;
    for(current_layer=0;current_layer<par_n_layers;current_layer++){
        printf("\nLoading layer #%d",current_layer);
        current_nrof_neurons = readIntFromFileLine(config_file);
        printf("\n Number of inputs:  %d",current_nrof_inputs);
        printf("\n Number of neurons: %d",current_nrof_neurons);
        p->layer[current_layer].nrof_neurons=current_nrof_neurons;
        readWeights(config_file,p,current_layer,current_nrof_inputs,current_nrof_neurons);
        readUmbrales(config_file,p,current_layer,current_nrof_neurons);
    }

}

void dumpPerceptron(s_perceptron *p) {
    printf("\n\nDUMP perceptron:");
    printf("\n- - - - - - - - - ");
    printf("\n nrof_layers=%d",p->nrof_layers);
    printf("\n nrof_inputs=%d",p->nrof_inputs);
    printf("\n MAX_INPUTS=%d",MAX_INPUTS);
    printf("\n MAX_LAYERS=%d",MAX_LAYERS);
    printf("\n MAX_NEURONS=%d",MAX_NEURONS);
    printf("\n layer->%d",p->layer);
    int L;
    for(L=0;L<MAX_LAYERS;L++){
        printf("\nlayer=%d",L);
        for(int i =0;i<MAX_INPUTS;i++){
            for(int j = 0; j<MAX_NEURONS;j++){
                float wij= p->layer[L].w[i][j];
                printf("\n  input=%d neuron=%d weight=%f",i,j,wij);
            }
        }
        for(int j = 0; j<MAX_NEURONS;j++){
            float umb = p->layer->umbral[j];
            printf("\n Layer[%d] Umbral[%d]=%f",L,j,umb);
        }
    }
    //double input[MAX_INPUTS]; /* synapses */
    //double output; /* axon single output value */
}

double activation_f(double x){

    return 1 / (1 + pow(M_E,-20*x));

}
/*
void processPerceptronLayer(matrix input, matrix output, s_perceptron *p) {

    DEF_MAT(current_inputs,1,MAX_NEURONS);
    allocMat(current_inputs,1,MAX_NEURONS);
    copyMat(inputs,current_inputs,1,nrof_inputs);
    int current_nrof_inputs = nrof_inputs;
    int current_nrof_outputs = 0;
    for(int l=0;l < p->nrof_layers;l++) {
        for(int n=0;n < p->layer[l].nrof_neurons;n++) {
            for(int i=0;i<nrof_inputs;i++) {
                weights[i][n]=p->layer[l].w[i][n];
            }
        }
        matMul(current_inputs,weights,outputs,current_nrof_inputs,);

        copyMat(outputs,current_inputs,1,nrof_inputs);
        current_nrof_outputs = p->layer[l].nrof_neurons;
        current_nrof_inputs = nrof_outputs;
    }

}
*/
/**
* 1. copy input data to first layer inputs to Perceptron layer input
* 2. loop through layers in perceptron:
* 2.1    process a layer:
* 2.1.1      r1 = multiply inputs by weights
* 2.1.2      r2 = add negative of umbral to each node r1
* 2.1.3      r3 = apply activation function to r2
* 2.1.4      store layer output(r3)
* 2.2    prepare for next layer
* 2.2.1      copy output data from previous layer to next layer input
*/

void processPerceptronInput(s_perceptron *p) {
    DEF_MAT(weights,MAX_INPUTS,MAX_NEURONS);
    allocMat(weights,MAX_INPUTS,MAX_NEURONS);
    setMat(weights,MAX_INPUTS,MAX_NEURONS,0.0);

    DEF_MAT(outputs,1,MAX_NEURONS);
    allocMat(outputs,1,MAX_NEURONS);
    setMat(outputs,1,MAX_NEURONS,0.0);

    //pseudo: matMul(p->layer[L].input,p->weights,p->layer[L].output)


}

void executePerceptron(s_perceptron *p, char *input_file_name) {
    printf("\n\n\nExecution of the Perceptron");
    printf(    "\n===========================");
    FILE *input_file;
    char *token;
    char input_line[LINE_LEN];
    input_file = fopen(input_file_name,"r");
    if (input_file == NULL) {
        printf("Error: input_file [%s] not found",input_file_name);
        exit(EXIT_FAILURE);
    }


    while(readLine(input_file,&input_line,LINE_LEN)){
        printf("\nInput : (");
        token = strtok(input_line," ");
        double input;
        for(int i = 0; i<p->nrof_inputs;i++){
            if(token!=NULL){
                input = atof(token);
                printf("%1.6f ",input);
                p->input[i]=input;
                token = strtok(NULL," ");
            }
            if(i<p->nrof_inputs-1) printf(", ");
            else printf(") -> ");
        }
        processPerceptronInput(&p);
        printf("      Output:(%1.6f)",p->output);
    }

}

int main()
{
    test_matmul();
    /*
    initPerceptron(&p,"configuration.txt");
    dumpPerceptron(&p);
    executePerceptron(&p,"entrada.txt");
    printf("\n");
    return 0;
    */
}
