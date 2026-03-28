
#include <criterion/criterion.h>
#include "matrix.h"

// 1 - All zeros matrix
Test(SparseMatrix, all_zeros) {
    int D[2] = {2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int S[3][2];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 0);
}

// 2 - Normal Case zero at beginning
Test(SparseMatrix, single_element_start) {
    int D[2] = {3, 2};
    int M[3][2] = {{5, 0}, {0, 0}, {0, 0}};
    int S[3][3]; 
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 0); 
    cr_assert_eq(*(*(S+2)+0), 5);
}

// 3 - Normal Case zero at the end
Test(SparseMatrix, single_element_end) {
    int D[2] = {2, 3};
    int M[2][3] = {{0, 0, 0}, {0, 0, 99}};
    int S[3][3]; 
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 1); 
    cr_assert_eq(*(*(S+1)+0), 2); 
    cr_assert_eq(*(*(S+2)+0), 99); 
}

// 4 - Rectangular: Rows > Cols
Test(SparseMatrix, rectangular_rows_sparse) {
    int D[2] = {4, 2}; 
    int M[4][2] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}}; 
    int S[3][4];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 4); 
}

// 5. - Rectangular: Cols > Rows
Test(SparseMatrix, rectangular_cols_not_sparse) {
    int D[2] = {2, 4}; 
    int M[2][4] = {{1, 2, 3, 0}, {4, 5, 0, 0}}; 
    int S[3][4];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, -1);
}

// 6 - Normal case: Matrix has negative values
Test(SparseMatrix, negative_values) {
    int D[2] = {2, 2};
    int M[2][2] = {{0, -10}, {5, 0}};
    int S[3][2];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 2);
    cr_assert_eq(*(*(S+2)+0), -10); 
}

// 7 1x1 Zero matrix
Test(SparseMatrix, one_by_one_zero) {
    int D[2] = {1, 1};
    int M[1][1] = {{0}};
    int S[3][1];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 0);
}

// 8 - 1x1 Non-zero matrix
Test(SparseMatrix, one_by_one_nonzero) {
    int D[2] = {1, 1};
    int M[1][1] = {{42}};
    int S[3][1];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+2)+0), 42);
}

// 9 - S should have padded zeros
Test(SparseMatrix, padding_check) {
    int D[2] = {3, 3}; 
    int M[3][3] = {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}}; 
    int S[3][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}; 
    SparseMatrix(D, M, S);
    cr_assert_eq(*(*(S+0)+1), 0); 
    cr_assert_eq(*(*(S+2)+2), 0); 
}

// 10 - Large dimensions
Test(SparseMatrix, large_sparse_check) {
    int D[2] = {10, 2}; 
    int M[10][2] = {{0}};
    *(*(M+9)+1) = 50; 
    int S[3][10];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 9);
    cr_assert_eq(*(*(S+1)+0), 1);
}

//===========================================================================================================================================

// 1 - Normal case: Perfect fit
Test(HadamardProduct, perfect_fit) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{5, 6}, {7, 8}};
    int A[2][2];
    cr_assert_eq(HadamardProduct(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 5);
}

// 2- M and N compatible but A oversized
Test(HadamardProduct, oversized_A) {
    int D[6] = {2, 2, 2, 2, 3, 3};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{2, 2}, {2, 2}};
    int A[3][3];
    cr_assert_eq(HadamardProduct(D, M, N, A), 2);
    cr_assert_eq(*(*(A+1)+1), 8);
    cr_assert_eq(*(*(A+2)+2), 0); 
}

// 3 - M and N compatible but A undersized
Test(HadamardProduct, undersized_A_compatible_MN) {
    int D[6] = {2, 2, 2, 2, 1, 1};
    int M[2][2] = {{10, 10}, {10, 10}};
    int N[2][2] = {{2, 2}, {2, 2}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), -3);
}

// 4 - M and N incompatible but A fits intersection
Test(HadamardProduct, incompatible_MN_A_fits) {
    int D[6] = {3, 3, 2, 2, 3, 3};
    int M[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int N[2][2] = {{2, 5}, {1, 4}};
    int A[3][3];
    cr_assert_eq(HadamardProduct(D, M, N, A), -1);
    cr_assert_eq(*(*(A+0)+0), 2); 
    cr_assert_eq(*(*(A+1)+1), 20); 
}

// 5 - M and N incompatible and A too small for intersection
Test(HadamardProduct, incompatible_MN_A_small) {
    int D[6] = {3, 3, 2, 2, 1, 1};
    int M[3][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
    int N[2][2] = {{3, 3}, {3, 3}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), -2);
}

// 6 - 1x1 matrix
Test(HadamardProduct, one_by_one_matrices) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{5}};
    int N[1][1] = {{10}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 50);
}

// 7 - Negative values
Test(HadamardProduct, negative_values) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{-2}};
    int N[1][1] = {{4}};
    int A[1][1];
    HadamardProduct(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), -8);
}

// 8 - All zeroes
Test(HadamardProduct, zero_matrices) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int N[2][2] = {{1, 1}, {1, 1}};
    int A[2][2];
    HadamardProduct(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 0);
}

// 9 - Largely oversized A
Test(HadamardProduct, very_oversized_A) {
    int D[6] = {1, 1, 1, 1, 5, 5};
    int M[1][1] = {{2}};
    int N[1][1] = {{3}};
    int A[5][5];
    cr_assert_eq(HadamardProduct(D, M, N, A), 2);
}

// 10 - Non-square intersection
Test(HadamardProduct, rectangular_intersection) {
    int D[6] = {5, 2, 2, 5, 5, 5};
    int M[5][2] = {{1, 1}, {1, 1}, {0, 0}, {0, 0}, {0, 0}};
    int N[2][5] = {{2, 2, 0, 0, 0}, {2, 2, 0, 0, 0}};
    int A[5][5];
    cr_assert_eq(HadamardProduct(D, M, N, A), -1);
}

//=======================================================================================================================================================

// 1 - Perfect fit
Test(Multiplication, perfect_fit) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{5, 6}, {7, 8}};
    int A[2][2];
    cr_assert_eq(Multiplication(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 19); 
}

// 2 - Compatible but oversized A
Test(Multiplication, oversized_A) {
    int D[6] = {2, 2, 2, 2, 4, 4};
    int M[2][2] = {{1, 0}, {0, 1}};
    int N[2][2] = {{5, 5}, {5, 5}};
    int A[4][4];
    cr_assert_eq(Multiplication(D, M, N, A), 2);
}

// 3 - Compatible but undersized A
Test(Multiplication, undersized_A_compatible) {
    int D[6] = {3, 2, 2, 3, 2, 4}; 
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[2][3] = {{1, 1, 1}, {1, 1, 1}};
    int A[2][4];
    cr_assert_eq(Multiplication(D, M, N, A), -3);
}

// 4 - Incompatible but A fits partial result
Test(Multiplication, incompatible_A_fits) {
    int D[6] = {3, 2, 3, 2, 4, 3}; //
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[3][2] = {{2, 2}, {2, 2}, {2, 2}};
    int A[4][3];
    cr_assert_eq(Multiplication(D, M, N, A), -1);
}

// 5 - Incompatible and A too small for partial result
Test(Multiplication, incompatible_A_small) {
    int D[6] = {3, 2, 3, 2, 2, 2}; 
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[3][2] = {{2, 2}, {2, 2}, {2, 2}};
    int A[2][2];
    cr_assert_eq(Multiplication(D, M, N, A), -2);
}

// 6 - 1x1
Test(Multiplication, identity_1x1) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{10}};
    int N[1][1] = {{2}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 20);
}

// 7 - Compatible: Rectangular
Test(Multiplication, rectangular_comp) {
    int D[6] = {1, 3, 3, 1, 1, 1}; 
    int M[1][3] = {{1, 2, 3}};
    int N[3][1] = {{1}, {1}, {1}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 6);
}

// 8 - Zero matrix multiplication
Test(Multiplication, zero_matrix) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int N[2][2] = {{9, 9}, {9, 9}};
    int A[2][2];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+1)+1), 0);
}

// 9 - Negative results
Test(Multiplication, negative_results) {
    int D[6] = {1, 2, 2, 1, 1, 1};
    int M[1][2] = {{2, -2}};
    int N[2][1] = {{5}, {10}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), -10); 
}
// 10 - Large dimension
Test(Multiplication, dimension_boundary) {
    int D[6] = {5, 1, 1, 5, 5, 5}; 
    int M[5][1] = {{1}, {1}, {1}, {1}, {1}};
    int N[1][5] = {{2, 2, 2, 2, 2}};
    int A[5][5];
    cr_assert_eq(Multiplication(D, M, N, A), 1);
    cr_assert_eq(*(*(A+4)+4), 2);
}

//======================================================================================================================================================================

// 1 - Square matrix and DS size  correct
Test(DiagonalSum, perfect_fit_square) {
    int D[4] = {2, 2, 4, 2}; 
    int A[2][2] = {{1, 2}, {3, 4}};
    int DS[4][2];
    cr_assert_eq(DiagonalSum(D, A, DS), 1);
    cr_assert_eq(*(*(DS+0)+0), 5); 
    cr_assert_eq(*(*(DS+0)+1), 5); 
}

// 2 - Rectangular matrix and Oversized DS
Test(DiagonalSum, oversized_ds_rectangular) {
    int D[4] = {3, 2, 10, 10}; 
    int A[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int DS[10][10];
    cr_assert_eq(DiagonalSum(D, A, DS), 2);
    cr_assert_eq(*(*(DS+2)+0), 2); 
}

// 3 - Undersized DS
Test(DiagonalSum, undersized_ds) {
    int D[4] = {3, 3, 2, 2}; // Needs 5x3
    int A[3][3] = {{0}};
    int DS[2][2];
    cr_assert_eq(DiagonalSum(D, A, DS), -1);
}

// 4 - Column sums check
Test(DiagonalSum, col_sum_check) {
    int D[4] = {2, 2, 4, 2};
    int A[2][2] = {{1, 0}, {5, 0}};
    int DS[4][2];
    DiagonalSum(D, A, DS);
    cr_assert_eq(*(*(DS+1)+0), 6); 
}

// 5 - Anti diagonal only
Test(DiagonalSum, anti_diagonal) {
    int D[4] = {3, 2, 5, 2};
    int A[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int DS[5][2];
    DiagonalSum(D, A, DS);
    cr_assert_eq(*(*(DS+0)+1), 0);
}

// 6 - 1x1 matrix
Test(DiagonalSum, one_by_one) {
    int D[4] = {1, 1, 3, 2};
    int A[1][1] = {{10}};
    int DS[3][2];
    DiagonalSum(D, A, DS);
    cr_assert_eq(*(*(DS+0)+0), 10); 
    cr_assert_eq(*(*(DS+0)+1), 10);
}

// 7 - Negative values
Test(DiagonalSum, negative_vals) {
    int D[4] = {2, 2, 4, 2};
    int A[2][2] = {{-1, -1}, {-1, -1}};
    int DS[4][2];
    DiagonalSum(D, A, DS);
    cr_assert_eq(*(*(DS+2)+0), -2); 
}

// 8 - DS has rows but not enough columns
Test(DiagonalSum, ds_small_cols) {
    int D[4] = {2, 5, 4, 1};
    int A[2][5] = {{0}};
    int DS[4][1];
    cr_assert_eq(DiagonalSum(D, A, DS), -1);
}

// 9 - Square but oversized DS
Test(DiagonalSum, very_large_ds) {
    int D[4] = {2, 2, 100, 100};
    int A[2][2] = {{1, 1}, {1, 1}};
    int DS[100][100];
    cr_assert_eq(DiagonalSum(D, A, DS), 2);
}

// 10 - Check if DS space has zeros
Test(DiagonalSum, zero_check) {
    int D[4] = {2, 2, 10, 10};
    int A[2][2] = {{1, 1}, {1, 1}};
    int DS[10][10];
    for(int i=0; i<10; i++) for(int j=0; j<10; j++) *(*(DS+i)+j) = 99;
    DiagonalSum(D, A, DS);
    cr_assert_eq(*(*(DS+9)+9), 0);
}