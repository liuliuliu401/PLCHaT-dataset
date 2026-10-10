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

void read_inputs(RANDOMNUMBERGENERATOR* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse: MINVALUE MAXVALUE RANDOMSEED_IN
    long temp_min, temp_max, temp_seed;
    if (sscanf(line, "%ld %ld %ld", &temp_min, &temp_max, &temp_seed) == 3) {
        instance->MINVALUE.value = temp_min;
        instance->MAXVALUE.value = temp_max;
        instance->RANDOMSEED_IN.value = temp_seed;
    }
}

void print_fb_state(RANDOMNUMBERGENERATOR* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  MINVALUE: %ld\n", (long)instance->MINVALUE.value);
    printf("  MAXVALUE: %ld\n", (long)instance->MAXVALUE.value);
    printf("  RANDOMSEED_IN: %ld\n", (long)instance->RANDOMSEED_IN.value);
    printf("Outputs:\n");
    printf("  ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("  STATUS: %u\n", (unsigned int)instance->STATUS.value);
    printf("  RANDOMNUMBER: %ld\n", (long)instance->RANDOMNUMBER.value);
    printf("Internals:\n");
    printf("  RANGE: %ld\n", (long)instance->RANGE.value);
    printf("  RANDOMSEED: %ld\n", (long)instance->RANDOMSEED.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    RANDOMNUMBERGENERATOR instance;
    RANDOMNUMBERGENERATOR_init__(&instance, FALSE);
    int cycle = 0;
    printf("RandomNumberGenerator C Harness\n");
    printf("Input format: MINVALUE MAXVALUE RANDOMSEED_IN (space-separated DINT values)\n");
    printf("Example: 1 100 12345\n");
    printf("Enter inputs (Ctrl+D to exit):\n");
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        RANDOMNUMBERGENERATOR_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}

