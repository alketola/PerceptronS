#ifndef AKMATRIX_H_INCLUDED
#define AKMATRIX_H_INCLUDED

#define DEF_MAT(a,m,n) double *a[m]
typedef double *matrix[];
extern int readMat(matrix a, int m, int n);
extern void allocMat(matrix a, int m, int n);
extern void setMat(matrix a, int m, int n, double value);
extern void addMat(matrix a, matrix b, int m, int n);
extern void subMat(matrix a, matrix b, int m, int n);
extern void copyMat(matrix a, matrix b, int rows, int cols);
extern void printMat(matrix a, char *matrix_name, int m, int n, int w);
extern void matMul(matrix a, matrix b, matrix c, int dim_a_row, int dim_a_col, int dim_b_row);


extern void test_matmul();


#endif // AKMATRIX_H_INCLUDED
