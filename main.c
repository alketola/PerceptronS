#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nullptr.h"
#include "akmatrix.h"
#include "perceptron.h"
#include "perconst.h"

s_perceptron p;

int readLine(FILE *fp, char *line, int length){
    char *read;
    int success = 0;
    read = fgets(line,length,fp);//getline(&line, &len, fp);

    if ((read != NULLPTR)){
        success = 1;
    }
    return success;
}

int readIntFromFileLine(FILE *fp) {
    char line[MAX_LINE_LEN]=" ";
    int ret_val=0;
    /* read int */
    readLine(fp,line,MAX_LINE_LEN);
    ret_val=atoi(line);

    return ret_val;
}

void readWeights(FILE *fp,s_perceptron *p, int current_layer, int current_nrof_inputs,int current_nrof_neurons) {
    char line[MAX_LINE_LEN]; /* if you read longer line than MAX_LINE_LEN you will experience stack smashing */
    char *token=NULLPTR;
    double wij;

    readLine(fp, (char *)&line,MAX_LINE_LEN);
    printf("\nWeights line:%s",line);
    token = strtok(line," "); /* pick first space-separated word from line */
    for(int i =0;i<current_nrof_inputs;i++){
        for(int j = 0; j<current_nrof_neurons;j++){
            if(token!=NULLPTR){
                wij = atof(token);
                printf("\nw[%d][%d]=%1.6f",i,j,wij);
            }
            p->layer[current_layer].w[i][j]=wij; /* weight from input i to neuron j on current layer of p */

            token = strtok(NULLPTR," "); /* Pick next token. With NULL source strtok will remember last line */
        }
    }
    printf("\n");
    return;
}

void readUmbrales(FILE *fp, s_perceptron *p, int current_layer, int current_nrof_neurons) {
    char line[MAX_LINE_LEN];
    char *token;
    double umb;
    readLine(fp, (char *)&line,MAX_LINE_LEN);
    printf("\nUmbral line:%s",line);
    token = strtok(line," ");
    for(int j = 0; j<current_nrof_neurons;j++){
        if(token!=NULLPTR){
            umb = atof(token);
            printf("\numbral[%d]=%1.6f",j,umb);
            p->layer[current_layer].umbral[0][j]=umb; /*  umbral of neuron j on current layer of perceptron */
            token = strtok(NULLPTR," ");
        }
    }
    printf("\n");
}

/* This function reads the configuration file and initializes coefficients etc */
void initPerceptron(s_perceptron *p, char *config_file_name) {
    FILE *config_file;

    int par_n_layers = 0;
    int par_n_inputs = 0;

    config_file = fopen(config_file_name,"r");
    if (config_file == NULLPTR) {
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
    allocMat(p->input,1,MAX_INPUTS);
    int current_layer;
    int current_nrof_inputs = par_n_inputs;
    int current_nrof_neurons = 0;
    for(current_layer=0;current_layer<par_n_layers;current_layer++){
        /* Will have to (m)alloc-ate the weights matrix for each layer */
        allocMat(p->layer[current_layer].w,MAX_INPUTS,MAX_NEURONS); /* allocate memory for weight matrix */
        allocMat(p->layer[current_layer].output,1,MAX_NEURONS);
        allocMat(p->layer[current_layer].umbral,1,MAX_NEURONS);
        printf("\nLoading layer #%d",current_layer);
        current_nrof_neurons = readIntFromFileLine(config_file);
        printf("\n Number of inputs:  %d",current_nrof_inputs);
        printf("\n Number of neurons: %d",current_nrof_neurons);
        p->layer[current_layer].nrof_neurons=current_nrof_neurons;
        readWeights(config_file,p,current_layer,current_nrof_inputs,current_nrof_neurons);
        printf("\n readweights done");
        readUmbrales(config_file,p,current_layer,current_nrof_neurons);
        current_nrof_inputs = current_nrof_neurons; /* store fir next round */
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
    printf("\n");
    int L=0;
    printf("Layer %d:",L);
    printMat(p->layer[L].w," Weights",p->nrof_inputs,p->layer[L].nrof_neurons,6);
    printMat(p->layer[L].umbral," Umbrals",1,p->layer[L].nrof_neurons,6);
    printMat(p->layer[L].output," LayerOutput",1,p->layer[L].nrof_neurons,6);
    for(L=1;L<p->nrof_layers;L++){
        printf("Layer %d:",L);
        printMat(p->layer[L].w," Weights",p->layer[L-1].nrof_neurons,p->layer[L].nrof_neurons,6);
        printMat(p->layer[L].umbral," Umbrals",1,p->layer[L].nrof_neurons,6);
        printMat(p->layer[L].output," LayerOutput",1,p->layer[L].nrof_neurons,6);
        printf("\n");
    }

/*    for(L=0;L<p->nrof_layers;L++){
        printf("\nlayer=%d",L);
        for(int i =0;i<MAX_INPUTS;i++){
            for(int j = 0; j<MAX_NEURONS;j++){
                printf("\n  input=%d neuron=%d ",i,j);
                double wij= p->layer[L].w[i][j];
                printf(" weight=%f",wij);
            }
        }
        for(int j = 0; j<MAX_NEURONS;j++){
            float umb = p->layer->umbral[0][j];
            printf("\n Layer[%d] Umbral of neuron[%d]=%f",L,j,umb);
        }
    }
    */
    //double input[MAX_INPUTS]; /* synapses */
    //double output; /* axon single output value */
}

double activation_f(double x){

    return pow((1 + exp(-20*x)),-1);

}

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

    //pseudo: matMul(p->layer[L].input,p->weights,p->layer[L].output)
    int L=0;
    //char outputname[20];
    /* Process layer 0 from input data */
    matMul(p->input,p->layer[L].w,p->layer[L].output,1,p->nrof_inputs,p->layer[L].nrof_neurons);
    subMat(p->layer[L].output,p->layer[L].umbral,1,p->layer[L].nrof_neurons);
    applyFxMat(p->layer[L].output,1,p->layer[L].nrof_neurons,activation_f);
    //printMat(p->layer[L].output,"layer[0]output",1,p->nrof_inputs,6);

    /* Process layer 1 onwards from the output of the previous layer */
    for(L=1; L < p->nrof_layers; L++ ) {
        matMul(p->layer[L-1].output,p->layer[L].w,p->layer[L].output,1,p->layer[L-1].nrof_neurons,p->layer[L].nrof_neurons);
        //printf("\n multiplied: ");
        //sprintf(outputname,"layer[%d]output",L);
        //printMat(p->layer[L].output,outputname,1,p->layer[L].nrof_neurons,6);

        subMat(p->layer[L].output,p->layer[L].umbral,1,p->layer[L].nrof_neurons);
        //printf("\n biased: ");
        //printMat(p->layer[L].output,outputname,1,p->layer[L].nrof_neurons,6);

        applyFxMat(p->layer[L].output,1,p->layer[L].nrof_neurons,activation_f);
        //printf("\n after activation function applied");
        //printMat(p->layer[L].output,outputname,1,p->layer[L].nrof_neurons,6);
    }
    p->output = p->layer[L-1].output[0][0];

}

void executePerceptron(s_perceptron *p, char *input_file_name) {
    printf("\n\n\nExecution of the Perceptron");
    printf(    "\n===========================");
    FILE *input_file;
    char *token;
    char input_line[MAX_LINE_LEN];
    input_file = fopen(input_file_name,"r");
    if (input_file == NULLPTR) {
        printf("Error: input_file [%s] not found",input_file_name);
        exit(EXIT_FAILURE);
    }


    while(readLine(input_file,(char *)&input_line,MAX_LINE_LEN)){
        printf("\nInput : (");
        token = strtok(input_line," ");
        double input;
        for(int i = 0; i<p->nrof_inputs;i++){
            if(token!=NULLPTR){
                input = atof(token);
                printf("%1.6f ",input);
                p->input[0][i]=input;
                token = strtok(NULLPTR," ");
            }
            if(i<p->nrof_inputs-1) printf(", ");
            else printf(") -> ");
        }
        processPerceptronInput(p);
        printf("      Output:(%6f)",p->output);
    }

}

int main()
{
    //test_matmul();
    //test_applyFx();
    //exit(EXIT_SUCCESS);

    initPerceptron(&p,"configuration.txt");
    //dumpPerceptron(&p);

    executePerceptron(&p,"entrada.txt");
    printf("\n");
    return 0;

}
