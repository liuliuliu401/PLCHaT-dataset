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


void read_inputs(MATRIXMULTIPLICATION* instance) {
    char line[4096]; // 扩展缓冲区以容纳 201 个浮点数（约 2000+ 字符）
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;

    // 使用指针维护当前的解析位置
    char* ptr = line;
    int offset;

    // 1. 解析 EXECUTE (BOOL) - 对应 Schema 第 0 列
    int execute_val;
    if (sscanf(ptr, "%d%n", &execute_val, &offset) == 1) {
        instance->EXECUTE.value = (execute_val != 0);
        ptr += offset;
    } else {
        return;
    }

    // 2. 解析 MATRIX1 (10x10 REAL) - 对应 Schema 第 1-100 列
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            float val;
            if (sscanf(ptr, "%f%n", &val, &offset) == 1) {
                instance->MATRIX1.value.table[i][j] = val;
                ptr += offset;
            } else {
                return; // 数据不足
            }
        }
    }

    // 3. 解析 MATRIX2 (10x10 REAL) - 对应 Schema 第 101-200 列
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            float val;
            if (sscanf(ptr, "%f%n", &val, &offset) == 1) {
                instance->MATRIX2.value.table[i][j] = val;
                ptr += offset;
            } else {
                return; // 数据不足
            }
        }
    }
}

void print_fb_state(MATRIXMULTIPLICATION* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    printf("EXECUTE: %d\n", (int)instance->EXECUTE.value);
    
    printf("MATRIX1 (first 3x3):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.3f ", instance->MATRIX1.value.table[i][j]);
        }
        printf("\n");
    }
    
    printf("MATRIX2 (first 3x3):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.3f ", instance->MATRIX2.value.table[i][j]);
        }
        printf("\n");
    }
    
    printf("MATRIXRESULT (first 3x3):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.3f ", instance->MATRIXRESULT.value.table[i][j]);
        }
        printf("\n");
    }
    
    printf("Internal counters: I=%d J=%d K=%d\n", 
           (int)instance->I.value, 
           (int)instance->J.value, 
           (int)instance->K.value);
    printf("TEMP: %.3f\n", instance->TEMP.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    MATRIXMULTIPLICATION instance;
    MATRIXMULTIPLICATION_init__(&instance, FALSE);
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        MATRIXMULTIPLICATION_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}