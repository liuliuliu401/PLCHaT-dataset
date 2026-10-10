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

void read_inputs(SELECTSORT* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Format: exe seq1 seq2 ... seq10
    int exe_val;
    double seq_vals[10];
    
    int matched = sscanf(line, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                         &exe_val,
                         &seq_vals[0], &seq_vals[1], &seq_vals[2],
                         &seq_vals[3], &seq_vals[4], &seq_vals[5],
                         &seq_vals[6], &seq_vals[7], &seq_vals[8],
                         &seq_vals[9]);
    
    if (matched == 11) {
        // Set EXE input (BOOL)
        instance->EXE.value = exe_val != 0;
        
        // Set SEQ array values
        for (int i = 0; i < 10; i++) {
            instance->SEQ.value.table[i] = seq_vals[i];
        }
    }
}

void print_fb_state(SELECTSORT* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print inputs
    printf("Inputs:\n");
    printf("  EXE: %d\n", instance->EXE.value);
    printf("  SEQ: [");
    for (int i = 0; i < 10; i++) {
        printf("%.3f", instance->SEQ.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("]\n");
    
    // Print outputs
    printf("Outputs:\n");
    printf("  ERROR: %d\n", instance->ERROR.value);
    printf("  STATUS: %u\n", instance->STATUS.value);
    
    // Print internal variables
    printf("Internals:\n");
    printf("  I: %d\n", instance->I.value);
    printf("  J: %d\n", instance->J.value);
    printf("  MIN_IDX: %d\n", instance->MIN_IDX.value);
    printf("  TEMP: %.3f\n", instance->TEMP.value);
    printf("  ARR_LEN: %d\n", instance->ARR_LEN.value);
    
    // Print sorted array (SEQ is modified in-place)
    printf("Sorted SEQ: [");
    for (int i = 0; i < instance->ARR_LEN.value; i++) {
        printf("%.3f", instance->SEQ.value.table[i]);
        if (i < instance->ARR_LEN.value - 1) printf(", ");
    }
    printf("]\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    SELECTSORT instance;
    SELECTSORT_init__(&instance, FALSE);
    
    int cycle = 0;
    printf("SELECTSORT Test Harness\n");
    printf("Input format: exe seq1 seq2 ... seq10\n");
    printf("Example: 1 5.0 3.0 8.0 1.0 9.0 4.0 7.0 2.0 6.0 0.0\n");
    printf("(Only first 5 elements will be sorted as ARR_LEN = 5)\n");
    printf("Enter inputs (Ctrl+D to exit):\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        SELECTSORT_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}