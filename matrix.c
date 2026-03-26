
/*
 * Homework 5 – Pointer Operations
 * CSE 220 – Spring 2026
 *
 * Name: Brandon Moy
 * SBU ID: 116187802
 */

#include "matrix.h"
#include <stdio.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

/*----------------------------------------------------
  1. Sparse Matrix
----------------------------------------------------*/
int SparseMatrix(int D[2], int M[D[0]][D[1]],
                 int S[3][(D[1] > D[0] ? D[1] : D[0])])
{

    int rows = *(D);
    int cols = *(D + 1);
    int m = max(rows, cols);

    int nonZeroCount = 0;

    for (int r = 0; r < 3; r++){
      for (int c = 0; c < m; c++){
        *(*(S + r) + c) = 0;
      }
    }

    for (int i = 0; i < rows; i++){
      for (int j = 0; j<cols; j++){
        int cur = *(*(M + i) + j);
        if (cur!=0){
          if (nonZeroCount < m){
            *(*(S) + nonZeroCount) = i;
            *(*(S + 1) + nonZeroCount) = j;
            *(*(S + 2) + nonZeroCount) = cur;
          }
          nonZeroCount++;
        }
      }
    }

    if (nonZeroCount > m){
      return -1;
    }

    return nonZeroCount;
}


/*----------------------------------------------------
  2. Hadamard Product
----------------------------------------------------*/
int HadamardProduct(const int D[6],
                    const int M[D[0]][D[1]],
                    const int N[D[2]][D[3]],
                    int A[D[4]][D[5]])
{

    int mRows = D[0];
    int mCols = D[1];

    int nRows = D[2];
    int nCols = D[3];

    int aRows = D[4];
    int aCols = D[5];


    return 0;
}


/*----------------------------------------------------
  3. Matrix Multiplication
----------------------------------------------------*/
int Multiplication(const int D[6],
                   const int M[D[0]][D[1]],
                   const int N[D[2]][D[3]],
                   int A[D[4]][D[5]])
{

    int mRows = D[0];
    int mCols = D[1];

    int nRows = D[2];
    int nCols = D[3];

    int aRows = D[4];
    int aCols = D[5];


    return 0;
}


/*----------------------------------------------------
  4. Diagonal Sum
----------------------------------------------------*/
int DiagonalSum(const int D[4],
                const int A[D[0]][D[1]],
                int DS[D[2]][D[3]])
{

    int rows = D[0];
    int cols = D[1];

    int dsRows = D[2];
    int dsCols = D[3];


    return 0;
}
