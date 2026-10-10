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

void read_inputs(TRUCKGARAGE* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse: ENTER_FLAG EXIT_FLAG VEHICLENUMBER
    int enter_flag, exit_flag, vehicle_number;
    if (sscanf(line, "%d %d %d", &enter_flag, &exit_flag, &vehicle_number) == 3) {
        instance->ENTER_FLAG.value = enter_flag;
        instance->EXIT_FLAG.value = exit_flag;
        instance->VEHICLENUMBER.value = vehicle_number;
    }
}

void print_fb_state(TRUCKGARAGE* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("ENTER_FLAG: %d\n", instance->ENTER_FLAG.value);
    printf("EXIT_FLAG: %d\n", instance->EXIT_FLAG.value);
    printf("VEHICLENUMBER: %d\n", instance->VEHICLENUMBER.value);
    printf("TRUCK - ROW: %d, COL: %d, VEHICLENUMBER: %d\n",
           instance->TRUCK.value.ROW,
           instance->TRUCK.value.COL,
           instance->TRUCK.value.VEHICLENUMBER);
    
    printf("GARAGE slots (Occupied, VehicleNumber):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            printf("  [%d][%d]: %d %d\n",
                   i+1, j+1,
                   instance->GARAGE.value.table[i][j].OCCUPIED,
                   instance->GARAGE.value.table[i][j].VEHICLENUMBER);
        }
    }
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    TRUCKGARAGE instance;
    TRUCKGARAGE_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        TRUCKGARAGE_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}