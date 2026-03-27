
#include <criterion/criterion.h>
#include "matrix.h"

// 1. Edge Case: Completely empty matrix (all zeros)
Test(SparseMatrix, all_zeros) {
    int D[2] = {2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int S[3][2]; // m = 2
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 0, "Should return 0 for all-zero matrix");
}

// 2. Normal Case: Single non-zero at the very beginning
Test(SparseMatrix, single_element_start) {
    int D[2] = {3, 2};
    int M[3][2] = {{5, 0}, {0, 0}, {0, 0}};
    int S[3][3]; // m = 3
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 0); // Row index
    cr_assert_eq(*(*(S+2)+0), 5); // Value
}

// 3. Normal Case: Single non-zero at the very end
Test(SparseMatrix, single_element_end) {
    int D[2] = {2, 3};
    int M[2][3] = {{0, 0, 0}, {0, 0, 99}};
    int S[3][3]; // m = 3
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 1); // Row index
    cr_assert_eq(*(*(S+1)+0), 2); // Col index
    cr_assert_eq(*(*(S+2)+0), 99); // Value
}

// 4. Edge Case: Rectangular (Rows > Cols) right at the limit
Test(SparseMatrix, rectangular_rows_sparse) {
    int D[2] = {4, 2}; // m = 4
    int M[4][2] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}}; // 4 non-zeros
    int S[3][4];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 4, "Should be sparse if nz == max dimension (4)"); [cite: 63, 145]
}

// 5. Edge Case: Rectangular (Cols > Rows) just over the limit
Test(SparseMatrix, rectangular_cols_not_sparse) {
    int D[2] = {2, 4}; // m = 4
    int M[2][4] = {{1, 2, 3, 0}, {4, 5, 0, 0}}; // 5 non-zeros
    int S[3][4];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, -1, "Should return -1 if nz > max dimension (5 > 4)"); [cite: 147]
}

// 6. Normal Case: Matrix with negative values
Test(SparseMatrix, negative_values) {
    int D[2] = {2, 2};
    int M[2][2] = {{0, -10}, {5, 0}};
    int S[3][2];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 2);
    cr_assert_eq(*(*(S+2)+0), -10); [cite: 63]
}

// 7. Edge Case: 1x1 Zero Matrix
Test(SparseMatrix, one_by_one_zero) {
    int D[2] = {1, 1};
    int M[1][1] = {{0}};
    int S[3][1];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 0);
}

// 8. Edge Case: 1x1 Non-Zero Matrix
Test(SparseMatrix, one_by_one_nonzero) {
    int D[2] = {1, 1};
    int M[1][1] = {{42}};
    int S[3][1];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+2)+0), 42);
}

// 9. Consistency: S should be padded with zeros if count < m
Test(SparseMatrix, padding_check) {
    int D[2] = {3, 3}; // m = 3
    int M[3][3] = {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}}; // 1 non-zero
    int S[3][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}; // Fill with garbage
    SparseMatrix(D, M, S);
    cr_assert_eq(*(*(S+0)+1), 0, "Unused S columns must be zeroed"); [cite: 148]
    cr_assert_eq(*(*(S+2)+2), 0, "Unused S columns must be zeroed"); [cite: 148]
}

// 10. Large Case: Boundary check for largest dimension
Test(SparseMatrix, large_sparse_check) {
    int D[2] = {10, 2}; // m = 10
    int M[10][2] = {{0}};
    *(*(M+9)+1) = 50; // One element at the very end
    int S[3][10];
    int res = SparseMatrix(D, M, S);
    cr_assert_eq(res, 1);
    cr_assert_eq(*(*(S+0)+0), 9);
    cr_assert_eq(*(*(S+1)+0), 1);
}

//===========================================================================================================================================

// 1. Case 1: All compatible (Perfect Fit) [cite: 255]
Test(HadamardProduct, perfect_fit) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{5, 6}, {7, 8}};
    int A[2][2];
    cr_assert_eq(HadamardProduct(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 5);
}

// 2. Case 2: M/N compatible, A oversized [cite: 260]
Test(HadamardProduct, oversized_A) {
    int D[6] = {2, 2, 2, 2, 3, 3};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{2, 2}, {2, 2}};
    int A[3][3];
    cr_assert_eq(HadamardProduct(D, M, N, A), 2);
    cr_assert_eq(*(*(A+1)+1), 8);
    cr_assert_eq(*(*(A+2)+2), 0); // Padding check
}

// 3. Case 3: M/N compatible, A undersized [cite: 172]
Test(HadamardProduct, undersized_A_compatible_MN) {
    int D[6] = {2, 2, 2, 2, 1, 1};
    int M[2][2] = {{10, 10}, {10, 10}};
    int N[2][2] = {{2, 2}, {2, 2}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), -3);
}

// 4. Case 4: M/N incompatible, A fits intersection [cite: 264]
Test(HadamardProduct, incompatible_MN_A_fits) {
    int D[6] = {3, 3, 2, 2, 3, 3};
    int M[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int N[2][2] = {{2, 5}, {1, 4}};
    int A[3][3];
    cr_assert_eq(HadamardProduct(D, M, N, A), -1);
    cr_assert_eq(*(*(A+0)+0), 2); // 1*2
    cr_assert_eq(*(*(A+1)+1), 20); // 5*4
}

// 5. Case 5: M/N incompatible, A too small for intersection [cite: 269]
Test(HadamardProduct, incompatible_MN_A_small) {
    int D[6] = {3, 3, 2, 2, 1, 1};
    int M[3][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
    int N[2][2] = {{3, 3}, {3, 3}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), -2);
}

// 6. Edge Case: One matrix is 1x1
Test(HadamardProduct, one_by_one_matrices) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{5}};
    int N[1][1] = {{10}};
    int A[1][1];
    cr_assert_eq(HadamardProduct(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 50);
}

// 7. Edge Case: Negative values
Test(HadamardProduct, negative_values) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{-2}};
    int N[1][1] = {{4}};
    int A[1][1];
    HadamardProduct(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), -8);
}

// 8. Edge Case: All zeros
Test(HadamardProduct, zero_matrices) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int N[2][2] = {{1, 1}, {1, 1}};
    int A[2][2];
    HadamardProduct(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 0);
}

// 9. Case 2 Variation: Extremely oversized A
Test(HadamardProduct, very_oversized_A) {
    int D[6] = {1, 1, 1, 1, 5, 5};
    int M[1][1] = {{2}};
    int N[1][1] = {{3}};
    int A[5][5];
    cr_assert_eq(HadamardProduct(D, M, N, A), 2);
}

// 10. Boundary: Non-square intersection
Test(HadamardProduct, rectangular_intersection) {
    int D[6] = {5, 2, 2, 5, 5, 5}; // Intersect is 2x2
    int M[5][2] = {{1, 1}, {1, 1}, {0, 0}, {0, 0}, {0, 0}};
    int N[2][5] = {{2, 2, 0, 0, 0}, {2, 2, 0, 0, 0}};
    int A[5][5];
    cr_assert_eq(HadamardProduct(D, M, N, A), -1);
}

//=======================================================================================================================================================

// 1. Compatible: Perfect Fit (Return 1)
Test(Multiplication, perfect_fit) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{1, 2}, {3, 4}};
    int N[2][2] = {{5, 6}, {7, 8}};
    int A[2][2];
    cr_assert_eq(Multiplication(D, M, N, A), 1);
    cr_assert_eq(*(*(A+0)+0), 19); // (1*5 + 2*7)
}

// 2. Compatible: Oversized A (Return 2) [cite: 281]
Test(Multiplication, oversized_A) {
    int D[6] = {2, 2, 2, 2, 4, 4};
    int M[2][2] = {{1, 0}, {0, 1}};
    int N[2][2] = {{5, 5}, {5, 5}};
    int A[4][4];
    cr_assert_eq(Multiplication(D, M, N, A), 2);
}

// 3. Compatible: Undersized A (Return -3) [cite: 283]
Test(Multiplication, undersized_A_compatible) {
    int D[6] = {3, 2, 2, 3, 2, 4}; // Result should be 3x3, A is 2x4
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[2][3] = {{1, 1, 1}, {1, 1, 1}};
    int A[2][4];
    cr_assert_eq(Multiplication(D, M, N, A), -3);
}

// 4. Incompatible: A fits partial result (Return -1) [cite: 276]
Test(Multiplication, incompatible_A_fits) {
    int D[6] = {3, 2, 3, 2, 4, 3}; // mC=2, nR=3 (Incomp), Result 3x2, A is 4x3
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[3][2] = {{2, 2}, {2, 2}, {2, 2}};
    int A[4][3];
    cr_assert_eq(Multiplication(D, M, N, A), -1);
}

// 5. Incompatible: A too small for partial result (Return -2) [cite: 219]
Test(Multiplication, incompatible_A_small) {
    int D[6] = {3, 2, 3, 2, 2, 2}; // Result 3x2, A is 2x2
    int M[3][2] = {{1, 1}, {1, 1}, {1, 1}};
    int N[3][2] = {{2, 2}, {2, 2}, {2, 2}};
    int A[2][2];
    cr_assert_eq(Multiplication(D, M, N, A), -2);
}

// 6. Edge Case: 1x1 Identity
Test(Multiplication, identity_1x1) {
    int D[6] = {1, 1, 1, 1, 1, 1};
    int M[1][1] = {{10}};
    int N[1][1] = {{2}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 20);
}

// 7. Edge Case: Rectangular compatible
Test(Multiplication, rectangular_comp) {
    int D[6] = {1, 3, 3, 1, 1, 1}; // 1x3 * 3x1 -> 1x1
    int M[1][3] = {{1, 2, 3}};
    int N[3][1] = {{1}, {1}, {1}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), 6);
}

// 8. Edge Case: Zero Matrix multiplication
Test(Multiplication, zero_matrix) {
    int D[6] = {2, 2, 2, 2, 2, 2};
    int M[2][2] = {{0, 0}, {0, 0}};
    int N[2][2] = {{9, 9}, {9, 9}};
    int A[2][2];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+1)+1), 0);
}

// 9. Edge Case: Negative results
Test(Multiplication, negative_results) {
    int D[6] = {1, 2, 2, 1, 1, 1};
    int M[1][2] = {{2, -2}};
    int N[2][1] = {{5}, {10}};
    int A[1][1];
    Multiplication(D, M, N, A);
    cr_assert_eq(*(*(A+0)+0), -10); // (10 - 20)
}

// 10. Large Dimension Boundary
Test(Multiplication, dimension_boundary) {
    int D[6] = {5, 1, 1, 5, 5, 5}; // 5x1 * 1x5 -> 5x5
    int M[5][1] = {{1}, {1}, {1}, {1}, {1}};
    int N[1][5] = {{2, 2, 2, 2, 2}};
    int A[5][5];
    cr_assert_eq(Multiplication(D, M, N, A), 1);
    cr_assert_eq(*(*(A+4)+4), 2);
}

//======================================================================================================================================================================

Test(DiagonalSum, square_matrix)
{
    int D[4] = {3,3,5,5};

    int A[3][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };

    int DS[5][5];

    int r = DiagonalSum(D,A,DS);

    cr_assert_eq(r,1);
}
