/**
* A matrix handling package based on the example in
* Korpela & Larmela: C-ohjelmointikieli, OtaData, 1986,(pages 80-81).
* It was the C programming language book of Helsinki
* University of Technology, pretty much the same as K&R C book.)
*
* Modernized and expanded by Antti Ketola
*/

#include <stdio.h>
#include <stdlib.h>
#include "akmatrix.h"

void allocMat(matrix a, int m, int n) {

    for(int i=0;i<m;i++){
        a[i] = (double *) malloc(n * sizeof(double));
    }
}

void setMat(matrix a, int m, int n, double value) {

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            a[i][j] = value;
        }
    }
}

void applyFxMat(matrix a, int m, int n, double (*fx)(double x)) {

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            a[i][j] = fx(a[i][j]);
        }
    }
}


void addMat(matrix a, matrix b, int m, int n) {

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            a[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subMat(matrix a, matrix b, int m, int n) {

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            a[i][j] = a[i][j] - b[i][j];
        }
    }
}


/**
* Copies values from matrix a to matrox b
* a and b must have same dimensions
*/
void copyMat(matrix a, matrix b, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            b[i][j] = a[i][j];
        }
    }


}

/**
* Reads floating point numbers to the matrix a from the console
*/
int readMat(matrix a, int m, int n) {

    int count = 0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(scanf("%lf",&a[i][j]) != 1) {
                return count;
            } else {
                count++;
            }
        }
    }
    return count;
}

/**
* Prints the matrix a of dimensions m x n.
* w is the field width of each element in the printout
*
*/
void printMat(matrix a, char *matname, int m, int n, int w) {
    char printformat[10];
    //printf("\nPRINTMAT:\n");
    sprintf(printformat, " %%f%%c",w); /* adding the desired char field width to the end */
    //printf("\n FORMAT=%s\n",printformat);
    printf("\n%s:",matname);
    printf("\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf(printformat,a[i][j], (j==n-1) ? '\n' : ' ');
        }
    }

}

/****
*
* Matrix multiplication
*
* matrix a of doubles, dimensions dim_a_row by dim_a_col
* matrix b of doubles, dimensions dim_b_row by dim_b_col
*
* matrix b must have the same y- dimension as a has the x-dimension,
*
* returns result in c which must be previously allocated
*
*
*/
void matMul(matrix a, matrix b, matrix c, int dim_a_row, int dim_a_col, int dim_b_row) {

    double sum;
    //printf("\nMatMul a_row=%d, a_col=%d, b_row=%d\n", dim_a_row, dim_a_col, dim_b_row);

    for(int i=0;i<dim_a_row;i++) {
        for(int j=0;j<dim_b_row;j++) {
            sum=0;
            for(int k=0;k<dim_a_col;k++){
                sum +=a[i][k]*b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

/**
* Calculates matrix sum of a and b to c
* All three matrices must be equals in dimensions
*/
void matSum(matrix a, matrix b, matrix c, int dim_rows, int dim_cols) {

    printf("\nMatSum rows=%d, cols=%d\n", dim_rows, dim_cols);

    for(int i=0;i<dim_rows;i++) {
        for(int j=0;j<dim_cols;j++) {
            c[i][j] =a[i][j] + b[i][j];
        }
    }
}
/**
* Multiplies a matrix a with a scalar "multiplier".
* Result in matrix c, which must have the same dimensions as matrix a.
*/
void matScal(matrix a, matrix c, int dim_rows, int dim_cols, double multiplier) {

    printf("\nMatScal rows=%d, cols=%d, multiplier=%f\n", dim_rows, dim_cols, multiplier);

    for(int i=0;i<dim_rows;i++) {
        for(int j=0;j<dim_cols;j++) {
            c[i][j] = a[i][j] * multiplier;
        }
    }
}

/**
* Transposes the matrix a storing result in c.
*
* If the matrix a has dim_a_row rows and dim_a_col columns,
* the matrix c MUST have dim_a_col rows and dim_a_row columns.
*/

void matTranspose(matrix a, matrix c, int dim_rows, int dim_cols) {
    for(int i=0;i<dim_rows;i++) {
        for(int j=0;j<dim_rows;j++) {
            c[i][j] = a[j][i];
        }
    }
}

/**
* Fills matrix a as unit matrix, i.e.
* the values at the diagonal are set to 1
* otherwise to 0
*/
void unitMat(matrix a, int m, int n) {

    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(i==j)
                a[i][j] = 1.0;
            else
                a[i][j] = 0.0;
        }
    }
}

/**
* Copy array of N elements of doubles to
* 1 x N matrix of doubles element by element.
*
* The matrix m must be defined DEF_MAT(m,1,N)
* and allocated allocMat(m,1,N)
*/
void array2matX1(double a[],matrix m, int N) {
    for(int i=0;i<N;i++) {
        m[0][i] = a[i];
    }
}

/**
* Compares matrices a and b element by element for equality
* While floating point numbers (doubles) are not always exact
* a comparison tolerance is introduced.
*
*/

int compareMat(matrix a, matrix b, int dim_row, int dim_col, double tolerance){

    for(int i=0;i<dim_row;i++) {
        for(int j=0;j<dim_col;j++) {
            if(abs(a[i][j]-b[i][j])>tolerance) {
                //printf("\nOut of tolerance %d at i=%d j=%d ",tolerance,i,j);
                return 0;
            }
        }
    }
    return 1;
}



