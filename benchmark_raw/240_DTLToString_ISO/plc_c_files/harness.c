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

void read_inputs(DTTOSTRING_ISO* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse DT input (DT is represented as ULINT in MatIEC)
    // Format: DT as 64-bit timestamp (seconds since 1970-01-01 00:00:00)
    // Example input: "1707311234 45" (DT timestamp and separator byte)
    unsigned long long dt_val;
    unsigned char sep_val;
    
    if (sscanf(line, "%llu %hhu", &dt_val, &sep_val) == 2) {
        instance->IN_DATE.value.tv_sec = (long)dt_val;
        instance->IN_DATE.value.tv_nsec = 0;
        instance->SEPARATOR.value = sep_val;
    }
    // If parsing fails, keep current values
}

void print_fb_state(DTTOSTRING_ISO* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print input values
    printf("IN_DATE (DT): %llu\n", instance->IN_DATE.value);
    printf("SEPARATOR: %u (ASCII: '%c')\n", 
           (unsigned int)instance->SEPARATOR.value,
           (char)instance->SEPARATOR.value);
    
    // Print internal date/time components
    printf("Internal date/time:\n");
    printf("  YEAR: %d\n", (int)instance->YEAR.value);
    printf("  MONTH: %d\n", (int)instance->MONTH.value);
    printf("  DAY: %d\n", (int)instance->DAY.value);
    printf("  HOUR: %d\n", (int)instance->HOUR.value);
    printf("  MINUTE: %d\n", (int)instance->MINUTE.value);
    printf("  SECOND: %d\n", (int)instance->SECOND.value);
    
    // Print intermediate strings (first few chars only)
    printf("Intermediate strings:\n");
    printf("  S_YEAR: \"%.*s\"\n", instance->S_YEAR.value.len, instance->S_YEAR.value.body);
    printf("  S_MONTH: \"%.*s\"\n", instance->S_MONTH.value.len, instance->S_MONTH.value.body);
    printf("  S_DAY: \"%.*s\"\n", instance->S_DAY.value.len, instance->S_DAY.value.body);
    printf("  S_HOUR: \"%.*s\"\n", instance->S_HOUR.value.len, instance->S_HOUR.value.body);
    printf("  S_MIN: \"%.*s\"\n", instance->S_MIN.value.len, instance->S_MIN.value.body);
    printf("  S_SEC: \"%.*s\"\n", instance->S_SEC.value.len, instance->S_SEC.value.body);
    printf("  S_SEP: \"%.*s\"\n", instance->S_SEP.value.len, instance->S_SEP.value.body);
    
    // Print final result string
    printf("RESULTSTRING: \"%.*s\"\n", 
           instance->RESULTSTRING.value.len, 
           instance->RESULTSTRING.value.body);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
        if (strcmp(argv[i], "--debug") == 0) __DEBUG = 1;
    }
    
    DTTOSTRING_ISO instance;
    DTTOSTRING_ISO_init__(&instance, FALSE);
    
    int cycle = 0;
    
    // Print initial state
    if (!silent_mode) {
        printf("DTToString_ISO Test Harness\n");
        printf("Input format: <DT_timestamp> <separator_byte>\n");
        printf("Example: 1707311234 45 (45 is '-' ASCII)\n");
        printf("Initial state:\n");
        print_fb_state(&instance, 0);
    }
    
    while (1) {
        if (!silent_mode) printf("\nEnter inputs: ");
        
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        DTTOSTRING_ISO_body__(&instance);
        
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
