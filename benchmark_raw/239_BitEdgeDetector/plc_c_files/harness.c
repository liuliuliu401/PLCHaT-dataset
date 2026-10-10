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

void read_inputs(BITEDGEDETECTOR* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input: expect a single DWORD (32-bit unsigned) value
    unsigned long temp_value;
    if (sscanf(line, "%lu", &temp_value) == 1) {
        instance->VALUE.value = (UDINT)temp_value;
    }
}

void print_fb_state(BITEDGEDETECTOR* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Input VALUE: %u (0x%08X)\n", 
           (unsigned int)instance->VALUE.value,
           (unsigned int)instance->VALUE.value);
    printf("Previous VALUE: %u (0x%08X)\n",
           (unsigned int)instance->PREVIOUSVALUE.value,
           (unsigned int)instance->PREVIOUSVALUE.value);
    printf("HASCHANGED: %s\n", instance->HASCHANGED.value ? "TRUE" : "FALSE");
    printf("HASRISINGEDGES: %s\n", instance->HASRISINGEDGES.value ? "TRUE" : "FALSE");
    printf("RISINGBITS: %u (0x%08X)\n",
           (unsigned int)instance->RISINGBITS.value,
           (unsigned int)instance->RISINGBITS.value);
    printf("NOOFRISINGBITS: %u\n", (unsigned int)instance->NOOFRISINGBITS.value);
    printf("HASFALLINGEDGES: %s\n", instance->HASFALLINGEDGES.value ? "TRUE" : "FALSE");
    printf("FALLINGBITS: %u (0x%08X)\n",
           (unsigned int)instance->FALLINGBITS.value,
           (unsigned int)instance->FALLINGBITS.value);
    printf("NOOFFALLINGBITS: %u\n", (unsigned int)instance->NOOFFALLINGBITS.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    BITEDGEDETECTOR instance;
    BITEDGEDETECTOR_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        BITEDGEDETECTOR_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}

