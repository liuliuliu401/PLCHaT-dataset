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

void read_inputs(FB_CALCULATEDAYOFYEAR* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse YEAR, MONTH, DAY
    int temp_year, temp_month, temp_day;
    if (sscanf(line, "%d %d %d", &temp_year, &temp_month, &temp_day) == 3) {
        instance->YEAR.value = temp_year;
        instance->MONTH.value = temp_month;
        instance->DAY.value = temp_day;
    }
}

void print_fb_state(FB_CALCULATEDAYOFYEAR* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs: YEAR=%d MONTH=%d DAY=%d\n", 
           (int)instance->YEAR.value, 
           (int)instance->MONTH.value, 
           (int)instance->DAY.value);
    printf("Outputs: DAYOFYEAR=%d ERROR=%s STATUS=0x%04X\n", 
           (int)instance->DAYOFYEAR.value,
           instance->ERROR.value ? "TRUE" : "FALSE",
           (unsigned int)instance->STATUS.value);
    
    // Show internal array (for debugging)
    printf("DAYSINMONTH array: ");
    for (int i = 0; i < 12; i++) {
        printf("%d ", (int)instance->DAYSINMONTH.value.table[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_CALCULATEDAYOFYEAR instance;
    FB_CALCULATEDAYOFYEAR_init__(&instance, FALSE);
    
    // Initialize the DAYSINMONTH array (matches ST initialization)
    int days_init[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int i = 0; i < 12; i++) {
        instance.DAYSINMONTH.value.table[i] = days_init[i];
    }
    
    int cycle = 0;
    printf("Enter YEAR MONTH DAY (e.g., 2024 2 29):\n");
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_CALCULATEDAYOFYEAR_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}