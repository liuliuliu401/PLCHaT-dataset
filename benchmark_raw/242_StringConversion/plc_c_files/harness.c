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

void read_inputs(STRINGCONVERT* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input line: first comes the string input, then mode (0/1)
    char input_str[81];  // STRING can hold up to 80 chars + null
    int mode_val;
    
    if (sscanf(line, "%80s %d", input_str, &mode_val) == 2) {
        // Copy string to instance->IN.value (STRING type)
        strncpy(instance->IN.value.body, input_str, 80);
        instance->IN.value.len = strlen(input_str);
        
        // Set mode boolean
        instance->MODE.value = (mode_val != 0);
    }
}

void print_fb_state(STRINGCONVERT* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print input string
    printf("IN: ");
    for (int i = 0; i < instance->IN.value.len; i++) {
        putchar(instance->IN.value.body[i]);
    }
    printf(" (len=%d)\n", instance->IN.value.len);
    
    // Print mode
    printf("MODE: %d\n", instance->MODE.value ? 1 : 0);
    
    // Print output string
    printf("OUT: ");
    for (int i = 0; i < instance->OUT.value.len; i++) {
        putchar(instance->OUT.value.body[i]);
    }
    printf(" (len=%d)\n", instance->OUT.value.len);
    
    // Print some internal variables for debugging
    printf("I: %ld\n", (long)instance->I.value);
    printf("TMPINT: %d\n", (int)instance->TMPINT.value);
    printf("CHAROFFSET: %d\n", (int)instance->CHAROFFSET.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    STRINGCONVERT instance;
    STRINGCONVERT_init__(&instance, FALSE);
    
    int cycle = 0;
    printf("StringConvert Test Harness\n");
    printf("Enter inputs: <string> <mode(0/1)>\n");
    printf("  mode=1: uppercase to lowercase\n");
    printf("  mode=0: lowercase to uppercase\n");
    printf("Example: \"Hello\" 1\n");
    printf("Example: \"WORLD\" 0\n");
    printf("(Max string length: 80 characters)\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        STRINGCONVERT_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
