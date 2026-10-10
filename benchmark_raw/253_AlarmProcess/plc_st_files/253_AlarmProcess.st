FUNCTION_BLOCK AlarmProcess
VAR_INPUT
    Value : REAL; (* Input value *)
    hiLevel : REAL; (* High limit alarm value *)
    loLevel : REAL; (* Low limit alarm value *)
    hystWindow : REAL; (* Hysteresis window *)
END_VAR
VAR_OUTPUT
    hiAlarm : BOOL; (* High limit alarm *)
    loAlarm : BOOL; (* Low limit alarm *)
    error : BOOL; (* Error flag *)
    status : WORD; (* Status code *)
END_VAR
VAR
    tempStatus : WORD;
END_VAR

error := FALSE;
tempStatus := 0;

IF loLevel >= hiLevel THEN
    error := TRUE;
    tempStatus := 16#8001;
ELSIF hystWindow >= (hiLevel - loLevel) THEN
    error := TRUE;
    tempStatus := 16#8002;
ELSE
    IF Value > hiLevel THEN
        hiAlarm := TRUE;
    ELSIF Value < (hiLevel - hystWindow) THEN
        hiAlarm := FALSE;
    END_IF;
    
    IF Value < loLevel THEN
        loAlarm := TRUE;
    ELSIF Value > (loLevel + hystWindow) THEN
        loAlarm := FALSE;
    END_IF;
END_IF;

status := tempStatus;

END_FUNCTION_BLOCK