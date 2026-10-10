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

void read_inputs(MULTIPUMPCTRL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse inputs: mode start stop priorities[1..5] selections[1..5]
    int mode_int, start_int, stop_int;
    int priority_vals[5];
    int selection_vals[5];
    
    // Parse all values
    int parsed = sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
        &mode_int, &start_int, &stop_int,
        &priority_vals[0], &priority_vals[1], &priority_vals[2], 
        &priority_vals[3], &priority_vals[4],
        &selection_vals[0], &selection_vals[1], &selection_vals[2],
        &selection_vals[3], &selection_vals[4]);
    
    // Check if we got at least the first 3 values
    if (parsed < 3) return;
    
    // Set scalar inputs
    instance->MODE.value = mode_int ? 1 : 0;
    instance->START.value = start_int ? 1 : 0;
    instance->STOP.value = stop_int ? 1 : 0;
    
    // Set priorities array (INT values, 0-indexed in C)
    for (int i = 0; i < 5; i++) {
        if (parsed >= 4 + i) {
            instance->PRIORITIES.value.table[i] = priority_vals[i];
        }
    }
    
    // Set selections array (BOOL values, 0-indexed in C)
    for (int i = 0; i < 5; i++) {
        if (parsed >= 8 + i) {
            instance->SELECTIONS.value.table[i] = selection_vals[i] ? 1 : 0;
        }
    }
}

void print_fb_state(MULTIPUMPCTRL* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  MODE: %s\n", instance->MODE.value ? "TRUE" : "FALSE");
    printf("  START: %s\n", instance->START.value ? "TRUE" : "FALSE");
    printf("  STOP: %s\n", instance->STOP.value ? "TRUE" : "FALSE");
    
    printf("  PRIORITIES: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", (int)instance->PRIORITIES.value.table[i]);
    }
    printf("\n");
    
    printf("  SELECTIONS: ");
    for (int i = 0; i < 5; i++) {
        printf("%s ", instance->SELECTIONS.value.table[i] ? "T" : "F");
    }
    printf("\n");
    
    printf("Outputs:\n");
    printf("  RUNCOMDS: ");
    for (int i = 0; i < 5; i++) {
        printf("%s ", instance->RUNCOMDS.value.table[i] ? "T" : "F");
    }
    printf("\n");
    
    printf("Internal State:\n");
    printf("  I: %d\n", (int)instance->I.value);
    printf("  J: %d\n", (int)instance->J.value);
    printf("  MAXPRIORITY: %d\n", (int)instance->MAXPRIORITY.value);
    printf("  MAXINDEX: %d\n", (int)instance->MAXINDEX.value);
    printf("  RUNCOUNT: %d\n", (int)instance->RUNCOUNT.value);
    
    printf("  USEDINAUTO: ");
    for (int i = 0; i < 5; i++) {
        printf("%s ", instance->USEDINAUTO.value.table[i] ? "T" : "F");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    MULTIPUMPCTRL instance;
    MULTIPUMPCTRL_init__(&instance, FALSE);
    
    int cycle = 0;
    printf("MultiPumpCtrl Harness Started\n");
    printf("Input format: mode start stop p1 p2 p3 p4 p5 s1 s2 s3 s4 s5\n");
    printf("  mode, start, stop: 0=FALSE, 1=TRUE\n");
    printf("  p1-p5: integer priorities\n");
    printf("  s1-s5: 0=FALSE, 1=TRUE selections\n");
    printf("Example: 1 1 0 10 20 30 40 50 0 1 0 1 0\n");
    printf("------------------------------------------------\n");
    
    while (1) {
        printf("[DEBUG] --- Cycle %d: Start ---\n", cycle + 1); fflush(stdout);
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        printf("[DEBUG] Entering MULTIPUMPCTRL_body__\n"); fflush(stdout);
        MULTIPUMPCTRL_body__(&instance);
        printf("[DEBUG] Exited MULTIPUMPCTRL_body__\n"); fflush(stdout);

        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}