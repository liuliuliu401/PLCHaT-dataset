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

// 辅助函数：将 C 字符串安全填充到 MatIEC STRING 结构体中
void set_iec_string(STRING* iec_str, const char* c_str) {
    size_t len = strlen(c_str);
    if (len > STR_MAX_LEN) len = STR_MAX_LEN;
    iec_str->len = (uint8_t)len;
    memcpy(iec_str->body, c_str, len);
    // 确保处理可能存在的旧数据
    if (len < STR_MAX_LEN) iec_str->body[len] = '\0';
}

void read_inputs(EXTRACTSUBARRAY* instance) {
    char line[2048];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;

    char s1[256], s2[256], s3[256];
    // 假设输入格式改为空格或逗号分隔的三个字符串
    if (sscanf(line, "%s %s %s", s1, s2, s3) >= 3) {
        set_iec_string(&instance->TEXTBEFORE.value, s1);
        set_iec_string(&instance->TEXTAFTER.value, s2);
        set_iec_string(&instance->SEARCHIN.value, s3);
    }
}

void print_fb_state(EXTRACTSUBARRAY* instance, int cycle) {
    if (silent_mode) return;
    
    printf("\n--- Cycle %d ---\n", cycle);
    
    // MatIEC STRING 的 body 本质上就是 char 数组，但需要根据 .len 限制打印
    printf("TEXTBEFORE: %.*s\n", instance->TEXTBEFORE.value.len, instance->TEXTBEFORE.value.body);
    printf("TEXTAFTER: %.*s\n", instance->TEXTAFTER.value.len, instance->TEXTAFTER.value.body);
    printf("SEARCHIN: %.*s\n", instance->SEARCHIN.value.len, instance->SEARCHIN.value.body);
    printf("EXTRACTEDSTRING: %.*s\n", instance->EXTRACTEDSTRING.value.len, instance->EXTRACTEDSTRING.value.body);
    
    printf("STATUS: 0x%04X\n", instance->STATUS.value);
    printf("LENSEARCHIN: %d\n", instance->LENSEARCHIN.value);
    printf("LENTEXTBEFORE: %d\n", instance->LENTEXTBEFORE.value);
    printf("LENTEXTAFTER: %d\n", instance->LENTEXTAFTER.value);
    printf("STARTINDEX: %d\n", instance->STARTINDEX.value);
    printf("ENDINDEX: %d\n", instance->ENDINDEX.value);
    printf("FOUNDTEXTBEFORE: %s\n", instance->FOUNDTEXTBEFORE.value ? "TRUE" : "FALSE");
    printf("FOUNDTEXTAFTER: %s\n", instance->FOUNDTEXTAFTER.value ? "TRUE" : "FALSE");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    EXTRACTSUBARRAY instance;
    EXTRACTSUBARRAY_init__(&instance, FALSE);
    
    // 初始化 STRING 结构体
    instance.TEXTBEFORE.value.len = 0;
    instance.TEXTAFTER.value.len = 0;
    instance.SEARCHIN.value.len = 0;
    instance.EXTRACTEDSTRING.value.len = 0;
    
    instance.EN.value = 1;
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        EXTRACTSUBARRAY_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}