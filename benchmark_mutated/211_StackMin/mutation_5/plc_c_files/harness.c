#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "iec_types_all.h"
#include "POUS.h"

// Global Variables required by MatIEC
TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;

// Robust Input Reading
void read_inputs(STACKMIN* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        exit(0); // End of input file
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;

    int push, pop, reset, item;
    // Assuming input order: PUSH POP RESET ITEM
    if (sscanf(line, "%d %d %d %d", &push, &pop, &reset, &item) == 4) {
        instance->PUSH.value = (BOOL)push;
        instance->POP.value = (BOOL)pop;
        instance->RESET.value = (BOOL)reset;
        instance->ITEM.value = (INT)item;
    }
}

void print_fb_state(STACKMIN* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("INPUTS: PUSH=%d, POP=%d, RESET=%d, ITEM=%d\n", 
           instance->PUSH.value, instance->POP.value, instance->RESET.value, instance->ITEM.value);
    printf("OUTPUTS: ERROR=%d, STATUS=0x%04X\n", 
           instance->ERROR.value, instance->STATUS.value);
    printf("INTERNAL: TOP=%d, STACK=[%d, %d, %d, %d]\n", 
           instance->TOP.value, 
           instance->STACK.value.table[0], instance->STACK.value.table[1], 
           instance->STACK.value.table[2], instance->STACK.value.table[3]);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }

    // Correct Init Call based on POUS.h
    STACKMIN instance;
    STACKMIN_init__(&instance, FALSE);

    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;

        // Correct Time Update
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;

        // Correct Body Call
        STACKMIN_body__(&instance);

        print_fb_state(&instance, cycle);
        fflush(stdout);
        /* Safety Invariant Assertion */
        if ((instance.TOP.value == 0) && (instance.ERROR.value == 0)) {
            if (instance.MININDEX.value > instance.TOP.value) {
                fprintf(stderr, "AUDIT FAIL: Logic corruption detected - minIndex %d exceeds top %d\n", 
                        instance.MININDEX.value, instance.TOP.value);
                exit(1);
            }
        }
    }
    return 0;
}