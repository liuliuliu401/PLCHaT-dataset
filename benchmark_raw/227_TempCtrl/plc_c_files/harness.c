#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "iec_types_all.h"
#include "iec_std_lib.h"
#include "POUS.h"

TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;

void read_inputs(TEMPCTRL* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse inputs: TemperatureSensorInput (REAL) and SetTemperature (REAL)
    // Format: <TemperatureSensorInput> <SetTemperature>
    float temp_sensor_input, set_temp;
    if (sscanf(line, "%f %f", &temp_sensor_input, &set_temp) == 2) {
        instance->TEMPERATURESENSORINPUT.value = temp_sensor_input;
        instance->SETTEMPERATURE.value = set_temp;
    }
}

void print_fb_state(TEMPCTRL* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  TemperatureSensorInput: %.2f\n", instance->TEMPERATURESENSORINPUT.value);
    printf("  SetTemperature: %.2f\n", instance->SETTEMPERATURE.value);
    printf("Outputs:\n");
    printf("  CurrentTemperature: %.2f\n", instance->CURRENTTEMPERATURE.value);
    printf("  SetTempDisplay: %.2f\n", instance->SETTEMPDISPLAY.value);
    printf("  HeaterStatus: %d\n", instance->HEATERSTATUS.value);
    printf("  OverheatProtection: %d\n", instance->OVERHEATPROTECTION.value);
    printf("  HeaterOutput: %d\n", instance->HEATEROUTPUT.value);
    printf("Internal:\n");
    printf("  LastHeaterState: %d\n", instance->LASTHEATERSTATE.value);
    printf("  TimerHeater.Q: %d\n", instance->TIMERHEATER.Q);
    printf("  TimerHeater.Q: %d\n", instance->TIMERHEATER.Q);
    printf("  TimerHeater.ET: %ld.%09lds\n", 
           (long)instance->TIMERHEATER.ET.value.tv_sec, 
           (long)instance->TIMERHEATER.ET.value.tv_nsec);

    printf("  TimerOverheat.Q: %d\n", instance->TIMEROVERHEAT.Q);
    printf("  TimerOverheat.ET: %ld.%09lds\n", 
           (long)instance->TIMEROVERHEAT.ET.value.tv_sec, 
           (long)instance->TIMEROVERHEAT.ET.value.tv_nsec);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    TEMPCTRL instance;
    TEMPCTRL_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        TEMPCTRL_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
