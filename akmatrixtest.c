/**
* Test functions for akmatrix.c
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "akmatrix.h"

/**
* Test matrix multiplication
*/
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
        printMat(m3,"m3 the result",dim_m2_row, dim_m2_col,3);
    } else {
        printf("\nnull result from matmul");
    }

    int comparison = compareMat(m3, expected_result_mat, dim_m1_row, dim_m1_col,0.0001);
    printf("\n m3 compares to expected: %d",comparison);
}

/**
* A function to test
*   applyFxMat(matrix,rows,cols, function_pointer)
*
*/
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
