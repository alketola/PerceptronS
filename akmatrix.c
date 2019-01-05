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
    sprintf(printformat, " %%.%de%%c",w); /* adding the desired char field width to the end */
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
    if(dim_a_row != dim_b_row) {
        printf("\nError: matmul: matrix a x-dimension cannot be different from y-dimension of matrix b.");
        exit(EXIT_FAILURE);
    }

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

void array2matX1(double a[],matrix m, int count) {
    for(int i=0;i<count;i++) {
        m[0][i] = a[i];
    }
}

int compareMat(matrix a, matrix b, int dim_row, int dim_col, double tolerance){

    for(int i=0;i<dim_row;i++) {
        for(int j=0;j<dim_col;j++) {
            if(abs(a[i][j]-b[i][j])>tolerance) {
                printf("\nOut of tolerance %d at i=%d j=%d ",tolerance,i,j);
                return 0;
            }
        }
    }
    return 1;
}

void test_matmul() {
    int dim_m1_row = 2;
    int dim_m1_col = 2;
    DEF_MAT(m1, dim_m1_row, dim_m1_col);
    allocMat(m1,dim_m1_row, dim_m1_col);
    setMat(m1,dim_m1_row, dim_m1_col,0);

    int dim_m2_row = 2;
    int dim_m2_col = 2;
    DEF_MAT(m2, dim_m2_row, dim_m2_col);
    allocMat(m2,dim_m2_row, dim_m2_col);
    setMat(m2,dim_m2_row, dim_m2_col,0);

    m1[0][0] = 1.00;
    m1[0][1] = 2.00;
    m1[1][0] = 3.00;
    m1[1][1] = 4.00;

    printMat(m1,"m1",dim_m1_row, dim_m1_col,3);

    m2[0][0] = 5.00;
    m2[0][1] = 6.00;
    m2[1][0] = 7.00;
    m2[1][1] = 8.00;

    printMat(m2,"m2",dim_m2_row, dim_m2_col,3);

    DEF_MAT(m3,dim_m1_row,dim_m1_col);
    allocMat(m3,dim_m1_row,dim_m2_col);
    setMat(m3,dim_m1_row, dim_m1_col,0);

    DEF_MAT(expected_result_mat,dim_m1_row, dim_m1_col);
    allocMat(expected_result_mat,dim_m1_row, dim_m1_col);
    setMat(expected_result_mat,dim_m1_row, dim_m1_col,0);

    expected_result_mat[0][0] = 19.00;
    expected_result_mat[0][1] = 22.00;
    expected_result_mat[1][0] = 43.00;
    expected_result_mat[1][1] = 50.00;

    matMul(m1,m2, m3, dim_m1_row,dim_m1_col,dim_m2_row);
    if(m3!=NULL) {
        printf("\nWe have a result!");
        for(int i=0;i<dim_m1_row;i++) {
            for(int j=0;j<dim_m1_col;j++) {
                printf("\n m3[%d][%d]=%f",i,j,m3[i][j]);
            }
        }
    } else {
        printf("\nnull result from matmul");
    }

    int comparison = compareMat(m3, expected_result_mat, dim_m1_row, dim_m1_col,0.0001);
    printf("\n m3 compares to expected: %d",comparison);
}

double testFx(double d) {
    return 2 * d;
}

void test_applyFx() {

    DEF_MAT(m1, 2, 2);
    allocMat(m1,2,2);
    setMat(m1,2,2,0);

    m1[0][0] = 1.00;
    m1[0][1] = 2.00;
    m1[1][0] = 3.00;
    m1[1][1] = 4.00;

    printMat(m1,"m1 ennen",2,2,6);

    applyFxMat(m1,2,2,testFx);

    printMat(m1,"m1 jälkeen",2,2,6);

    DEF_MAT(expected_result_mat,2,2);
    allocMat(expected_result_mat,2,2);
    setMat(expected_result_mat,2,2,0);

    expected_result_mat[0][0] = 2.00;
    expected_result_mat[0][1] = 4.00;
    expected_result_mat[1][0] = 6.00;
    expected_result_mat[1][1] = 8.00;
    printMat(expected_result_mat,"expected ",2,2,6);
    int comparison = compareMat(m1, expected_result_mat, 2, 2, 0.0001);
    printf("\n m1 compares to expected: %d",comparison);

}
