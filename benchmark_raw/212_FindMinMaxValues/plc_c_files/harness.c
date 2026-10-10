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

void read_inputs(FINDMINMAXVALUES* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse 10 DINT values for the VALUES array
    long temp_vals[10];
    int parsed = sscanf(line, 
        "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
        &temp_vals[0], &temp_vals[1], &temp_vals[2], &temp_vals[3], &temp_vals[4],
        &temp_vals[5], &temp_vals[6], &temp_vals[7], &temp_vals[8], &temp_vals[9]);
    
    // Only update if we got all 10 values
    if (parsed == 10) {
        for (int i = 0; i < 10; i++) {
            instance->VALUES.value.table[i] = temp_vals[i];
        }
    }
}

void print_fb_state(FINDMINMAXVALUES* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print input array
    printf("VALUES: [");
    for (int i = 0; i < 10; i++) {
        printf("%ld", (long)instance->VALUES.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("]\n");
    
    // Print outputs - note that ST uses 1-based indexing, but C uses 0-based
    // So we convert the index by adding 1 for display
    printf("MINVALUE: %ld\n", (long)instance->MINVALUE.value);
    printf("MINVALUEINDEX (1-based): %ld\n", (long)instance->MINVALUEINDEX.value);
    printf("MAXVALUE: %ld\n", (long)instance->MAXVALUE.value);
    printf("MAXVALUEINDEX (1-based): %ld\n", (long)instance->MAXVALUEINDEX.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FINDMINMAXVALUES instance;
    FINDMINMAXVALUES_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FINDMINMAXVALUES_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}

