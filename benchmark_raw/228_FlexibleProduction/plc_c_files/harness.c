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

void read_inputs(FLEXIBLEPRODUCTION* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse all boolean inputs and 8 integer array values
    int sensor1, sensor2, sensor3, sensor4, sensor5;
    int station1Start, station2Complete, station3Complete, station4Complete;
    int temp_array[8];
    
    sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
           &sensor1, &sensor2, &sensor3, &sensor4, &sensor5,
           &station1Start, &station2Complete, &station3Complete, &station4Complete,
           &temp_array[0], &temp_array[1], &temp_array[2], &temp_array[3],
           &temp_array[4], &temp_array[5], &temp_array[6], &temp_array[7]);
    
    // Assign boolean inputs (MatIEC uses BOOL type)
    instance->SENSOR1.value = sensor1 != 0;
    instance->SENSOR2.value = sensor2 != 0;
    instance->SENSOR3.value = sensor3 != 0;
    instance->SENSOR4.value = sensor4 != 0;
    instance->SENSOR5.value = sensor5 != 0;
    instance->STATION1START.value = station1Start != 0;
    instance->STATION2COMPLETE.value = station2Complete != 0;
    instance->STATION3COMPLETE.value = station3Complete != 0;
    instance->STATION4COMPLETE.value = station4Complete != 0;
    
    // Assign array values (note: ST array is 1..8, C is 0..7)
    for (int i = 0; i < 8; i++) {
        instance->PROCESSSEQUENCE.value.table[i] = temp_array[i];
    }
}

void print_fb_state(FLEXIBLEPRODUCTION* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print current state
    printf("Current State: %d\n", (int)instance->CURRENTSTATE.value);
    printf("Next State: %d\n", (int)instance->NEXTSTATE.value);
    printf("Sequence Index: %d\n", (int)instance->SEQUENCEINDEX.value);
    
    // Print moving flags
    printf("Moving: %s\n", instance->MOVING.value ? "TRUE" : "FALSE");
    printf("Processing Done: %s\n", instance->PROCESSINGDONE.value ? "TRUE" : "FALSE");
    printf("Request Move: %s\n", instance->REQUESTMOVE.value ? "TRUE" : "FALSE");
    
    // Print outputs
    printf("Conveyor Left: %s\n", instance->CONVEYORLEFT.value ? "TRUE" : "FALSE");
    printf("Conveyor Right: %s\n", instance->CONVEYORRIGHT.value ? "TRUE" : "FALSE");
    
    // Print current process sequence element (1-based indexing)
    int idx = instance->SEQUENCEINDEX.value;
    if (idx >= 1 && idx <= 8) {
        printf("Current Sequence Value: %d\n", (int)instance->PROCESSSEQUENCE.value.table[idx-1]);
    }
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FLEXIBLEPRODUCTION instance;
    FLEXIBLEPRODUCTION_init__(&instance, FALSE);
    
    printf("FlexibleProduction Harness Started\n");
    printf("Input format: sensor1 sensor2 sensor3 sensor4 sensor5 station1Start station2Complete station3Complete station4Complete processSequence[1..8]\n");
    printf("Enter values as 0/1 for booleans, integers for array elements (space separated)\n");
    printf("Example: 0 1 0 0 0 1 0 0 0 2 3 4 5 1 2 3 4\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FLEXIBLEPRODUCTION_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
