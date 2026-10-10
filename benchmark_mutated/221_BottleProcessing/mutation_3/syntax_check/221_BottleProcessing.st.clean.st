FUNCTION_BLOCK FB_BottleProcessing
VAR_INPUT
    bottleSensor : BOOL; 
    cleaningConfirmButton : BOOL; 
    fillingConfirmButton : BOOL; 
    cappingConfirmButton : BOOL; 
    packingConfirmButton : BOOL; 
    finishedButton : BOOL; 
END_VAR
VAR_OUTPUT
    Pump_Motor : BOOL; 
    Filling_Valve : BOOL; 
    Capping_Machine : BOOL; 
    Packing_Machine : BOOL; 
    Completion_Light : BOOL; 
END_VAR
VAR
    cleaningStep : BOOL := FALSE;
    fillingStep : BOOL := FALSE;
    cappingStep : BOOL := FALSE;
    packingStep : BOOL := FALSE;
    completionStep : BOOL := FALSE;
END_VAR

IF bottleSensor THEN
    cleaningStep := TRUE;
    Pump_Motor := TRUE;
    Filling_Valve := FALSE;
    Capping_Machine := FALSE;
    Packing_Machine := FALSE;
    Completion_Light := FALSE;
    cappingStep := FALSE; 
END_IF;

IF cleaningConfirmButton AND cleaningStep THEN
    cleaningStep := FALSE;
    fillingStep := TRUE;
    Pump_Motor := FALSE;
    Filling_Valve := TRUE;
    Capping_Machine := FALSE;
    Packing_Machine := FALSE;
    Completion_Light := FALSE;
END_IF;

IF fillingConfirmButton AND fillingStep THEN
    fillingStep := FALSE;
    cappingStep := TRUE;
    Pump_Motor := FALSE;
    Filling_Valve := FALSE;
    Capping_Machine := TRUE;
    Packing_Machine := FALSE;
    Completion_Light := FALSE;
END_IF;

IF cappingConfirmButton AND cappingStep THEN
    cappingStep := FALSE;
    packingStep := TRUE;
    Pump_Motor := FALSE;
    Filling_Valve := FALSE;
    Capping_Machine := FALSE;
    Packing_Machine := TRUE;
    Completion_Light := FALSE;
END_IF;

IF packingConfirmButton AND packingStep THEN
    packingStep := FALSE;
    completionStep := TRUE;
    Pump_Motor := FALSE;
    Filling_Valve := FALSE;
    Capping_Machine := FALSE;
    Packing_Machine := FALSE;
    Completion_Light := TRUE;
END_IF;

IF finishedButton AND completionStep THEN
    cleaningStep := FALSE;
    fillingStep := FALSE;
    cappingStep := FALSE;
    packingStep := FALSE;
    completionStep := FALSE;
    Pump_Motor := FALSE;
    Filling_Valve := FALSE;
    Capping_Machine := FALSE;
    Packing_Machine := FALSE;
    Completion_Light := FALSE;
END_IF;

END_FUNCTION_BLOCK