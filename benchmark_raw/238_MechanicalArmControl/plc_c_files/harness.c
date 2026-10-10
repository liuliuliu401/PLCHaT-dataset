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

void read_inputs(MECHANICALARM* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    int temp_vals[18];
    // Format: 18 BOOL inputs as integers (0 or 1)
    // Order: MANUALMODE SINGLESTEPMODE SINGLECYCLEMODE CONTINUOUSMODE EMERGENCYSTOP START
    //        DOWNBUTTON UPBUTTON LEFTBUTTON RIGHTBUTTON GRIPBUTTON RELEASEBUTTON
    //        DOWNLIMITSWITCH UPLIMITSWITCH LEFTLIMITSWITCH RIGHTLIMITSWITCH
    //        GRIPLIMITSWITCH RELEASELIMITSWITCH
    if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
               &temp_vals[0], &temp_vals[1], &temp_vals[2], &temp_vals[3],
               &temp_vals[4], &temp_vals[5], &temp_vals[6], &temp_vals[7],
               &temp_vals[8], &temp_vals[9], &temp_vals[10], &temp_vals[11],
               &temp_vals[12], &temp_vals[13], &temp_vals[14], &temp_vals[15],
               &temp_vals[16], &temp_vals[17]) >= 18) {
        instance->MANUALMODE.value = temp_vals[0];
        instance->SINGLESTEPMODE.value = temp_vals[1];
        instance->SINGLECYCLEMODE.value = temp_vals[2];
        instance->CONTINUOUSMODE.value = temp_vals[3];
        instance->EMERGENCYSTOP.value = temp_vals[4];
        instance->START.value = temp_vals[5];
        instance->DOWNBUTTON.value = temp_vals[6];
        instance->UPBUTTON.value = temp_vals[7];
        instance->LEFTBUTTON.value = temp_vals[8];
        instance->RIGHTBUTTON.value = temp_vals[9];
        instance->GRIPBUTTON.value = temp_vals[10];
        instance->RELEASEBUTTON.value = temp_vals[11];
        instance->DOWNLIMITSWITCH.value = temp_vals[12];
        instance->UPLIMITSWITCH.value = temp_vals[13];
        instance->LEFTLIMITSWITCH.value = temp_vals[14];
        instance->RIGHTLIMITSWITCH.value = temp_vals[15];
        instance->GRIPLIMITSWITCH.value = temp_vals[16];
        instance->RELEASELIMITSWITCH.value = temp_vals[17];
    }
}

void print_fb_state(MECHANICALARM* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    printf("INPUTS:\n");
    printf("  ManualMode: %d  SingleStepMode: %d  SingleCycleMode: %d  ContinuousMode: %d\n",
           instance->MANUALMODE.value, instance->SINGLESTEPMODE.value,
           instance->SINGLECYCLEMODE.value, instance->CONTINUOUSMODE.value);
    printf("  EmergencyStop: %d  Start: %d\n", instance->EMERGENCYSTOP.value, instance->START.value);
    printf("  Buttons: Down:%d Up:%d Left:%d Right:%d Grip:%d Release:%d\n",
           instance->DOWNBUTTON.value, instance->UPBUTTON.value,
           instance->LEFTBUTTON.value, instance->RIGHTBUTTON.value,
           instance->GRIPBUTTON.value, instance->RELEASEBUTTON.value);
    printf("  Limits: Down:%d Up:%d Left:%d Right:%d Grip:%d Release:%d\n",
           instance->DOWNLIMITSWITCH.value, instance->UPLIMITSWITCH.value,
           instance->LEFTLIMITSWITCH.value, instance->RIGHTLIMITSWITCH.value,
           instance->GRIPLIMITSWITCH.value, instance->RELEASELIMITSWITCH.value);
    printf("OUTPUTS:\n");
    printf("  MoveDown: %d  MoveUp: %d  MoveLeft: %d  MoveRight: %d  ActivateGrip: %d  ActivateRelease: %d\n",
           instance->MOVEDOWN.value, instance->MOVEUP.value,
           instance->MOVELEFT.value, instance->MOVERIGHT.value,
           instance->ACTIVATEGRIP.value, instance->ACTIVATERELEASE.value);
    printf("INTERNAL STATE:\n");
    printf("  CurrentStep: %d  CycleCompleted: %d  PreviousStart: %d\n",
           instance->CURRENTSTEP.value, instance->CYCLECOMPLETED.value,
           instance->PREVIOUSSTART.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    MECHANICALARM instance;
    MECHANICALARM_init__(&instance, FALSE);
    
    printf("MECHANICAL ARM TEST HARNESS\n");
    printf("Input format (18 BOOLs as 0/1):\n");
    printf("  ManualMode SingleStepMode SingleCycleMode ContinuousMode EmergencyStop Start\n");
    printf("  DownButton UpButton LeftButton RightButton GripButton ReleaseButton\n");
    printf("  DownLimitSwitch UpLimitSwitch LeftLimitSwitch RightLimitSwitch GripLimitSwitch ReleaseLimitSwitch\n");
    printf("Example: 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
    printf("Ctrl+C to exit\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        MECHANICALARM_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    return 0;
}