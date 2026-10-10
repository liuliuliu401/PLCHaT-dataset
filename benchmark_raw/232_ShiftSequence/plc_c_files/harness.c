#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include "iec_types_all.h"
#include "POUS.h"

TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;
double scan_cycle_ms = 50.0;

void emergency_exit_handler(int sig) {
    if (sig == SIGALRM) {
        fprintf(stderr, "[WATCHDOG] Terminating! Single cycle execution exceeded scan_cycle_ms limit.\n");
    }
    fflush(stdout);
    exit(0);
}

void read_inputs(SHIFTSEQUENCE* instance) {
    char line[1024];
    printf("[DEBUG] Waiting for input (fgets)...\n"); fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("[DEBUG] EOF reached. Terminating.\n"); fflush(stdout);
        exit(0);
    }
    printf("[DEBUG] Read line: %s", line); fflush(stdout);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Variables to temporarily hold parsed input values
    int shiftLeft_val, shiftRight_val, shiftRange_val, clear_val;
    long long initialItem_val;
    long long buffer_temp[10];
    
    // Parse input line according to the variable order in ST code
    // Format: shiftLeft shiftRight shiftRange clear initialItem buffer[1] buffer[2] ... buffer[10]
    int parsed = sscanf(line, 
        "%d %d %d %d %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld",
        &shiftLeft_val, &shiftRight_val, &shiftRange_val, &clear_val,
        &initialItem_val,
        &buffer_temp[0], &buffer_temp[1], &buffer_temp[2], &buffer_temp[3], &buffer_temp[4],
        &buffer_temp[5], &buffer_temp[6], &buffer_temp[7], &buffer_temp[8], &buffer_temp[9]);
    
    // Only assign if we got at least the first 5 values
    if (parsed >= 5) {
        instance->SHIFTLEFT.value = shiftLeft_val;
        instance->SHIFTRIGHT.value = shiftRight_val;
        instance->SHIFTRANGE.value = shiftRange_val;
        printf("[DEBUG] SHIFTRANGE: %d\n", shiftRange_val); fflush(stdout);
        instance->CLEAR.value = clear_val;
        instance->INITIALITEM.value = initialItem_val;
        
        // Assign buffer values if they were provided
        if (parsed >= 15) {
            for (int i = 0; i < 10; i++) {
                instance->BUFFER.value.table[i] = buffer_temp[i];
            }
        }
    }
}

void print_fb_state(SHIFTSEQUENCE* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Inputs:\n");
    printf("  SHIFTLEFT: %s\n", instance->SHIFTLEFT.value ? "TRUE" : "FALSE");
    printf("  SHIFTRIGHT: %s\n", instance->SHIFTRIGHT.value ? "TRUE" : "FALSE");
    printf("  SHIFTRANGE: %d\n", instance->SHIFTRANGE.value);
    printf("  CLEAR: %s\n", instance->CLEAR.value ? "TRUE" : "FALSE");
    printf("  INITIALITEM: %lld\n", (long long)instance->INITIALITEM.value);
    
    printf("\nBuffer (index 1..10 in ST, 0..9 in C):\n");
    for (int i = 0; i < 10; i++) {
        printf("  BUFFER[%d]: %lld\n", i + 1, (long long)instance->BUFFER.value.table[i]);
    }
    
    printf("\nInternal variables:\n");
    printf("  I: %d\n", instance->I.value);
    printf("  J: %d\n", instance->J.value);
    printf("  K: %d\n", instance->K.value);
    printf("  TEMP: %lld\n", (long long)instance->TEMP.value);
}

int main(int argc, char** argv) {
    signal(SIGTERM, emergency_exit_handler);
    signal(SIGINT, emergency_exit_handler);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    SHIFTSEQUENCE instance;
    SHIFTSEQUENCE_init__(&instance, FALSE);
    
    int cycle = 0;
    
    // Initial print to show starting state
    if (!silent_mode) {
        printf("Starting ShiftSequence Function Block\n");
        printf("Input format: SHIFTLEFT SHIFTRIGHT SHIFTRANGE CLEAR INITIALITEM");
        printf(" BUFFER1 BUFFER2 BUFFER3 BUFFER4 BUFFER5 BUFFER6 BUFFER7 BUFFER8 BUFFER9 BUFFER10\n");
        printf("Example: 1 0 3 0 99 1 2 3 4 5 6 7 8 9 10\n");
        printf("Use # for comments or blank lines to continue with same inputs\n");
        print_fb_state(&instance, cycle);
    }
    
    while (1) {
        printf("[DEBUG] --- Cycle %d: Start ---\n", cycle + 1); fflush(stdout);
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        // --- 开启异步看门狗 ---
        struct itimerval timer;
        timer.it_value.tv_sec = (long)(scan_cycle_ms / 1000);
        timer.it_value.tv_usec = (long)(((long)scan_cycle_ms % 1000) * 1000);
        timer.it_interval.tv_sec = 0; // 不循环触发
        timer.it_interval.tv_usec = 0;
        
        signal(SIGALRM, emergency_exit_handler); // 确保捕获信号
        setitimer(ITIMER_REAL, &timer, NULL); 

        printf("[DEBUG] Entering body__ (Watchdog ACTIVE)\n"); fflush(stdout);
        
        // 如果这里死循环，信号会在 scan_cycle_ms 后准时达到
        // 触发 emergency_exit_handler -> exit(0) -> 保存覆盖率
        SHIFTSEQUENCE_body__(&instance); 

        // --- 执行成功，关闭看门狗 ---
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        
        printf("[DEBUG] Exited body__ (Watchdog CLEARED)\n"); fflush(stdout);

        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    printf("[DEBUG] Going to return value\n");
    return 0;
}

