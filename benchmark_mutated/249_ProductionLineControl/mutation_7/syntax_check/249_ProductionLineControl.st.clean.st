FUNCTION_BLOCK FB_ProductionLineControl
VAR_INPUT
    Mode : BOOL; 
    ForwardButton : BOOL; 
    ReverseButton : BOOL; 
    SensorA : BOOL; 
    SensorB : BOOL; 
    SensorC : BOOL; 
    ButtonA : BOOL; 
    ButtonB : BOOL; 
    ButtonC : BOOL; 
END_VAR
VAR_OUTPUT
    MotorForward : BOOL; 
    MotorReverse : BOOL; 
    CompletionLight : BOOL; 
END_VAR
VAR
    LastSensorA : BOOL := FALSE; 
    LastSensorB : BOOL := FALSE; 
    LastSensorC : BOOL := FALSE; 
    LastButtonA : BOOL := FALSE; 
    LastButtonB : BOOL := FALSE; 
    LastButtonC : BOOL := FALSE; 
END_VAR

IF Mode THEN
    
    IF SensorA AND NOT LastSensorA THEN
        MotorForward := TRUE;
        MotorReverse := FALSE;
    ELSIF SensorB AND NOT LastSensorB THEN
        MotorForward := FALSE;
        MotorReverse := FALSE;
    ELSIF SensorC AND NOT LastSensorC THEN
        MotorForward := FALSE;
        MotorReverse := FALSE;
        CompletionLight := TRUE;
    END_IF;
    IF ButtonA AND NOT LastButtonA THEN
        MotorForward := TRUE;
    ELSIF ButtonB AND NOT LastButtonB THEN
        MotorForward := TRUE;
    ELSIF ButtonC AND NOT LastButtonC THEN
        MotorForward := FALSE;
        CompletionLight := CompletionLight;
    END_IF;
ELSE
    
    IF ForwardButton AND NOT ReverseButton THEN
        MotorForward := TRUE;
        MotorReverse := FALSE;
    ELSIF ReverseButton AND NOT ForwardButton THEN
        MotorForward := FALSE;
        MotorReverse := TRUE;
    ELSE
        MotorForward := FALSE;
        MotorReverse := FALSE;
    END_IF;
    CompletionLight := FALSE;
END_IF;


LastSensorA := SensorA;
LastSensorB := SensorB;
LastSensorC := SensorC;
LastButtonA := ButtonA;
LastButtonB := ButtonB;
LastButtonC := ButtonC;

END_FUNCTION_BLOCK