/* Auto-generated Harness by PLCHaT */
/* DO NOT EDIT MANUALLY */

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
#define SCAN_CYCLE_MS 50  // PLC scan cycle: 50ms


/* Expanded scanf format string and temp variables */

/* Temp variable declarations */
int temp_sys_enable;
int temp_temp_ped_ns_raw;
int temp_temp_ped_ew_raw;

void read_inputs(FB_TRAFFICLIGHT* instance) {
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;

    /* Parse input: sys_enable, temp_ped_ns_raw, temp_ped_ew_raw */

    int parsed = sscanf(line, "%d %d %d",
        &temp_sys_enable,
        &temp_temp_ped_ns_raw,
        &temp_temp_ped_ew_raw
    );

    if (parsed < 3) return;

    /* Map parsed values to instance */
    /* sys_enable: BOOL */
    instance->SYS_ENABLE.value = temp_sys_enable ? 1 : 0;
    /* temp_ped_ns_raw: BOOL */
    instance->TEMP_PED_NS_RAW.value = temp_temp_ped_ns_raw ? 1 : 0;
    /* temp_ped_ew_raw: BOOL */
    instance->TEMP_PED_EW_RAW.value = temp_temp_ped_ew_raw ? 1 : 0;
}

void print_fb_state(FB_TRAFFICLIGHT* instance, int cycle) {
    if (silent_mode) return;

    printf("\n--- Cycle %d ---\n", cycle);

    /* Inputs */
    printf("sys_enable: %s\n", instance->SYS_ENABLE.value ? "TRUE" : "FALSE");
    printf("temp_ped_ns_raw: %s\n", instance->TEMP_PED_NS_RAW.value ? "TRUE" : "FALSE");
    printf("temp_ped_ew_raw: %s\n", instance->TEMP_PED_EW_RAW.value ? "TRUE" : "FALSE");

    /* Outputs */
    printf("lamp_ns_green: %s\n", instance->LAMP_NS_GREEN.value ? "TRUE" : "FALSE");
    printf("lamp_ns_red: %s\n", instance->LAMP_NS_RED.value ? "TRUE" : "FALSE");
    printf("lamp_ew_green: %s\n", instance->LAMP_EW_GREEN.value ? "TRUE" : "FALSE");
    printf("lamp_ew_red: %s\n", instance->LAMP_EW_RED.value ? "TRUE" : "FALSE");
    printf("val_countdown: %ld\n", (long)instance->VAL_COUNTDOWN.value);

    /* Debug - __IEC_TIME_t has 'value' member */
    printf("[DEBUG] T_FSM.PT: %ld sec, %ld nsec\n",
           instance->T_FSM.PT.value.tv_sec, instance->T_FSM.PT.value.tv_nsec);
    printf("[DEBUG] T_FSM.ET: %ld sec, %ld nsec\n",
           instance->T_FSM.ET.value.tv_sec, instance->T_FSM.ET.value.tv_nsec);
    printf("[DEBUG] T_FSM.Q: %s\n", instance->T_FSM.Q.value ? "TRUE" : "FALSE");
    printf("[DEBUG] TEMP_REMAINSEC: %ld\n", (long)instance->TEMP_REMAINSEC.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }

    FB_TRAFFICLIGHT instance;
    FB_TRAFFICLIGHT_init__(&instance, FALSE);

    // Initialize simulated time
    __CURRENT_TIME = __time_to_timespec(1, 0, 0, 0, 0, 0);

    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;

        // Increment simulated time by scan cycle (50ms)
        // __time_to_timespec(sign, mseconds, seconds, minutes, hours, days)
        __CURRENT_TIME = __time_add(__CURRENT_TIME, __time_to_timespec(1, SCAN_CYCLE_MS, 0, 0, 0, 0));

        FB_TRAFFICLIGHT_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }

    return 0;
}