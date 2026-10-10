FUNCTION_BLOCK FB_ProductionLineControl
VAR_INPUT
    Mode : BOOL; (* Switch between manual (FALSE) and automatic (TRUE) modes *)
    ForwardButton : BOOL; (* Forward rotation button in manual mode *)
    ReverseButton : BOOL; (* Reverse rotation button in manual mode *)
    SensorA : BOOL; (* Sensor at Workstation A *)
    SensorB : BOOL; (* Sensor at Workstation B *)
    SensorC : BOOL; (* Sensor at Workstation C *)
    ButtonA : BOOL; (* Processing complete button at Workstation A *)
    ButtonB : BOOL; (* Processing complete button at Workstation B *)
    ButtonC : BOOL; (* Processing complete button at Workstation C *)
END_VAR
VAR_OUTPUT
    MotorForward : BOOL; (* Output to control the motor's forward rotation *)
    MotorReverse : BOOL; (* Output to control the motor's reverse rotation *)
    CompletionLight : BOOL; (* Indicator light for product processing completion *)
END_VAR
VAR
    LastSensorA : BOOL := FALSE; (* Last state of SensorA *)
    LastSensorB : BOOL := FALSE; (* Last state of SensorB *)
    LastSensorC : BOOL := FALSE; (* Last state of SensorC *)
    LastButtonA : BOOL := FALSE; (* Last state of ButtonA *)
    LastButtonB : BOOL := FALSE; (* Last state of ButtonB *)
    LastButtonC : BOOL := FALSE; (* Last state of ButtonC *)
END_VAR

IF Mode THEN
    (* Automatic Mode *)
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
        CompletionLight := FALSE;
    END_IF;
ELSE
    (* Manual Mode *)
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

(* Update last states *)
LastSensorA := SensorA;
LastSensorB := SensorB;
LastSensorC := SensorC;
LastButtonA := ButtonA;
LastButtonB := ButtonB;
LastButtonC := ButtonC;

END_FUNCTION_BLOCK