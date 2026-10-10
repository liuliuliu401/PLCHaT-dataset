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
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }

    return 0;
}