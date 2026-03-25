/*
 * Helper function to print a sparse matrix representation
 * Not required for the assignment but useful for testing
 */
void print_sparse_matrix(int S[3][m], int m) {
    printf("Sparse Matrix Representation (3 x %d):\n", m);
    printf("Row:    ");
    for (int j = 0; j < m; j++) {
        printf("%d ", *((int *)S + 0 * m + j));
    }
    printf("\nColumn: ");
    for (int j = 0; j < m; j++) {
        printf("%d ", *((int *)S + 1 * m + j));
    }
    printf("\nValue:  ");
    for (int j = 0; j < m; j++) {
        printf("%d ", *((int *)S + 2 * m + j));
    }
    printf("\n");
}