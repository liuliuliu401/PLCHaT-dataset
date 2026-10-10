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

void read_inputs(ANALOGBATCHPROCESSING* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse format: COUNT ANALOGVALUES[1-10] CHANNEL_ENABLE[1-10] CHANNEL_HILIM[1-10] CHANNEL_LOLIM[1-10] 
    // CHANNEL_BIPOLAR[1-10] CHANNEL_MEASURINGMODE[1-10]
    int temp_count;
    float temp_analog[10];
    int temp_enable[10];
    float temp_hilim[10];
    float temp_lolim[10];
    int temp_bipolar[10];
    int temp_mode[10];
    
    int scanned = sscanf(line, 
        "%d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d",
        &temp_count,
        &temp_analog[0], &temp_analog[1], &temp_analog[2], &temp_analog[3], &temp_analog[4],
        &temp_analog[5], &temp_analog[6], &temp_analog[7], &temp_analog[8], &temp_analog[9],
        &temp_enable[0], &temp_enable[1], &temp_enable[2], &temp_enable[3], &temp_enable[4],
        &temp_enable[5], &temp_enable[6], &temp_enable[7], &temp_enable[8], &temp_enable[9],
        &temp_hilim[0], &temp_hilim[1], &temp_hilim[2], &temp_hilim[3], &temp_hilim[4],
        &temp_hilim[5], &temp_hilim[6], &temp_hilim[7], &temp_hilim[8], &temp_hilim[9],
        &temp_lolim[0], &temp_lolim[1], &temp_lolim[2], &temp_lolim[3], &temp_lolim[4],
        &temp_lolim[5], &temp_lolim[6], &temp_lolim[7], &temp_lolim[8], &temp_lolim[9],
        &temp_bipolar[0], &temp_bipolar[1], &temp_bipolar[2], &temp_bipolar[3], &temp_bipolar[4],
        &temp_bipolar[5], &temp_bipolar[6], &temp_bipolar[7], &temp_bipolar[8], &temp_bipolar[9],
        &temp_mode[0], &temp_mode[1], &temp_mode[2], &temp_mode[3], &temp_mode[4],
        &temp_mode[5], &temp_mode[6], &temp_mode[7], &temp_mode[8], &temp_mode[9]
    );
    
    if (scanned >= 1) {
        instance->COUNT.value = temp_count;
    }
    
    if (scanned >= 11) {
        for (int i = 0; i < 10; i++) {
            instance->ANALOGVALUES.value.table[i] = temp_analog[i];
        }
    }
    
    if (scanned >= 21) {
        for (int i = 0; i < 10; i++) {
            instance->CHANNEL_ENABLE.value.table[i] = temp_enable[i] != 0;
        }
    }
    
    if (scanned >= 31) {
        for (int i = 0; i < 10; i++) {
            instance->CHANNEL_HILIM.value.table[i] = temp_hilim[i];
        }
    }
    
    if (scanned >= 41) {
        for (int i = 0; i < 10; i++) {
            instance->CHANNEL_LOLIM.value.table[i] = temp_lolim[i];
        }
    }
    
    if (scanned >= 51) {
        for (int i = 0; i < 10; i++) {
            instance->CHANNEL_BIPOLAR.value.table[i] = temp_bipolar[i] != 0;
        }
    }
    
    if (scanned >= 61) {
        for (int i = 0; i < 10; i++) {
            instance->CHANNEL_MEASURINGMODE.value.table[i] = temp_mode[i];
        }
    }
}

void print_fb_state(ANALOGBATCHPROCESSING* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("COUNT: %d\n", (int)instance->COUNT.value);
    
    printf("ANALOGVALUES: ");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", instance->ANALOGVALUES.value.table[i]);
    }
    printf("\n");
    
    printf("CHANNEL_ENABLE: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", instance->CHANNEL_ENABLE.value.table[i] ? 1 : 0);
    }
    printf("\n");
    
    printf("CHANNEL_HILIM: ");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", instance->CHANNEL_HILIM.value.table[i]);
    }
    printf("\n");
    
    printf("CHANNEL_LOLIM: ");
    for (int i = 0; i < 10; i++) {
        printf("%.2f ", instance->CHANNEL_LOLIM.value.table[i]);
    }
    printf("\n");
    
    printf("CHANNEL_BIPOLAR: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", instance->CHANNEL_BIPOLAR.value.table[i] ? 1 : 0);
    }
    printf("\n");
    
    printf("CHANNEL_MEASURINGMODE: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", (int)instance->CHANNEL_MEASURINGMODE.value.table[i]);
    }
    printf("\n");
    
    printf("CHANNEL_RETVAL: ");
    for (int i = 0; i < 10; i++) {
        printf("0x%04X ", (unsigned int)instance->CHANNEL_RETVAL.value.table[i]);
    }
    printf("\n");
    
    printf("CHANNEL_OUTPUTVALUE: ");
    for (int i = 0; i < 10; i++) {
        printf("%.6f ", instance->CHANNEL_OUTPUTVALUE.value.table[i]);
    }
    printf("\n");
    
    printf("I: %d\n", (int)instance->I.value);
    printf("LOOP_END: %d\n", (int)instance->LOOP_END.value);
    printf("C_MAX_ADC_UNIPOLAR: %.2f\n", instance->C_MAX_ADC_UNIPOLAR.value);
    printf("C_MAX_ADC_BIPOLAR_POS: %.2f\n", instance->C_MAX_ADC_BIPOLAR_POS.value);
    printf("C_MAX_ADC_BIPOLAR_NEG: %.2f\n", instance->C_MAX_ADC_BIPOLAR_NEG.value);
    printf("C_BIPOLAR_RANGE: %.2f\n", instance->C_BIPOLAR_RANGE.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    ANALOGBATCHPROCESSING instance;
    ANALOGBATCHPROCESSING_init__(&instance, FALSE);
    
    // Set internal constant values as per ST code
    instance.C_MAX_ADC_UNIPOLAR.value = 27648.0;
    instance.C_MAX_ADC_BIPOLAR_POS.value = 27648.0;
    instance.C_MAX_ADC_BIPOLAR_NEG.value = -27648.0;
    instance.C_BIPOLAR_RANGE.value = 55296.0;
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        ANALOGBATCHPROCESSING_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}