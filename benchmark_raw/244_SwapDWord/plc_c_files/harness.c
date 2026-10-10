#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include "iec_types_all.h"
#include "POUS.h"

TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;
double scan_cycle_ms = 50.0;

void emergency_exit_handler(int sig) {
    if (sig == SIGALRM) {
        fprintf(stderr, "[WATCHDOG] Terminating! FB_SWAPDWORD_body__ exceeded scan_cycle_ms.\n");
    }
    fflush(stdout);
    exit(0);
}

void read_inputs(FB_SWAPDWORD* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Parse input array dataIn[1..10] (C indices 0..9)
    long temp_vals[10];
    int parsed = sscanf(line, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                       &temp_vals[0], &temp_vals[1], &temp_vals[2],
                       &temp_vals[3], &temp_vals[4], &temp_vals[5],
                       &temp_vals[6], &temp_vals[7], &temp_vals[8],
                       &temp_vals[9]);
    
    printf("[DEBUG] sscanf parsed %d items\n", parsed); fflush(stdout);
    if (parsed == 10) {
        for (int i = 0; i < 10; i++) {
            instance->DATAIN.value.table[i] = temp_vals[i];
        }
    } else {
        printf("[DEBUG] Warning: Expected 10 values, got %d. Cycle might skip logic.\n", parsed); fflush(stdout);
    }
}

void print_fb_state(FB_SWAPDWORD* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print inputs
    printf("DATAIN:");
    for (int i = 0; i < 10; i++) {
        printf(" %ld", (long)instance->DATAIN.value.table[i]);
    }
    printf("\n");
    
    // Print outputs
    printf("SWAPNUM: %d\n", instance->SWAPNUM.value);
    
    printf("DATAOUT:");
    for (int i = 0; i < 10; i++) {
        printf(" %ld", (long)instance->DATAOUT.value.table[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    signal(SIGTERM, emergency_exit_handler);
    signal(SIGINT, emergency_exit_handler);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
        if (strcmp(argv[i], "--cycle") == 0 && i + 1 < argc) {
            scan_cycle_ms = atof(argv[++i]);
        }
    }
    
    FB_SWAPDWORD instance;
    FB_SWAPDWORD_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        printf("[DEBUG] --- Cycle %d: Start ---\n", cycle + 1); fflush(stdout);
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        // --- 启动 PLC 扫描周期异步看门狗 ---
        struct itimerval timer;
        timer.it_value.tv_sec = (long)(scan_cycle_ms / 1000);
        timer.it_value.tv_usec = (long)(((long)scan_cycle_ms % 1000) * 1000);
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        
        signal(SIGALRM, emergency_exit_handler);
        setitimer(ITIMER_REAL, &timer, NULL); 

        printf("[DEBUG] Entering FB_SWAPDWORD_body__ (Watchdog: %.2fms)\n", scan_cycle_ms); fflush(stdout);
        
        // 若此处因 I=0 导致死循环，内核将在 scan_cycle_ms 后准时发送 SIGALRM
        FB_SWAPDWORD_body__(&instance);

        // --- 正常完成，关闭定时器 ---
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        
        printf("[DEBUG] Exited FB_SWAPDWORD_body__\n"); fflush(stdout);

        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    printf("[DEBUG] Going to return value\n");
    return 0;
}

