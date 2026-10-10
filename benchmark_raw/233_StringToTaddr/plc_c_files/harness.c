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

void read_inputs(STRINGTOTADDR* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input: "ip_address_string"
    char ip_string[256];
    sscanf(line, "%255s", ip_string);
    
    // Convert to MatIEC STRING type
    // Copy string content and set length
    strncpy(instance->IPADDRESSSTRING.value.body, ip_string, sizeof(instance->IPADDRESSSTRING.value.body) - 1);
    instance->IPADDRESSSTRING.value.body[sizeof(instance->IPADDRESSSTRING.value.body) - 1] = '\0';
    instance->IPADDRESSSTRING.value.len = strlen(ip_string);
    if (instance->IPADDRESSSTRING.value.len > sizeof(instance->IPADDRESSSTRING.value.body) - 1) {
        instance->IPADDRESSSTRING.value.len = sizeof(instance->IPADDRESSSTRING.value.body) - 1;
    }
}

void print_fb_state(STRINGTOTADDR* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print input
    printf("Input IPADDRESSSTRING: ");
    for (int i = 0; i < instance->IPADDRESSSTRING.value.len; i++) {
        printf("%c", instance->IPADDRESSSTRING.value.body[i]);
    }
    printf("\n");
    
    // Print outputs
    printf("ERROR: %s\n", instance->ERROR.value ? "TRUE" : "FALSE");
    printf("STATUS: 0x%04X\n", (unsigned int)instance->STATUS.value);
    printf("RETURNVALUE.IPADDR: %ld\n", (long)instance->RETURNVALUE.value.IPADDR);
    printf("RETURNVALUE.PORT: %d\n", (int)instance->RETURNVALUE.value.PORT);
    
    // Print internal array values (0-indexed in C, was 1..4 in ST)
    printf("IPPARTS array: [");
    for (int i = 0; i < 4; i++) {
        printf("%d", (int)instance->IPPARTS.value.table[i]);
        if (i < 3) printf(", ");
    }
    printf("]\n");
    
    printf("PORTNUMBER: %d\n", (int)instance->PORTNUMBER.value);
    printf("TEMPERROR: %s\n", instance->TEMPERROR.value ? "TRUE" : "FALSE");
    printf("TEMPSTATUS: 0x%04X\n", (unsigned int)instance->TEMPSTATUS.value);
    printf("I: %d\n", (int)instance->I.value);
    printf("FINDPOS: %d\n", (int)instance->FINDPOS.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    STRINGTOTADDR instance;
    STRINGTOTADDR_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        STRINGTOTADDR_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}

