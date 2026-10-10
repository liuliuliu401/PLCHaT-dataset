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

void read_inputs(SHELLSORT_DINT* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse format: sortDirection arr[1..10] as space-separated values
    // Example input: 0 5 2 8 1 9 3 7 4 6 10  (sortDirection=0, arr=5,2,8,1,9,3,7,4,6,10)
    char* token = strtok(line, " \t\n\r");
    if (token == NULL) return;
    
    // Read sortDirection (BOOL)
    int temp_sort;
    if (sscanf(token, "%d", &temp_sort) != 1) return;
    instance->SORTDIRECTION.value = (temp_sort != 0);
    
    // Read array values (10 DINT values)
    long temp_vals[10];
    for (int i = 0; i < 10; i++) {
        token = strtok(NULL, " \t\n\r");
        if (token == NULL) {
            // Not enough values - fill remaining with 0
            temp_vals[i] = 0;
        } else {
            if (sscanf(token, "%ld", &temp_vals[i]) != 1) temp_vals[i] = 0;
        }
    }
    
    // Assign to ARR array (MatIEC array access)
    for (int i = 0; i < 10; i++) {
        instance->ARR.value.table[i] = temp_vals[i];
    }
}

void print_fb_state(SHELLSORT_DINT* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("SORTDIRECTION: %s\n", instance->SORTDIRECTION.value ? "TRUE" : "FALSE");
    printf("ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("STATUS: 0x%04X\n", (unsigned int)instance->STATUS.value);
    
    printf("INPUT ARR: ");
    for (int i = 0; i < 10; i++) {
        printf("%ld", (long)instance->ARR.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("\n");
    
    printf("OUTPUT SORTED_ARR: ");
    for (int i = 0; i < 10; i++) {
        printf("%ld", (long)instance->SORTED_ARR.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("\n");
    
    printf("Internal vars: I=%d, J=%d, GAP=%d, TEMP=%ld, N=%d\n",
           (int)instance->I.value,
           (int)instance->J.value,
           (int)instance->GAP.value,
           (long)instance->TEMP.value,
           (int)instance->N.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    SHELLSORT_DINT instance;
    SHELLSORT_DINT_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        SHELLSORT_DINT_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}