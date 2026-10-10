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

void read_inputs(ALARMPROCESS* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    float temp_value, temp_hilevel, temp_lolevel, temp_hystwindow;
    if (sscanf(line, "%f %f %f %f", 
               &temp_value, &temp_hilevel, &temp_lolevel, &temp_hystwindow) == 4) {
        instance->VALUE.value = temp_value;
        instance->HILEVEL.value = temp_hilevel;
        instance->LOLEVEL.value = temp_lolevel;
        instance->HYSTWINDOW.value = temp_hystwindow;
    }
}

void print_fb_state(ALARMPROCESS* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  VALUE: %f\n", instance->VALUE.value);
    printf("  HILEVEL: %f\n", instance->HILEVEL.value);
    printf("  LOLEVEL: %f\n", instance->LOLEVEL.value);
    printf("  HYSTWINDOW: %f\n", instance->HYSTWINDOW.value);
    printf("Outputs:\n");
    printf("  HIALARM: %s\n", instance->HIALARM.value ? "TRUE" : "FALSE");
    printf("  LOALARM: %s\n", instance->LOALARM.value ? "TRUE" : "FALSE");
    printf("  ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("  STATUS: 0x%04X\n", (unsigned int)instance->STATUS.value);
    printf("Internal:\n");
    printf("  TEMPSTATUS: 0x%04X\n", (unsigned int)instance->TEMPSTATUS.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    ALARMPROCESS instance;
    ALARMPROCESS_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        ALARMPROCESS_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}

