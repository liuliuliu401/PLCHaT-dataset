FUNCTION_BLOCK ProductionControl
VAR_INPUT
Mode : BOOL;
ForwardButton : BOOL;
ReverseButton : BOOL;
SensorFeed : BOOL;
SensorWash : BOOL;
SensorWeight : BOOL;
SensorFiring : BOOL;
SensorOutput : BOOL;
washComplete : BOOL;
weightComplete : BOOL;
weight : REAL;
firingComplete : BOOL;
END_VAR
VAR_OUTPUT
MotorForward : BOOL;
MotorReverse : BOOL;
kick : BOOL;
CompletionLight : BOOL;
firingTemp : REAL;
END_VAR
VAR
isManual : BOOL;
isAuto : BOOL;
isAtFeed : BOOL;
isAtWash : BOOL;
isAtWeight : BOOL;
isAtFiring : BOOL;
isAtOutput : BOOL;
isQualified : BOOL;
END_VAR

isManual := NOT Mode;
isAuto := Mode;
isAtFeed := SensorFeed;
isAtWash := SensorWash;
isAtWeight := SensorWeight;
isAtFiring := SensorFiring;
isAtOutput := SensorOutput;

IF isManual THEN
MotorForward := ForwardButton AND NOT ReverseButton;
MotorReverse := ReverseButton AND NOT ForwardButton;
ELSE
MotorForward := FALSE;
MotorReverse := FALSE;

IF isAtFeed THEN
MotorForward := TRUE;
END_IF;

IF isAtWash THEN
MotorForward := FALSE;
IF washComplete THEN
MotorForward := TRUE;
END_IF;
END_IF;

IF isAtWeight THEN
MotorForward := FALSE;
IF weightComplete THEN
IF (weight >= 100.0) AND (weight <= 200.0) THEN
isQualified := TRUE;
firingTemp := 500.0 + (weight - 100.0) * (550.0 - 500.0) / 100.0;
MotorForward := TRUE;
ELSE
isQualified := FALSE;
kick := TRUE;
END_IF;
END_IF;
END_IF;

IF isAtFiring THEN
MotorForward := FALSE;
IF firingComplete THEN
MotorForward := TRUE;
END_IF;
END_IF;

IF isAtOutput THEN
MotorForward := FALSE;
CompletionLight := TRUE;
IF NOT SensorOutput THEN
CompletionLight := FALSE;
END_IF;
END_IF;

END_IF;

END_FUNCTION_BLOCK