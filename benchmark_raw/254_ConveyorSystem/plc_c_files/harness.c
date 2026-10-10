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

void read_inputs(CONVEYORSYSTEM* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    int startbutton, sensorstart, sensor1, sensor2, sensor3;
    int unloadcomplete1, unloadcomplete2, unloadcomplete3;
    int partspec, partnumber;
    
    // Parse all inputs in one line format
    if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d",
               &startbutton, &sensorstart, &sensor1, &sensor2, &sensor3,
               &unloadcomplete1, &unloadcomplete2, &unloadcomplete3,
               &partspec, &partnumber) == 10) {
        instance->STARTBUTTON.value = startbutton;
        instance->SENSORSTART.value = sensorstart;
        instance->SENSOR1.value = sensor1;
        instance->SENSOR2.value = sensor2;
        instance->SENSOR3.value = sensor3;
        instance->UNLOADCOMPLETE1.value = unloadcomplete1;
        instance->UNLOADCOMPLETE2.value = unloadcomplete2;
        instance->UNLOADCOMPLETE3.value = unloadcomplete3;
        instance->PARTSPEC.value = partspec;
        instance->PARTNUMBER.value = partnumber;
    }
}

void print_fb_state(CONVEYORSYSTEM* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n=== Cycle %d ===\n", cycle);
    
    // Inputs
    printf("Inputs:\n");
    printf("  StartButton: %d\n", instance->STARTBUTTON.value);
    printf("  SensorStart: %d\n", instance->SENSORSTART.value);
    printf("  Sensor1: %d, Sensor2: %d, Sensor3: %d\n", 
           instance->SENSOR1.value, instance->SENSOR2.value, instance->SENSOR3.value);
    printf("  UnloadComplete1: %d, UnloadComplete2: %d, UnloadComplete3: %d\n",
           instance->UNLOADCOMPLETE1.value, instance->UNLOADCOMPLETE2.value, instance->UNLOADCOMPLETE3.value);
    printf("  PartSpec: %d, PartNumber: %d\n", 
           instance->PARTSPEC.value, instance->PARTNUMBER.value);
    
    // Outputs
    printf("Outputs:\n");
    printf("  ConveyorRun: %d\n", instance->CONVEYORRUN.value);
    
    // Warehouse databases (0-indexed in C, but displaying as 1-indexed for clarity)
    printf("  WarehouseDatabase1: [");
    for (int i = 0; i < 10; i++) {
        printf("%d", instance->WAREHOUSEDATABASE1.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("]\n");
    
    printf("  WarehouseDatabase2: [");
    for (int i = 0; i < 10; i++) {
        printf("%d", instance->WAREHOUSEDATABASE2.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("]\n");
    
    printf("  WarehouseDatabase3: [");
    for (int i = 0; i < 10; i++) {
        printf("%d", instance->WAREHOUSEDATABASE3.value.table[i]);
        if (i < 9) printf(", ");
    }
    printf("]\n");
    
    // Internal state
    printf("Internal State:\n");
    printf("  CurrentIndex1: %d, CurrentIndex2: %d, CurrentIndex3: %d\n",
           instance->CURRENTINDEX1.value, instance->CURRENTINDEX2.value, instance->CURRENTINDEX3.value);
    printf("  WaitState: %d\n", instance->WAITSTATE.value);
    printf("================\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    CONVEYORSYSTEM instance;
    CONVEYORSYSTEM_init__(&instance, FALSE);
    
    printf("ConveyorSystem Test Harness\n");
    printf("Input format: StartButton SensorStart Sensor1 Sensor2 Sensor3 ");
    printf("UnloadComplete1 UnloadComplete2 UnloadComplete3 PartSpec PartNumber\n");
    printf("Example: 1 1 0 0 0 1 0 0 1 100\n");
    printf("Enter '#' for comments, Ctrl+C to exit\n\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        CONVEYORSYSTEM_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
