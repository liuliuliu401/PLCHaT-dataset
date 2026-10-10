FUNCTION_BLOCK MechanicalArm
VAR_INPUT
    ManualMode : BOOL;
    SingleStepMode : BOOL;
    SingleCycleMode : BOOL;
    ContinuousMode : BOOL;
    EmergencyStop : BOOL;
    Start : BOOL;
    DownButton : BOOL;
    UpButton : BOOL;
    LeftButton : BOOL;
    RightButton : BOOL;
    GripButton : BOOL;
    ReleaseButton : BOOL;
    DownLimitSwitch : BOOL;
    UpLimitSwitch : BOOL;
    LeftLimitSwitch : BOOL;
    RightLimitSwitch : BOOL;
    GripLimitSwitch : BOOL;
    ReleaseLimitSwitch : BOOL;
END_VAR
VAR_OUTPUT
    MoveDown : BOOL;
    MoveUp : BOOL;
    MoveLeft : BOOL;
    MoveRight : BOOL;
    ActivateGrip : BOOL;
    ActivateRelease : BOOL;
END_VAR
VAR
    CurrentStep : INT := 0;
    CycleCompleted : BOOL := FALSE;
    PreviousStart : BOOL := FALSE;
END_VAR

IF NOT EmergencyStop THEN
    IF ManualMode THEN
        MoveDown := DownButton AND NOT DownLimitSwitch;
        MoveUp := UpButton AND NOT UpLimitSwitch;
        MoveLeft := LeftButton AND NOT LeftLimitSwitch;
        MoveRight := RightButton AND NOT RightLimitSwitch;
        ActivateGrip := GripButton AND NOT GripLimitSwitch;
        ActivateRelease := ReleaseButton AND NOT ReleaseLimitSwitch;
    
    ELSIF SingleStepMode THEN
        IF Start AND NOT PreviousStart THEN
            CASE CurrentStep OF
                0: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                1: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := TRUE; ActivateRelease := FALSE;
                2: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                3: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := TRUE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                4: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                5: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := TRUE;
                6: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                7: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := TRUE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
            END_CASE;
            CurrentStep := CurrentStep + 1;
            IF CurrentStep = 8 THEN
                CurrentStep := 0;
            END_IF;
        END_IF;
        PreviousStart := Start;

    ELSIF SingleCycleMode THEN
        IF Start AND NOT PreviousStart THEN
            CASE CurrentStep OF
                0: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                1: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := TRUE; ActivateRelease := FALSE;
                2: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                3: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := TRUE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                4: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                5: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := TRUE;
                6: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                7: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := TRUE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
            END_CASE;
            CurrentStep := CurrentStep + 1;
            IF CurrentStep = 8 THEN
                CurrentStep := 0;
                CycleCompleted := TRUE;
            END_IF;
        END_IF;
        PreviousStart := Start;

    ELSIF ContinuousMode THEN
        IF Start THEN
            CASE CurrentStep OF
                0: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                1: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := TRUE; ActivateRelease := FALSE;
                2: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                3: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := TRUE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                4: MoveDown := TRUE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                5: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := TRUE;
                6: MoveDown := FALSE; MoveUp := TRUE; MoveLeft := FALSE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
                7: MoveDown := FALSE; MoveUp := FALSE; MoveLeft := TRUE; MoveRight := FALSE; ActivateGrip := FALSE; ActivateRelease := FALSE;
            END_CASE;
            CurrentStep := CurrentStep + 1;
            IF CurrentStep = 8 THEN
                CurrentStep := 0;
            END_IF;
        END_IF;
    END_IF;
ELSE
    MoveDown := FALSE;
    MoveUp := FALSE;
    MoveLeft := FALSE;
    MoveRight := FALSE;
    ActivateGrip := FALSE;
    ActivateRelease := FALSE;
END_IF;

END_FUNCTION_BLOCK