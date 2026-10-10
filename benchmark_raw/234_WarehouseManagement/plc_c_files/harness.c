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

void read_inputs(FB_WAREHOUSEMANAGEMENT* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse inputs: requestIn requestOut library[1..9] as 1/0
    // Format: requestIn requestOut lib1 lib2 ... lib9
    // Example: 1 0 0 0 1 1 0 0 0 0 1
    
    int temp_requestin, temp_requestout;
    int lib_vals[9];
    
    // Parse all values
    int parsed = sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d",
                       &temp_requestin, &temp_requestout,
                       &lib_vals[0], &lib_vals[1], &lib_vals[2],
                       &lib_vals[3], &lib_vals[4], &lib_vals[5],
                       &lib_vals[6], &lib_vals[7], &lib_vals[8]);
    
    if (parsed == 11) {
        // Convert to BOOL (MatIEC BOOL is uint8_t)
        instance->REQUESTIN.value = temp_requestin ? 1 : 0;
        instance->REQUESTOUT.value = temp_requestout ? 1 : 0;
        
        // Fill array - note ST array indices 1..9 map to C indices 0..8
        for (int i = 0; i < 9; i++) {
            instance->LIBRARY.value.table[i] = lib_vals[i] ? 1 : 0;
        }
    }
}

void print_fb_state(FB_WAREHOUSEMANAGEMENT* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  REQUESTIN: %s\n", instance->REQUESTIN.value ? "TRUE" : "FALSE");
    printf("  REQUESTOUT: %s\n", instance->REQUESTOUT.value ? "TRUE" : "FALSE");
    printf("  LIBRARY: [");
    for (int i = 0; i < 9; i++) {
        printf("%s%s", instance->LIBRARY.value.table[i] ? "1" : "0", 
               (i < 8) ? " " : "");
    }
    printf("]\n");
    
    printf("\nOutputs:\n");
    printf("  PRODUCTNUM: %d\n", (int)instance->PRODUCTNUM.value);
    printf("  LIBFREENUM: %d\n", (int)instance->LIBFREENUM.value);
    printf("  ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("  STATUS: %u (0x%04X)\n", 
           (unsigned int)instance->STATUS.value, 
           (unsigned int)instance->STATUS.value);
    
    printf("\nInternal:\n");
    printf("  I: %d\n", (int)instance->I.value);
    printf("  FOUND: %s\n", instance->FOUND.value ? "TRUE" : "FALSE");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_WAREHOUSEMANAGEMENT instance;
    FB_WAREHOUSEMANAGEMENT_init__(&instance, FALSE);
    
    printf("Warehouse Management FB Test Harness\n");
    printf("Input format: REQUESTIN REQUESTOUT LIB[1] LIB[2] ... LIB[9]\n");
    printf("Example: 1 0 0 0 1 1 0 0 0 0 1\n");
    printf("Use # for comments, empty line to reuse previous inputs\n");
    printf("----------------------------------------\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_WAREHOUSEMANAGEMENT_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}

