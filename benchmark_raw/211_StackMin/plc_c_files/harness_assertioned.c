/* Auto-generated Harness by PLCHaT */
/* DO NOT EDIT MANUALLY */

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


/* Expanded scanf format string and temp variables */

/* Temp variable declarations */
int temp_push;
int temp_pop;
int temp_reset;
int temp_item;
int temp_stack_0;
int temp_stack_1;
int temp_stack_2;
int temp_stack_3;

void read_inputs(STACKMIN* instance) {
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;

    /* Parse input: push, pop, reset, item, stack */

    int parsed = sscanf(line, "%d %d %d %d %d %d %d %d",
        &temp_push,
        &temp_pop,
        &temp_reset,
        &temp_item,
        &temp_stack_0,
        &temp_stack_1,
        &temp_stack_2,
        &temp_stack_3
    );

    if (parsed < 8) return;

    /* Map parsed values to instance */
    /* push: BOOL */
    instance->PUSH.value = temp_push ? 1 : 0;
    /* pop: BOOL */
    instance->POP.value = temp_pop ? 1 : 0;
    /* reset: BOOL */
    instance->RESET.value = temp_reset ? 1 : 0;
    /* item: INT */
    instance->ITEM.value = temp_item;
    /* stack: ARRAY[0..3] OF INT */
    instance->STACK.value.table[0] = temp_stack_0;
    instance->STACK.value.table[1] = temp_stack_1;
    instance->STACK.value.table[2] = temp_stack_2;
    instance->STACK.value.table[3] = temp_stack_3;
}

void print_fb_state(STACKMIN* instance, int cycle) {
    if (silent_mode) return;

    printf("\n--- Cycle %d ---\n", cycle);

    /* Inputs */
    printf("push: %s\n", instance->PUSH.value ? "TRUE" : "FALSE");
    printf("pop: %s\n", instance->POP.value ? "TRUE" : "FALSE");
    printf("reset: %s\n", instance->RESET.value ? "TRUE" : "FALSE");

    /* Outputs */
    printf("error: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("status: %d\n", (int)instance->STATUS.value);

}

int check_assertions(STACKMIN* inst) {
    /* P0: Error code monitoring */
    if (inst->ERROR.value) {
        fprintf(stderr, "SECURITY_ASSERTION_FAILED: ERROR is TRUE (indicating stack operation failure)");
        fflush(stderr);
        return 1;;
    }

    /* P0: State consistency — STATUS must be a valid stack state indicator */
    /* STATUS encodes stack occupancy (0–4) in lower 3 bits; higher bits reserved for flags */
    /* Valid occupancy: 0 to 4 inclusive */
    uint16_t occupancy = inst->STATUS.value & 0x07U;
    if (occupancy > 4U) {
        fprintf(stderr, "SECURITY_ASSERTION_FAILED: STATUS occupancy bits (0x%04x & 0x07) = %u exceeds max capacity 4", (unsigned)inst->STATUS.value, (unsigned)occupancy);
        fflush(stderr);
        return 2;;
    }

    /* P1: Boundary check — verify all stack elements are within INT16 range (assumed element type) */
    /* STACK is an array of 4 INTs; access via .table[0..3] */
    #define STACK_CAPACITY 4
    for (int i = 0; i < STACK_CAPACITY; i++) {
        /* Assuming stack elements are INT (16-bit signed); validate representable range */
        int16_t elem = (int16_t)inst->STACK.value.table[i];
        /* INT16_MIN = -32768, INT16_MAX = 32767 */
        if (elem < -32768 || elem > 32767) {
            fprintf(stderr, "SECURITY_ASSERTION_FAILED: STACK[%d]=%d violates INT16 bounds", i, (int)elem);
            fflush(stderr);
            return 3;;
        }
    }

    /* P1: Anomaly pattern detection — if occupancy > 0, at least one stack element must be defined */
    /* But more critically: if occupancy == 0, all stack elements must be zero (clean reset state) */
    if (occupancy == 0) {
        for (int i = 0; i < STACK_CAPACITY; i++) {
            if (inst->STACK.value.table[i] != 0) {
                fprintf(stderr, "SECURITY_ASSERTION_FAILED: STATUS occupancy=0 but STACK[%d]=%ld ≠ 0 (inconsistent reset state)", i, (long)inst->STACK.value.table[i]);
                fflush(stderr);
                return 4;;
            }
        };
    }

    return 0;;
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }

    STACKMIN instance;
    STACKMIN_init__(&instance, FALSE);

    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;

        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;

        STACKMIN_body__(&instance);
    int assertion_result = check_assertions(&instance);
    if (assertion_result != 0) { exit(assertion_result); }
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }

    return 0;
}