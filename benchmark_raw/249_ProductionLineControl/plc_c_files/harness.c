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

void read_inputs(FB_PRODUCTIONLINECONTROL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    int mode, forwardButton, reverseButton, sensorA, sensorB, sensorC;
    int buttonA, buttonB, buttonC;
    
    // Parse all inputs as integers (0/1) for BOOL type
    if (sscanf(line, "%d %d %d %d %d %d %d %d %d", 
               &mode, &forwardButton, &reverseButton, 
               &sensorA, &sensorB, &sensorC,
               &buttonA, &buttonB, &buttonC) == 9) {
        instance->MODE.value = mode;
        instance->FORWARDBUTTON.value = forwardButton;
        instance->REVERSEBUTTON.value = reverseButton;
        instance->SENSORA.value = sensorA;
        instance->SENSORB.value = sensorB;
        instance->SENSORC.value = sensorC;
        instance->BUTTONA.value = buttonA;
        instance->BUTTONB.value = buttonB;
        instance->BUTTONC.value = buttonC;
    }
}

void print_fb_state(FB_PRODUCTIONLINECONTROL* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  MODE: %d\n", instance->MODE.value);
    printf("  FORWARDBUTTON: %d\n", instance->FORWARDBUTTON.value);
    printf("  REVERSEBUTTON: %d\n", instance->REVERSEBUTTON.value);
    printf("  SENSORA: %d  SENSORB: %d  SENSORC: %d\n", 
           instance->SENSORA.value, instance->SENSORB.value, instance->SENSORC.value);
    printf("  BUTTONA: %d  BUTTONB: %d  BUTTONC: %d\n", 
           instance->BUTTONA.value, instance->BUTTONB.value, instance->BUTTONC.value);
    printf("Outputs:\n");
    printf("  MOTORFORWARD: %d\n", instance->MOTORFORWARD.value);
    printf("  MOTORREVERSE: %d\n", instance->MOTORREVERSE.value);
    printf("  COMPLETIONLIGHT: %d\n", instance->COMPLETIONLIGHT.value);
    printf("Internal States:\n");
    printf("  LASTSENSORA: %d  LASTSENSORB: %d  LASTSENSORC: %d\n",
           instance->LASTSENSORA.value, instance->LASTSENSORB.value, instance->LASTSENSORC.value);
    printf("  LASTBUTTONA: %d  LASTBUTTONB: %d  LASTBUTTONC: %d\n",
           instance->LASTBUTTONA.value, instance->LASTBUTTONB.value, instance->LASTBUTTONC.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_PRODUCTIONLINECONTROL instance;
    FB_PRODUCTIONLINECONTROL_init__(&instance, FALSE);
    
    printf("Starting FB_ProductionLineControl test harness\n");
    printf("Enter inputs as space-separated integers (0 or 1):\n");
    printf("Format: MODE FORWARDBUTTON REVERSEBUTTON SENSORA SENSORB SENSORC BUTTONA BUTTONB BUTTONC\n");
    printf("Example: 1 0 0 1 0 0 0 0 0\n");
    printf("Press Ctrl+C to exit\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_PRODUCTIONLINECONTROL_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}

