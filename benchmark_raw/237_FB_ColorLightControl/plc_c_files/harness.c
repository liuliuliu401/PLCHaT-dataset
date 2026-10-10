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

void read_inputs(FB_COLORLIGHTCONTROL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input for control button (0 or 1)
    int control_val;
    if (sscanf(line, "%d", &control_val) == 1) {
        instance->CONTROLBUTTON.value = (control_val != 0);
    }
}

void print_fb_state(FB_COLORLIGHTCONTROL* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  CONTROL_BUTTON: %s\n", instance->CONTROLBUTTON.value ? "TRUE" : "FALSE");
    printf("State:\n");
    printf("  CURRENT_STATE: %ld\n", (long)instance->CURRENTSTATE.value);
    printf("Outputs:\n");
    printf("  GREEN_LIGHT: %s\n", instance->GREENLIGHT.value ? "TRUE" : "FALSE");
    printf("  RED_LIGHT: %s\n", instance->REDLIGHT.value ? "TRUE" : "FALSE");
    printf("  YELLOW_LIGHT: %s\n", instance->YELLOWLIGHT.value ? "TRUE" : "FALSE");
    printf("\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    FB_COLORLIGHTCONTROL instance;
    FB_COLORLIGHTCONTROL_init__(&instance, FALSE);
    int cycle = 0;
    
    printf("FB_COLORLIGHTCONTROL Test Harness\n");
    printf("Input format: Enter 0 or 1 for control button (press Enter to run)\n");
    printf("Example: 1 (for TRUE) or 0 (for FALSE)\n");
    printf("Blank line or # to skip input (use previous value)\n");
    printf("Ctrl+C to exit\n");
    
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        FB_COLORLIGHTCONTROL_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}