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

void read_inputs(SPLITBYTEARRAY* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse 10 bytes for BYTEARRAY
    unsigned int byte_vals[10];
    if (sscanf(line, "%u %u %u %u %u %u %u %u %u %u",
               &byte_vals[0], &byte_vals[1], &byte_vals[2],
               &byte_vals[3], &byte_vals[4], &byte_vals[5],
               &byte_vals[6], &byte_vals[7], &byte_vals[8],
               &byte_vals[9]) == 10) {
        for (int i = 0; i < 10; i++) {
            instance->BYTEARRAY.value.table[i] = (BYTE)(byte_vals[i] & 0xFF);
        }
    }
}

void print_fb_state(SPLITBYTEARRAY* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    printf("Input BYTEARRAY[1..10]: ");
    for (int i = 0; i < 10; i++) {
        printf("%u ", (unsigned int)instance->BYTEARRAY.value.table[i]);
    }
    printf("\n");
    
    printf("Output BITARRAY[1..80]: ");
    for (int i = 0; i < 80 && i < 20; i++) { // Show first 20 bits
        printf("%d ", instance->BITARRAY.value.table[i] ? 1 : 0);
    }
    if (80 > 20) printf("... (truncated)");
    printf("\n");
    
    printf("SPLITBYTENUM: %ld\n", (long)instance->SPLITBYTENUM.value);
    
    // Internal variables for debugging
    printf("Internal: I=%d J=%d K=%d TMPBYTE=%u\n",
           (int)instance->I.value,
           (int)instance->J.value,
           (int)instance->K.value,
           (unsigned int)instance->TMPBYTE.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    SPLITBYTEARRAY instance;
    SPLITBYTEARRAY_init__(&instance, FALSE);
    
    printf("SPLITBYTEARRAY Test Harness\n");
    printf("Enter 10 byte values (0-255) separated by spaces:\n");
    printf("Example: 255 128 64 32 16 8 4 2 1 0\n");
    printf("Enter # to skip cycle, Ctrl-D to exit\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        SPLITBYTEARRAY_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
