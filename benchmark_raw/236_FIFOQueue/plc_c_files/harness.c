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

void read_inputs(FIFO* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input format: enqueue dequeue reset clear initialItem item buffer0 buffer1 ... buffer9
    // All LWORD values as %lu (unsigned long)
    int enqueue_int, dequeue_int, reset_int, clear_int;
    unsigned long initialItem_val, item_val;
    unsigned long buffer_vals[10];
    
    int result = sscanf(line, "%d %d %d %d %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
        &enqueue_int, &dequeue_int, &reset_int, &clear_int,
        &initialItem_val, &item_val,
        &buffer_vals[0], &buffer_vals[1], &buffer_vals[2], &buffer_vals[3], &buffer_vals[4],
        &buffer_vals[5], &buffer_vals[6], &buffer_vals[7], &buffer_vals[8], &buffer_vals[9]);
    
    if (result >= 5) {
        // Set BOOL inputs
        instance->ENQUEUE.value = (BOOL)(enqueue_int != 0);
        instance->DEQUEUE.value = (BOOL)(dequeue_int != 0);
        instance->RESET.value = (BOOL)(reset_int != 0);
        instance->CLEAR.value = (BOOL)(clear_int != 0);
        instance->INITIALITEM.value = initialItem_val;
        
        // Only set ITEM if we have at least 6 values
        if (result >= 6) {
            instance->ITEM.value = item_val;
        }
        
        // Set BUFFER array if we have all values
        if (result >= 16) {
            for (int i = 0; i < 10; i++) {
                instance->BUFFER.value.table[i] = buffer_vals[i];
            }
        }
    }
}

void print_fb_state(FIFO* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs: ENQUEUE=%d DEQUEUE=%d RESET=%d CLEAR=%d INITIALITEM=%lu ITEM=%lu\n",
        (int)instance->ENQUEUE.value, 
        (int)instance->DEQUEUE.value,
        (int)instance->RESET.value,
        (int)instance->CLEAR.value,
        (unsigned long)instance->INITIALITEM.value,
        (unsigned long)instance->ITEM.value);
    
    printf("Buffer: [");
    for (int i = 0; i < 10; i++) {
        printf("%lu", (unsigned long)instance->BUFFER.value.table[i]);
        if (i < 9) printf(" ");
    }
    printf("]\n");
    
    printf("Outputs: ERROR=%d STATUS=%u ELEMENTCOUNT=%ld ISEMPTY=%d\n",
        (int)instance->ERROR.value,
        (unsigned int)instance->STATUS.value,
        (long)instance->ELEMENTCOUNT.value,
        (int)instance->ISEMPTY.value);
    
    printf("Internal: HEAD=%ld TAIL=%ld CAPACITY=%ld I=%ld\n",
        (long)instance->HEAD.value,
        (long)instance->TAIL.value,
        (long)instance->CAPACITY.value,
        (long)instance->I.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FIFO instance;
    FIFO_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FIFO_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}
