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

void read_inputs(FB_SPLITNUMBER* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input: expecting a single INT value for INPUTNUMBER
    int input_val;
    if (sscanf(line, "%d", &input_val) == 1) {
        instance->INPUTNUMBER.value = input_val;
    }
}

void print_fb_state(FB_SPLITNUMBER* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("INPUTNUMBER: %d\n", (int)instance->INPUTNUMBER.value);
    printf("THOUSANDS: %d\n", (int)instance->THOUSANDS.value);
    printf("HUNDREDS: %d\n", (int)instance->HUNDREDS.value);
    printf("TENS: %d\n", (int)instance->TENS.value);
    printf("ONES: %d\n", (int)instance->ONES.value);
    printf("OUTMIN: %d\n", (int)instance->OUTMIN.value);
    printf("ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("STATUS: %u (0x%04X)\n", 
           (unsigned int)instance->STATUS.value,
           (unsigned int)instance->STATUS.value);
    printf("TEMPNUMBER: %d\n", (int)instance->TEMPNUMBER.value);
    printf("MINVALUE: %d\n", (int)instance->MINVALUE.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_SPLITNUMBER instance;
    FB_SPLITNUMBER_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_SPLITNUMBER_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}

