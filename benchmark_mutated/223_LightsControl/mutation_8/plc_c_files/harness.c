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

void read_inputs(LIGHTSCONTROL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    int b1, b2, b3;
    if (sscanf(line, "%d %d %d", &b1, &b2, &b3) == 3) {
        instance->BUTTON1.value = (BOOL)b1;
        instance->BUTTON2.value = (BOOL)b2;
        instance->BUTTON3.value = (BOOL)b3;
    }
}

void print_fb_state(LIGHTSCONTROL* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs: BUTTON1=%d, BUTTON2=%d, BUTTON3=%d\n", 
           instance->BUTTON1.value, 
           instance->BUTTON2.value, 
           instance->BUTTON3.value);
    printf("Outputs: GREENLIGHT=%d, REDLIGHT=%d, YELLOWLIGHT=%d\n", 
           instance->GREENLIGHT.value, 
           instance->REDLIGHT.value, 
           instance->YELLOWLIGHT.value);
    printf("State: CURRENTSTATE=%d, AUTORUNTIMER=%d\n", 
           instance->CURRENTSTATE.value, 
           instance->AUTORUNTIMER.value);
    printf("Timers Q: TIMERAUTO=%d, TIMERGREEN=%d, TIMERRED=%d, TIMERYELLOW=%d\n",
           instance->TIMERAUTO.Q,
           instance->TIMERGREEN.Q,
           instance->TIMERRED.Q,
           instance->TIMERYELLOW.Q);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    LIGHTSCONTROL instance;
    LIGHTSCONTROL_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        LIGHTSCONTROL_body__(&instance);
        /* Safety Invariant Assertion - State Lock Detection */
        if (instance.BUTTON2.value == FALSE && instance.BUTTON3.value == TRUE) {
            static int last_state = -1;
            static int stuck_cycles = 0;
            if (instance.CURRENTSTATE.value == last_state) {
                stuck_cycles++;
                if (stuck_cycles > 100) { /* Watchdog threshold */
                    fprintf(stderr, "AUDIT FAIL: State %d locked for 100 cycles - Liveness violation\n", 
                            instance.CURRENTSTATE.value);
                    exit(1);
                }
            } else {
                last_state = instance.CURRENTSTATE.value;
                stuck_cycles = 0;
            }
        }
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}
