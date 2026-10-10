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

void read_inputs(PULSEGENERATOR* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    float temp_freq, temp_ratio;
    if (sscanf(line, "%f %f", &temp_freq, &temp_ratio) == 2) {
        instance->FREQUENCY.value = temp_freq;
        instance->PULSEPAUSERATIO.value = temp_ratio;
    }
}

void print_fb_state(PULSEGENERATOR* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  FREQUENCY: %.2f\n", instance->FREQUENCY.value);
    printf("  PULSEPAUSERATIO: %.2f\n", instance->PULSEPAUSERATIO.value);
    
    printf("Outputs:\n");
    printf("  PULSE: %s\n", instance->PULSE.value ? "TRUE" : "FALSE");
    
    // Convert TIME to milliseconds for display
    long remaining_ms = instance->REMAININGTIME.value.tv_sec * 1000 + 
                       instance->REMAININGTIME.value.tv_nsec / 1000000;
    printf("  REMAININGTIME: %ld ms\n", remaining_ms);
    
    printf("Internal State:\n");
    printf("  INTERNALSTATE: %s\n", instance->INTERNALSTATE.value ? "TRUE" : "FALSE");
    
    long current_ms = instance->CURRENTDURATION.value.tv_sec * 1000 + 
                     instance->CURRENTDURATION.value.tv_nsec / 1000000;
    printf("  CURRENTDURATION: %ld ms\n", current_ms);
    
    long total_ms = instance->TOTALCYCLETIME.value.tv_sec * 1000 + 
                   instance->TOTALCYCLETIME.value.tv_nsec / 1000000;
    printf("  TOTALCYCLETIME: %ld ms\n", total_ms);
    
    long true_ms = instance->TRUEDURATION.value.tv_sec * 1000 + 
                  instance->TRUEDURATION.value.tv_nsec / 1000000;
    printf("  TRUEDURATION: %ld ms\n", true_ms);
    
    long false_ms = instance->FALSEDURATION.value.tv_sec * 1000 + 
                   instance->FALSEDURATION.value.tv_nsec / 1000000;
    printf("  FALSEDURATION: %ld ms\n", false_ms);
    
    printf("  TOTAL_MS: %.2f\n", instance->TOTAL_MS.value);
    printf("  TRUE_MS: %.2f\n", instance->TRUE_MS.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    PULSEGENERATOR instance;
    PULSEGENERATOR_init__(&instance, FALSE);
    
    printf("Pulse Generator Harness\n");
    printf("Enter frequency (Hz) and pulsePauseRatio separated by space\n");
    printf("Example: 10.0 1.5\n");
    printf("(Ctrl+C to exit)\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        PULSEGENERATOR_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}