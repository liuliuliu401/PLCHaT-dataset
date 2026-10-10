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

void read_inputs(PRODUCTIONCONTROL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Temporary variables for parsing
    int mode_val, forwardbutton_val, reversebutton_val;
    int sensorfeed_val, sensorwash_val, sensorweight_val;
    int sensorfiring_val, sensoroutput_val;
    int washcomplete_val, weightcomplete_val, firingcomplete_val;
    float weight_val;
    
    // Parse input values in the order they appear in the ST code
    // Format: MODE FORWARDBUTTON REVERSEBUTTON SENSORFEED SENSORWASH SENSORWEIGHT SENSORFIRING SENSOROUTPUT WASHCOMPLETE WEIGHTCOMPLETE WEIGHT FIRINGCOMPLETE
    int parsed = sscanf(line, 
        "%d %d %d %d %d %d %d %d %d %d %f %d",
        &mode_val, &forwardbutton_val, &reversebutton_val,
        &sensorfeed_val, &sensorwash_val, &sensorweight_val,
        &sensorfiring_val, &sensoroutput_val,
        &washcomplete_val, &weightcomplete_val,
        &weight_val, &firingcomplete_val);
    
    if (parsed < 12) return; // Not enough values
    
    // Assign values to FB instance (using UPPERCASE names as in POUS.h)
    instance->MODE.value = mode_val ? TRUE : FALSE;
    instance->FORWARDBUTTON.value = forwardbutton_val ? TRUE : FALSE;
    instance->REVERSEBUTTON.value = reversebutton_val ? TRUE : FALSE;
    instance->SENSORFEED.value = sensorfeed_val ? TRUE : FALSE;
    instance->SENSORWASH.value = sensorwash_val ? TRUE : FALSE;
    instance->SENSORWEIGHT.value = sensorweight_val ? TRUE : FALSE;
    instance->SENSORFIRING.value = sensorfiring_val ? TRUE : FALSE;
    instance->SENSOROUTPUT.value = sensoroutput_val ? TRUE : FALSE;
    instance->WASHCOMPLETE.value = washcomplete_val ? TRUE : FALSE;
    instance->WEIGHTCOMPLETE.value = weightcomplete_val ? TRUE : FALSE;
    instance->WEIGHT.value = weight_val;
    instance->FIRINGCOMPLETE.value = firingcomplete_val ? TRUE : FALSE;
}

void print_fb_state(PRODUCTIONCONTROL* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  MODE: %d\n", instance->MODE.value);
    printf("  FORWARDBUTTON: %d\n", instance->FORWARDBUTTON.value);
    printf("  REVERSEBUTTON: %d\n", instance->REVERSEBUTTON.value);
    printf("  SENSORFEED: %d\n", instance->SENSORFEED.value);
    printf("  SENSORWASH: %d\n", instance->SENSORWASH.value);
    printf("  SENSORWEIGHT: %d\n", instance->SENSORWEIGHT.value);
    printf("  SENSORFIRING: %d\n", instance->SENSORFIRING.value);
    printf("  SENSOROUTPUT: %d\n", instance->SENSOROUTPUT.value);
    printf("  WASHCOMPLETE: %d\n", instance->WASHCOMPLETE.value);
    printf("  WEIGHTCOMPLETE: %d\n", instance->WEIGHTCOMPLETE.value);
    printf("  WEIGHT: %.2f\n", instance->WEIGHT.value);
    printf("  FIRINGCOMPLETE: %d\n", instance->FIRINGCOMPLETE.value);
    
    printf("\nOutputs:\n");
    printf("  MOTORFORWARD: %d\n", instance->MOTORFORWARD.value);
    printf("  MOTORREVERSE: %d\n", instance->MOTORREVERSE.value);
    printf("  KICK: %d\n", instance->KICK.value);
    printf("  COMPLETIONLIGHT: %d\n", instance->COMPLETIONLIGHT.value);
    printf("  FIRINGTEMP: %.2f\n", instance->FIRINGTEMP.value);
    
    printf("\nInternal States:\n");
    printf("  ISMANUAL: %d\n", instance->ISMANUAL.value);
    printf("  ISAUTO: %d\n", instance->ISAUTO.value);
    printf("  ISATFEED: %d\n", instance->ISATFEED.value);
    printf("  ISATWASH: %d\n", instance->ISATWASH.value);
    printf("  ISATWEIGHT: %d\n", instance->ISATWEIGHT.value);
    printf("  ISATFIRING: %d\n", instance->ISATFIRING.value);
    printf("  ISATOUTPUT: %d\n", instance->ISATOUTPUT.value);
    printf("  ISQUALIFIED: %d\n", instance->ISQUALIFIED.value);
    printf("---\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    PRODUCTIONCONTROL instance;
    PRODUCTIONCONTROL_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        PRODUCTIONCONTROL_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}

