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

void read_inputs(RINGQUEUEMULTIITEM* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse format: push pop reset itemIndex itemLen [10 BYTE items] [10 BYTE queue]
    int push_val, pop_val, reset_val;
    long itemIndex_val, itemLen_val;
    unsigned char item_bytes[10], queue_bytes[10];
    
    if (sscanf(line, "%d %d %d %ld %ld %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
               &push_val, &pop_val, &reset_val, &itemIndex_val, &itemLen_val,
               &item_bytes[0], &item_bytes[1], &item_bytes[2], &item_bytes[3], &item_bytes[4],
               &item_bytes[5], &item_bytes[6], &item_bytes[7], &item_bytes[8], &item_bytes[9],
               &queue_bytes[0], &queue_bytes[1], &queue_bytes[2], &queue_bytes[3], &queue_bytes[4],
               &queue_bytes[5], &queue_bytes[6], &queue_bytes[7], &queue_bytes[8], &queue_bytes[9]) >= 5) {
        
        // Set scalar inputs
        instance->PUSH.value = push_val;
        instance->POP.value = pop_val;
        instance->RESET.value = reset_val;
        instance->ITEMINDEX.value = itemIndex_val;
        instance->ITEMLEN.value = itemLen_val;
        
        // Set ARRAY[1..10] OF BYTE - remember 0-indexed in C
        if (sscanf(line, "%d %d %d %ld %ld", &push_val, &pop_val, &reset_val, &itemIndex_val, &itemLen_val) == 5) {
            // Extract item array bytes
            char* ptr = line;
            for (int i = 0; i < 5; i++) {
                ptr = strchr(ptr, ' ');
                if (ptr) ptr++;
            }
            
            if (ptr) {
                for (int i = 0; i < 10; i++) {
                    if (sscanf(ptr, "%hhu", &item_bytes[i]) == 1) {
                        instance->ITEM.value.table[i] = item_bytes[i];
                        ptr = strchr(ptr, ' ');
                        if (ptr) ptr++;
                    }
                }
                
                // Extract queue array bytes
                for (int i = 0; i < 10; i++) {
                    if (ptr && sscanf(ptr, "%hhu", &queue_bytes[i]) == 1) {
                        instance->QUEUE.value.table[i] = queue_bytes[i];
                        ptr = strchr(ptr, ' ');
                        if (ptr) ptr++;
                    }
                }
            }
        }
    }
}

void print_fb_state(RINGQUEUEMULTIITEM* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Inputs
    printf("Inputs: PUSH=%d POP=%d RESET=%d ITEMINDEX=%ld ITEMLEN=%ld\n",
           instance->PUSH.value, instance->POP.value, instance->RESET.value,
           (long)instance->ITEMINDEX.value, (long)instance->ITEMLEN.value);
    
    // Outputs
    printf("Outputs: QUEUEUSED=%ld QUEUEUNUSED=%ld\n",
           (long)instance->QUEUEUSED.value, (long)instance->QUEUEUNUSED.value);
    
    // Internal state
    printf("Internal: HEAD=%ld TAIL=%ld CAPACITY=%ld COUNT=%ld I=%ld\n",
           (long)instance->HEAD.value, (long)instance->TAIL.value,
           (long)instance->CAPACITY.value, (long)instance->COUNT.value,
           (long)instance->I.value);
    
    // Item array (IN_OUT)
    printf("ITEM array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", instance->ITEM.value.table[i]);
    }
    printf("\n");
    
    // Queue array (IN_OUT)
    printf("QUEUE array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", instance->QUEUE.value.table[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    RINGQUEUEMULTIITEM instance;
    RINGQUEUEMULTIITEM_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        RINGQUEUEMULTIITEM_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}