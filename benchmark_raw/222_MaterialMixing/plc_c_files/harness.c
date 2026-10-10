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

void read_inputs(MATERIALMIXING* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Format: estop opeMode valveAStart valveBStart valveCStart valveDStart mixMotorStart autoStart processMode levelSensor mixMotorComplete
    int temp_estop, temp_opemode, temp_valveastart, temp_valvebstart, temp_valvecstart, temp_valvedstart;
    int temp_mixmotorstart, temp_autostart, temp_processmode, temp_levelsensor, temp_mixmotorcomplete;
    
    if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d",
               &temp_estop, &temp_opemode, &temp_valveastart, &temp_valvebstart,
               &temp_valvecstart, &temp_valvedstart, &temp_mixmotorstart,
               &temp_autostart, &temp_processmode, &temp_levelsensor,
               &temp_mixmotorcomplete) == 11) {
        instance->ESTOP.value = temp_estop;
        instance->OPEMODE.value = temp_opemode;
        instance->VALVEASTART.value = temp_valveastart;
        instance->VALVEBSTART.value = temp_valvebstart;
        instance->VALVECSTART.value = temp_valvecstart;
        instance->VALVEDSTART.value = temp_valvedstart;
        instance->MIXMOTORSTART.value = temp_mixmotorstart;
        instance->AUTOSTART.value = temp_autostart;
        instance->PROCESSMODE.value = temp_processmode;
        instance->LEVELSENSOR.value = temp_levelsensor;
        instance->MIXMOTORCOMPLETE.value = temp_mixmotorcomplete;
    }
}

void print_fb_state(MATERIALMIXING* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  ESTOP: %d\n", instance->ESTOP.value);
    printf("  OPEMODE: %d\n", instance->OPEMODE.value);
    printf("  VALVEASTART: %d\n", instance->VALVEASTART.value);
    printf("  VALVEBSTART: %d\n", instance->VALVEBSTART.value);
    printf("  VALVECSTART: %d\n", instance->VALVECSTART.value);
    printf("  VALVEDSTART: %d\n", instance->VALVEDSTART.value);
    printf("  MIXMOTORSTART: %d\n", instance->MIXMOTORSTART.value);
    printf("  AUTOSTART: %d\n", instance->AUTOSTART.value);
    printf("  PROCESSMODE: %d\n", instance->PROCESSMODE.value);
    printf("  LEVELSENSOR: %d\n", instance->LEVELSENSOR.value);
    printf("  MIXMOTORCOMPLETE: %d\n", instance->MIXMOTORCOMPLETE.value);
    
    printf("\nOutputs:\n");
    printf("  VALVEARUN: %d\n", instance->VALVEARUN.value);
    printf("  VALVEBRUN: %d\n", instance->VALVEBRUN.value);
    printf("  VALVECRUN: %d\n", instance->VALVECRUN.value);
    printf("  VALVEDRUN: %d\n", instance->VALVEDRUN.value);
    printf("  MIXMOTORRUN: %d\n", instance->MIXMOTORRUN.value);
    
    printf("\nInternal State:\n");
    printf("  AUTOSTEP: %d\n", instance->AUTOSTEP.value);
    printf("  VALVEA_RE: %d\n", instance->VALVEA_RE.value);
    printf("  VALVEB_RE: %d\n", instance->VALVEB_RE.value);
    printf("  VALVEC_RE: %d\n", instance->VALVEC_RE.value);
    printf("  VALVED_RE: %d\n", instance->VALVED_RE.value);
    printf("  MIXMOTOR_RE: %d\n", instance->MIXMOTOR_RE.value);
    printf("  AUTOSTART_RE: %d\n", instance->AUTOSTART_RE.value);
    printf("  PREVIOUSVALVEASTART: %d\n", instance->PREVIOUSVALVEASTART.value);
    printf("  PREVIOUSVALVEBSTART: %d\n", instance->PREVIOUSVALVEBSTART.value);
    printf("  PREVIOUSVALVECSTART: %d\n", instance->PREVIOUSVALVECSTART.value);
    printf("  PREVIOUSVALVEDSTART: %d\n", instance->PREVIOUSVALVEDSTART.value);
    printf("  PREVIOUSMIXMOTORSTART: %d\n", instance->PREVIOUSMIXMOTORSTART.value);
    printf("  PREVIOUSAUTOSTART: %d\n", instance->PREVIOUSAUTOSTART.value);
    printf("------------------------\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    MATERIALMIXING instance;
    MATERIALMIXING_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        MATERIALMIXING_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}