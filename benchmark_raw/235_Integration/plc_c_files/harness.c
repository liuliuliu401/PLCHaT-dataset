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

void read_inputs(INTEGRATION* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse: ENABLE VALUE CURRENTTIMEIN RESET
    // Format: <bool> <double> <TIME> <bool>
    // TIME is represented as milliseconds (e.g., 1000 = 1 second)
    int enable_int;
    double value_double;
    long long current_time_ms;
    int reset_int;
    
    if (sscanf(line, "%d %lf %lld %d", 
               &enable_int, &value_double, &current_time_ms, &reset_int) == 4) {
        instance->ENABLE.value = enable_int ? 1 : 0;
        instance->VALUE.value = value_double;
        // Convert ms to TIME (nanoseconds)
        instance->CURRENTTIMEIN.value.tv_sec = current_time_ms / 1000;
        instance->CURRENTTIMEIN.value.tv_nsec = (current_time_ms % 1000) * 1000000LL;
        instance->RESET.value = reset_int ? 1 : 0;
    }
}

void print_fb_state(INTEGRATION* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  ENABLE: %d\n", instance->ENABLE.value);
    printf("  VALUE: %.6f\n", instance->VALUE.value);
    
    // Convert TIME to ms for display
    long long current_time_ms = (instance->CURRENTTIMEIN.value.tv_sec * 1000LL) + 
                                (instance->CURRENTTIMEIN.value.tv_nsec / 1000000LL);
    printf("  CURRENTTIMEIN: %lld ms\n", current_time_ms);
    printf("  RESET: %d\n", instance->RESET.value);
    
    printf("Outputs:\n");
    printf("  OUTINTEGRAL: %.6f\n", instance->OUTINTEGRAL.value);
    printf("  ERROR: %d\n", instance->ERROR.value);
    printf("  STATUS: 0x%04X\n", (unsigned int)instance->STATUS.value);
    
    printf("Internal State:\n");
    printf("  PREVIOUS_INTEGRAL: %.6f\n", instance->PREVIOUS_INTEGRAL.value);
    printf("  TIME_LAST: %.6f\n", instance->TIME_LAST.value);
    printf("  CURRENT_TIME_LREAL: %.6f\n", instance->CURRENT_TIME_LREAL.value);
    printf("  TIME_DIFF: %.6f\n", instance->TIME_DIFF.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    INTEGRATION instance;
    INTEGRATION_init__(&instance, FALSE);
    
    int cycle = 0;
    printf("Integration FB Test Harness\n");
    printf("Input format: <ENABLE(0/1)> <VALUE(double)> <CURRENTTIMEIN(ms)> <RESET(0/1)>\n");
    printf("Example: 1 2.5 1000 0\n");
    printf("Enter inputs or Ctrl+C to exit...\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        INTEGRATION_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}