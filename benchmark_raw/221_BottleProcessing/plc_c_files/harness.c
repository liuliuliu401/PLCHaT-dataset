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

void read_inputs(FB_BOTTLEPROCESSING* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse inputs in order: bottleSensor, cleaningConfirmButton, fillingConfirmButton, 
    // cappingConfirmButton, packingConfirmButton, finishedButton
    int bs, ccb, fcb, capb, pcb, fb;
    if (sscanf(line, "%d %d %d %d %d %d", 
               &bs, &ccb, &fcb, &capb, &pcb, &fb) >= 6) {
        instance->BOTTLESENSOR.value = bs;
        instance->CLEANINGCONFIRMBUTTON.value = ccb;
        instance->FILLINGCONFIRMBUTTON.value = fcb;
        instance->CAPPINGCONFIRMBUTTON.value = capb;
        instance->PACKINGCONFIRMBUTTON.value = pcb;
        instance->FINISHEDBUTTON.value = fb;
    }
}

void print_fb_state(FB_BOTTLEPROCESSING* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs: bottleSensor=%d cleaningConfirmButton=%d fillingConfirmButton=%d cappingConfirmButton=%d packingConfirmButton=%d finishedButton=%d\n",
           instance->BOTTLESENSOR.value,
           instance->CLEANINGCONFIRMBUTTON.value,
           instance->FILLINGCONFIRMBUTTON.value,
           instance->CAPPINGCONFIRMBUTTON.value,
           instance->PACKINGCONFIRMBUTTON.value,
           instance->FINISHEDBUTTON.value);
    printf("Internal States: cleaningStep=%d fillingStep=%d cappingStep=%d packingStep=%d completionStep=%d\n",
           instance->CLEANINGSTEP.value,
           instance->FILLINGSTEP.value,
           instance->CAPPINGSTEP.value,
           instance->PACKINGSTEP.value,
           instance->COMPLETIONSTEP.value);
    printf("Outputs: Pump_Motor=%d Filling_Valve=%d Capping_Machine=%d Packing_Machine=%d Completion_Light=%d\n",
           instance->PUMP_MOTOR.value,
           instance->FILLING_VALVE.value,
           instance->CAPPING_MACHINE.value,
           instance->PACKING_MACHINE.value,
           instance->COMPLETION_LIGHT.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    FB_BOTTLEPROCESSING instance;
    FB_BOTTLEPROCESSING_init__(&instance, FALSE);
    int cycle = 0;
    printf("Starting FB_BottleProcessing Harness\n");
    printf("Input format: bottleSensor cleaningConfirmButton fillingConfirmButton cappingConfirmButton packingConfirmButton finishedButton\n");
    printf("Use 1 for TRUE, 0 for FALSE, space-separated\n");
    printf("Example: 1 0 0 0 0 0\n");
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        FB_BOTTLEPROCESSING_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}