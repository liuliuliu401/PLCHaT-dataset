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

void read_inputs(FB_HEXDIGITANALYZER* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    int input_val;
    if (sscanf(line, "%d", &input_val) == 1) {
        instance->INPUTNUMBER.value = input_val;
    }
}

void print_fb_state(FB_HEXDIGITANALYZER* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("InputNumber: %d\n", (int)instance->INPUTNUMBER.value);
    printf("Thousands: %d\n", (int)instance->THOUSANDS.value);
    printf("Hundreds: %d\n", (int)instance->HUNDREDS.value);
    printf("Tens: %d\n", (int)instance->TENS.value);
    printf("Ones: %d\n", (int)instance->ONES.value);
    printf("OutMin: %d\n", (int)instance->OUTMIN.value);
    printf("error: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("status: %u\n", (unsigned int)instance->STATUS.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    FB_HEXDIGITANALYZER instance;
    FB_HEXDIGITANALYZER_init__(&instance, FALSE);
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        FB_HEXDIGITANALYZER_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}