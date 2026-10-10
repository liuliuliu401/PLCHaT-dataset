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

void read_inputs(FB_DATABASEMANAGER* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Format: dataInput[0..5] (6 bytes), storeTrigger, resetTrigger
    // Bytes as decimal values (0-255)
    unsigned temp_bytes[6];
    int store_trigger, reset_trigger;
    
    if (sscanf(line, "%u %u %u %u %u %u %d %d",
               &temp_bytes[0], &temp_bytes[1], &temp_bytes[2],
               &temp_bytes[3], &temp_bytes[4], &temp_bytes[5],
               &store_trigger, &reset_trigger) >= 8) {
        // Set dataInput array (0-indexed in C, matching ST 0..5)
        for (int i = 0; i < 6; i++) {
            instance->DATAINPUT.value.table[i] = (BYTE)(temp_bytes[i] & 0xFF);
        }
        instance->STORETRIGGER.value = store_trigger ? 1 : 0;
        instance->RESETTRIGGER.value = reset_trigger ? 1 : 0;
    }
}

void print_fb_state(FB_DATABASEMANAGER* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print inputs
    printf("Inputs:\n");
    printf("  dataInput: [");
    for (int i = 0; i < 6; i++) {
        printf("%u", (unsigned)instance->DATAINPUT.value.table[i]);
        if (i < 5) printf(" ");
    }
    printf("]\n");
    printf("  storeTrigger: %s\n", instance->STORETRIGGER.value ? "TRUE" : "FALSE");
    printf("  resetTrigger: %s\n", instance->RESETTRIGGER.value ? "TRUE" : "FALSE");
    
    // Print outputs
    printf("Outputs:\n");
    printf("  usedSpace: %d\n", (int)instance->USEDSPACE.value);
    printf("  remainingSpace: %d\n", (int)instance->REMAININGSPACE.value);
    printf("  error: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("  status: 0x%04X\n", (unsigned)instance->STATUS.value);
    
    // Print database (IN_OUT variable)
    printf("Database (size 20): [");
    for (int i = 0; i < 20; i++) {
        printf("%u", (unsigned)instance->DATABASE.value.table[i]);
        if (i < 19) printf(" ");
    }
    printf("]\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_DATABASEMANAGER instance;
    FB_DATABASEMANAGER_init__(&instance, FALSE);
    
    printf("FB_DatabaseManager Harness\n");
    printf("Input format: 6 bytes (0-255 decimal) storeTrigger(0/1) resetTrigger(0/1)\n");
    printf("Example: 5 10 20 30 40 50 1 0\n");
    printf("Press Ctrl+C to exit\n\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_DATABASEMANAGER_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}
