#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "iec_types_all.h"
#include "POUS.h"

TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;

void read_inputs(MATRIXADDITION* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse two 10x10 LREAL matrices and store in temporary arrays
    double temp1[10][10], temp2[10][10];
    int scan_count = sscanf(line, 
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
        &temp1[0][0], &temp1[0][1], &temp1[0][2], &temp1[0][3], &temp1[0][4],
        &temp1[0][5], &temp1[0][6], &temp1[0][7], &temp1[0][8], &temp1[0][9],
        &temp1[1][0], &temp1[1][1], &temp1[1][2], &temp1[1][3], &temp1[1][4],
        &temp1[1][5], &temp1[1][6], &temp1[1][7], &temp1[1][8], &temp1[1][9],
        &temp1[2][0], &temp1[2][1], &temp1[2][2], &temp1[2][3], &temp1[2][4],
        &temp1[2][5], &temp1[2][6], &temp1[2][7], &temp1[2][8], &temp1[2][9],
        &temp1[3][0], &temp1[3][1], &temp1[3][2], &temp1[3][3], &temp1[3][4],
        &temp1[3][5], &temp1[3][6], &temp1[3][7], &temp1[3][8], &temp1[3][9],
        &temp1[4][0], &temp1[4][1], &temp1[4][2], &temp1[4][3], &temp1[4][4],
        &temp1[4][5], &temp1[4][6], &temp1[4][7], &temp1[4][8], &temp1[4][9],
        &temp1[5][0], &temp1[5][1], &temp1[5][2], &temp1[5][3], &temp1[5][4],
        &temp1[5][5], &temp1[5][6], &temp1[5][7], &temp1[5][8], &temp1[5][9],
        &temp1[6][0], &temp1[6][1], &temp1[6][2], &temp1[6][3], &temp1[6][4],
        &temp1[6][5], &temp1[6][6], &temp1[6][7], &temp1[6][8], &temp1[6][9],
        &temp1[7][0], &temp1[7][1], &temp1[7][2], &temp1[7][3], &temp1[7][4],
        &temp1[7][5], &temp1[7][6], &temp1[7][7], &temp1[7][8], &temp1[7][9],
        &temp1[8][0], &temp1[8][1], &temp1[8][2], &temp1[8][3], &temp1[8][4],
        &temp1[8][5], &temp1[8][6], &temp1[8][7], &temp1[8][8], &temp1[8][9],
        &temp1[9][0], &temp1[9][1], &temp1[9][2], &temp1[9][3], &temp1[9][4],
        &temp1[9][5], &temp1[9][6], &temp1[9][7], &temp1[9][8], &temp1[9][9],
        &temp2[0][0], &temp2[0][1], &temp2[0][2], &temp2[0][3], &temp2[0][4],
        &temp2[0][5], &temp2[0][6], &temp2[0][7], &temp2[0][8], &temp2[0][9],
        &temp2[1][0], &temp2[1][1], &temp2[1][2], &temp2[1][3], &temp2[1][4],
        &temp2[1][5], &temp2[1][6], &temp2[1][7], &temp2[1][8], &temp2[1][9],
        &temp2[2][0], &temp2[2][1], &temp2[2][2], &temp2[2][3], &temp2[2][4],
        &temp2[2][5], &temp2[2][6], &temp2[2][7], &temp2[2][8], &temp2[2][9],
        &temp2[3][0], &temp2[3][1], &temp2[3][2], &temp2[3][3], &temp2[3][4],
        &temp2[3][5], &temp2[3][6], &temp2[3][7], &temp2[3][8], &temp2[3][9],
        &temp2[4][0], &temp2[4][1], &temp2[4][2], &temp2[4][3], &temp2[4][4],
        &temp2[4][5], &temp2[4][6], &temp2[4][7], &temp2[4][8], &temp2[4][9],
        &temp2[5][0], &temp2[5][1], &temp2[5][2], &temp2[5][3], &temp2[5][4],
        &temp2[5][5], &temp2[5][6], &temp2[5][7], &temp2[5][8], &temp2[5][9],
        &temp2[6][0], &temp2[6][1], &temp2[6][2], &temp2[6][3], &temp2[6][4],
        &temp2[6][5], &temp2[6][6], &temp2[6][7], &temp2[6][8], &temp2[6][9],
        &temp2[7][0], &temp2[7][1], &temp2[7][2], &temp2[7][3], &temp2[7][4],
        &temp2[7][5], &temp2[7][6], &temp2[7][7], &temp2[7][8], &temp2[7][9],
        &temp2[8][0], &temp2[8][1], &temp2[8][2], &temp2[8][3], &temp2[8][4],
        &temp2[8][5], &temp2[8][6], &temp2[8][7], &temp2[8][8], &temp2[8][9],
        &temp2[9][0], &temp2[9][1], &temp2[9][2], &temp2[9][3], &temp2[9][4],
        &temp2[9][5], &temp2[9][6], &temp2[9][7], &temp2[9][8], &temp2[9][9]
    );
    
    // If we got at least some values, copy them to the instance
    if (scan_count > 0) {
        // Copy first matrix values (if provided)
        int values_copied = 0;
        for (int i = 0; i < 10 && values_copied < scan_count; i++) {
            for (int j = 0; j < 10 && values_copied < scan_count; j++) {
                instance->MATRIX1.value.table[i][j] = temp1[i][j];
                values_copied++;
            }
        }
        
        // Copy second matrix values if enough were provided
        if (scan_count >= 200) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    instance->MATRIX2.value.table[i][j] = temp2[i][j];
                }
            }
        }
    }
}

void print_fb_state(MATRIXADDITION* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print ERROR status
    printf("ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    
    // Print a 3x3 submatrix of the result for brevity
    printf("Result matrix (first 3x3):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%8.3f ", instance->MATRIXRESULT.value.table[i][j]);
        }
        printf("\n");
    }
    
    // Print internal ROW and COL values
    printf("ROW: %d, COL: %d\n", 
           (int)instance->ROW.value, 
           (int)instance->COL.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    MATRIXADDITION instance;
    MATRIXADDITION_init__(&instance, FALSE);
    
    int cycle = 0;
    printf("Matrix Addition Test Harness\n");
    printf("Enter 200 LREAL values (matrix1[10][10] then matrix2[10][10]) separated by spaces:\n");
    printf("Format: m1[0][0] m1[0][1] ... m1[9][9] m2[0][0] ... m2[9][9]\n");
    printf("Or just press Enter to use previous/default values\n");
    printf("Enter '#' to comment, Ctrl+D to exit\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        MATRIXADDITION_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}