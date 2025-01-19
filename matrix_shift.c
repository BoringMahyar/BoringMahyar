#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

void cyclic_shift(long double *matrix, int rows, int cols, int T) {
    for (int col = 0; col < cols; col++) {
        for (int i = 1; i < rows; i += 2) { // Каждый второй элемент (индексы: 1, 3, ...)
            long double temp = matrix[col * rows + i];
            int new_pos = (i + T) % rows;
            if (new_pos % 2 == 0) {
                continue; // Только для нечётных позиций
            }
            matrix[col * rows + i] = matrix[col * rows + new_pos];
            matrix[col * rows + new_pos] = temp;
        }
    }
}

void print_matrix(long double *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2Lf ", matrix[j * rows + i]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s [-m] rows cols T\n", argv[0]);
        return EXIT_FAILURE;
    }

    int manual_input = 0;
    int arg_offset = 0;

    if (strcmp(argv[1], "-m") == 0) {
        manual_input = 1;
        arg_offset = 1;
    }

    int rows = atoi(argv[1 + arg_offset]);
    int cols = atoi(argv[2 + arg_offset]);
    int T = atoi(argv[3 + arg_offset]);

    if (rows <= 0 || cols <= 0 || T < 0) {
        fprintf(stderr, "Error: Invalid matrix dimensions or shift value.\n");
        return EXIT_FAILURE;
    }

    long double *matrix = (long double *)malloc(rows * cols * sizeof(long double));
    if (!matrix) {
        perror("Error allocating memory");
        return EXIT_FAILURE;
    }

    if (manual_input) {
        printf("Enter matrix elements (%dx%d):\n", rows, cols);
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                if (scanf("%Lf", &matrix[i * rows + j]) != 1) {
                    fprintf(stderr, "Error: Invalid input.\n");
                    free(matrix);
                    return EXIT_FAILURE;
                }
            }
        }
    } else {
        srand((unsigned int)time(NULL));
        for (int i = 0; i < rows * cols; i++) {
            matrix[i] = (long double)(rand() % 100) / 10.0;
        }
    }

    printf("Original matrix:\n");
    print_matrix(matrix, rows, cols);

    cyclic_shift(matrix, rows, cols, T);

    printf("Transformed matrix:\n");
    print_matrix(matrix, rows, cols);

    free(matrix);
    return EXIT_SUCCESS;
}
