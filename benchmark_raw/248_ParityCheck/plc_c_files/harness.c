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

// 修复点 1：直接对标量指针进行赋值
void read_inputs(LWORD* value, BOOL* odd) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    unsigned long long temp_value = 0;
    int temp_odd = 0;
    
    if (sscanf(line, "%llx %d", &temp_value, &temp_odd) == 2 ||
        sscanf(line, "%llu %d", &temp_value, &temp_odd) == 2) {
        *value = (LWORD)temp_value;  // 移除 .value
        *odd = temp_odd ? 1 : 0;     // 移除 .value
    }
}

// 修复点 2：直接打印标量值
void print_fb_state(BOOL parity_result, USINT countBitsFalse, USINT countBitsTrue, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("Parity Check Result: %s\n", parity_result ? "TRUE" : "FALSE"); // 移除 .value
    printf("Count Bits False: %u\n", (unsigned int)countBitsFalse);        // 移除 .value
    printf("Count Bits True: %u\n", (unsigned int)countBitsTrue);          // 移除 .value
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    int cycle = 0;
    
    // 变量声明保持不变，它们在 C 中是 scalar 类型
    BOOL EN;
    BOOL ENO;
    LWORD VALUE;
    BOOL ODD;
    USINT COUNTBITSFALSE;
    USINT COUNTBITSTRUE;
    
    // 修复点 3：初始化时直接赋值
    EN = 1;
    ENO = 0;
    VALUE = 0;
    ODD = 0;
    COUNTBITSFALSE = 0;
    COUNTBITSTRUE = 0;
    
    while (1) {
        read_inputs(&VALUE, &ODD);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        // 注意：__CURRENT_TIME 是否有成员取决于具体定义，参考 Case 227
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        // 调用函数
        BOOL parity_result = PARITYCHECK(
            EN, 
            &ENO, 
            VALUE, 
            ODD, 
            &COUNTBITSFALSE, 
            &COUNTBITSTRUE
        );
        
        print_fb_state(parity_result, COUNTBITSFALSE, COUNTBITSTRUE, cycle);
        fflush(stdout);
    }
    
    return 0;
}