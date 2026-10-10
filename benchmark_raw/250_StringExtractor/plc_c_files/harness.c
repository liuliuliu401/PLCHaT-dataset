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

void read_inputs(GETSTRING* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input format: "TEXTBEFORE TEXTAFTER INCLUDEBEFOREAFTER STARTPOS charArray[1] ... charArray[100]"
    char textBefore_buf[81] = {0};
    char textAfter_buf[81] = {0};
    int includeBeforeAfter_int;
    long startPos_long;
    char arrayValues[100][81] = {{0}};
    
    // Try to parse the first 4 fields
    int matched = sscanf(line, "%80s %80s %d %ld", 
                        textBefore_buf, textAfter_buf, 
                        &includeBeforeAfter_int, &startPos_long);
    
    if (matched >= 4) {
        // Set scalar inputs
        strncpy(instance->TEXTBEFORE.value.body, textBefore_buf, 80);
        instance->TEXTBEFORE.value.len = strnlen(textBefore_buf, 80);
        
        strncpy(instance->TEXTAFTER.value.body, textAfter_buf, 80);
        instance->TEXTAFTER.value.len = strnlen(textAfter_buf, 80);
        
        instance->INCLUDEBEFOREAFTER.value = (BOOL)includeBeforeAfter_int;
        instance->STARTPOS.value = (DINT)startPos_long;
        
        // Parse array values (if provided)
        char* token = line;
        int array_idx = 0;
        
        // Skip first 4 tokens
        for (int i = 0; i < 4; i++) {
            token = strchr(token, ' ');
            if (!token) break;
            token++; // Skip the space
        }
        
        // Parse remaining tokens as array elements
        while (token && *token && array_idx < 100) {
            // Find next space or end of line
            char* end = strchr(token, ' ');
            if (end) {
                *end = '\0';
                strncpy(arrayValues[array_idx], token, 80);
                arrayValues[array_idx][80] = '\0';
                instance->CHARARRAY.value.table[array_idx].len = strnlen(arrayValues[array_idx], 80);
                strncpy(instance->CHARARRAY.value.table[array_idx].body, arrayValues[array_idx], 80);
                token = end + 1;
            } else {
                // Last token
                strncpy(arrayValues[array_idx], token, 80);
                arrayValues[array_idx][80] = '\0';
                instance->CHARARRAY.value.table[array_idx].len = strnlen(arrayValues[array_idx], 80);
                strncpy(instance->CHARARRAY.value.table[array_idx].body, arrayValues[array_idx], 80);
                break;
            }
            array_idx++;
        }
        
        // If fewer than 100 array values provided, clear the rest
        for (int i = array_idx; i < 100; i++) {
            instance->CHARARRAY.value.table[i].len = 0;
            instance->CHARARRAY.value.table[i].body[0] = '\0';
        }
    }
}

void print_fb_state(GETSTRING* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  TEXTBEFORE: '%.*s'\n", (int)instance->TEXTBEFORE.value.len, instance->TEXTBEFORE.value.body);
    printf("  TEXTAFTER: '%.*s'\n", (int)instance->TEXTAFTER.value.len, instance->TEXTAFTER.value.body);
    printf("  INCLUDEBEFOREAFTER: %d\n", (int)instance->INCLUDEBEFOREAFTER.value);
    printf("  STARTPOS: %ld\n", (long)instance->STARTPOS.value);
    
    printf("Outputs:\n");
    printf("  RETSTRING: '%.*s'\n", (int)instance->RETSTRING.value.len, instance->RETSTRING.value.body);
    printf("  POSITION: %ld\n", (long)instance->POSITION.value);
    printf("  LENGTH: %ld\n", (long)instance->LENGTH.value);
    
    printf("Internal State:\n");
    printf("  BEFOREFOUND: %d\n", (int)instance->BEFOREFOUND.value);
    printf("  AFTERFOUND: %d\n", (int)instance->AFTERFOUND.value);
    printf("  BEFOREINDEX: %ld\n", (long)instance->BEFOREINDEX.value);
    printf("  AFTERINDEX: %ld\n", (long)instance->AFTERINDEX.value);
    printf("  CURRENTSTRING: '%.*s'\n", (int)instance->CURRENTSTRING.value.len, instance->CURRENTSTRING.value.body);
    
    printf("First 10 CHARARRAY elements:\n");
    for (int i = 0; i < 10 && i < 100; i++) {
        printf("  [%d]: '%.*s'\n", i+1, 
               (int)instance->CHARARRAY.value.table[i].len, 
               instance->CHARARRAY.value.table[i].body);
    }
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    GETSTRING instance;
    GETSTRING_init__(&instance, FALSE);
    
    printf("GetString FB Test Harness\n");
    printf("Input format: TEXTBEFORE TEXTAFTER INCLUDEBEFOREAFTER STARTPOS charArray[1] ... charArray[100]\n");
    printf("  TEXTBEFORE, TEXTAFTER: strings (max 80 chars)\n");
    printf("  INCLUDEBEFOREAFTER: 0=FALSE, 1=TRUE\n");
    printf("  STARTPOS: integer (1-100)\n");
    printf("  charArray[1..100]: 100 strings (max 80 chars each), space-separated\n");
    printf("Example: \"Hello\" \"World\" 1 1 \"H\" \"e\" \"l\" \"l\" \"o\" \" \" \"W\" \"o\" \"r\" \"l\" \"d\" ...\n");
    printf("Press Ctrl+C to exit\n\n");
    
    int cycle = 0;
    while (1) {
        if (!silent_mode) printf("\nEnter inputs: ");
        fflush(stdout);
        
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        GETSTRING_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}

