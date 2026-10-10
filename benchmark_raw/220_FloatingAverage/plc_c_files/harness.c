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

void read_inputs(FLOATINGAVERAGE* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse inputs: CYCLICEXECUTION TRIGGER VALUE WINDOWSIZE RESET
    // Format: BOOL BOOL REAL INT BOOL
    // Example: 1 0 12.5 10 0
    int temp_cyclicExecution, temp_trigger, temp_reset;
    float temp_value;
    int temp_windowSize;
    
    if (sscanf(line, "%d %d %f %d %d", 
               &temp_cyclicExecution, &temp_trigger, &temp_value, 
               &temp_windowSize, &temp_reset) == 5) {
        instance->CYCLICEXECUTION.value = temp_cyclicExecution;
        instance->TRIGGER.value = temp_trigger;
        instance->VALUE.value = temp_value;
        instance->WINDOWSIZE.value = temp_windowSize;
        instance->RESET.value = temp_reset;
    }
}

void print_fb_state(FLOATINGAVERAGE* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs: CYCLICEXECUTION=%d, TRIGGER=%d, VALUE=%.6f, WINDOWSIZE=%d, RESET=%d\n",
           instance->CYCLICEXECUTION.value, instance->TRIGGER.value,
           instance->VALUE.value, instance->WINDOWSIZE.value, instance->RESET.value);
    printf("Outputs: AVERAGE=%.6f, WINDOWSIZEREACHED=%d, ERROR=%d, STATUS=%u\n",
           instance->AVERAGE.value, instance->WINDOWSIZEREACHED.value,
           instance->ERROR.value, (unsigned int)instance->STATUS.value);
    printf("Internal: BUFFERINDEX=%d, SUM=%.6f, TRIGGER_PREV=%d, R_TRIG_PULSE=%d\n",
           instance->BUFFERINDEX.value, instance->SUM.value,
           instance->TRIGGER_PREV.value, instance->R_TRIG_PULSE.value);
    
    // Print first 5 elements of buffer for debugging
    printf("DATABUFFER[0..4]: ");
    for (int i = 0; i < 5 && i < 100; i++) {
        printf("%.2f ", instance->DATABUFFER.value.table[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FLOATINGAVERAGE instance;
    FLOATINGAVERAGE_init__(&instance, FALSE);
    int cycle = 0;
    
    printf("FloatingAverage Test Harness\n");
    printf("Input format: CYCLICEXECUTION(0/1) TRIGGER(0/1) VALUE(float) WINDOWSIZE(int) RESET(0/1)\n");
    printf("Example: 1 0 12.5 10 0\n");
    printf("Start entering inputs (Ctrl+C to exit):\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        FLOATINGAVERAGE_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}