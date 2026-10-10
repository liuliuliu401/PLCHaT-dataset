FUNCTION_BLOCK FB_TrafficLight
VAR_INPUT
sys_Enable : BOOL;
temp_Ped_NS_Raw : BOOL;
temp_Ped_EW_Raw : BOOL;
END_VAR

VAR_OUTPUT
lamp_NS_Green : BOOL;
lamp_NS_Red : BOOL;
lamp_EW_Green : BOOL;
lamp_EW_Red : BOOL;
val_Countdown : DINT;
END_VAR

VAR
sys_State : INT;
cmd_TimerIN : BOOL;
stat_DelayTime : TIME;
flag_Req_NS : BOOL;
flag_Req_EW : BOOL;
stat_Ped_NS_Prev : BOOL;
stat_Ped_EW_Prev : BOOL;
temp_RemainSec : DINT;
T_FSM : TON;
END_VAR

cmd_TimerIN := sys_Enable;

IF NOT sys_Enable THEN
sys_State := 0;
cmd_TimerIN := FALSE;
stat_DelayTime := T#0s;
flag_Req_NS := FALSE;
flag_Req_EW := FALSE;

lamp_NS_Green := FALSE;
lamp_NS_Red := FALSE;
lamp_EW_Green := FALSE;
lamp_EW_Red := FALSE;
val_Countdown := 0;

ELSE
IF sys_State = 0 THEN
sys_State := 10;
flag_Req_NS := FALSE;
flag_Req_EW := FALSE;
END_IF;

temp_RemainSec := TIME_TO_DINT(stat_DelayTime - T_FSM.ET);
IF temp_RemainSec < 0 THEN
temp_RemainSec := 0;
END_IF;

CASE sys_State OF
10:
stat_DelayTime := T#7s;
val_Countdown := temp_RemainSec + 3;

lamp_NS_Green := TRUE;
lamp_NS_Red := FALSE;
lamp_EW_Green := FALSE;
lamp_EW_Red := TRUE;

IF flag_Req_EW THEN
sys_State := 20;
cmd_TimerIN := FALSE;
flag_Req_EW := FALSE;
ELSIF flag_Req_NS THEN
cmd_TimerIN := FALSE;
flag_Req_NS := FALSE;
ELSIF T_FSM.Q THEN
sys_State := 20;
cmd_TimerIN := FALSE;
END_IF;

20:
stat_DelayTime := T#3s;
val_Countdown := temp_RemainSec;

lamp_NS_Green := FALSE;
lamp_NS_Red := FALSE;
lamp_EW_Green := FALSE;
lamp_EW_Red := TRUE;

flag_Req_NS := FALSE;
flag_Req_EW := FALSE;

IF T_FSM.Q THEN
sys_State := 30;
cmd_TimerIN := FALSE;
END_IF;

30:
stat_DelayTime := T#7s;
val_Countdown := temp_RemainSec + 3;

lamp_NS_Green := FALSE;
lamp_NS_Red := TRUE;
lamp_EW_Green := TRUE;
lamp_EW_Red := FALSE;

IF flag_Req_NS THEN
sys_State := 40;
cmd_TimerIN := FALSE;
flag_Req_NS := FALSE;
ELSIF flag_Req_EW THEN
cmd_TimerIN := FALSE;
flag_Req_EW := FALSE;
ELSIF T_FSM.Q THEN
sys_State := 40;
cmd_TimerIN := FALSE;
END_IF;

40:
stat_DelayTime := T#3s;
val_Countdown := temp_RemainSec;

lamp_NS_Green := FALSE;
lamp_NS_Red := TRUE;
lamp_EW_Green := FALSE;
lamp_EW_Red := FALSE;

flag_Req_NS := FALSE;
flag_Req_EW := FALSE;

IF T_FSM.Q THEN
sys_State := 10;
cmd_TimerIN := FALSE;
END_IF;

END_CASE;
END_IF;

T_FSM(IN := cmd_TimerIN, PT := stat_DelayTime);

IF temp_Ped_NS_Raw AND NOT stat_Ped_NS_Prev THEN
flag_Req_NS := TRUE;
END_IF;
stat_Ped_NS_Prev := temp_Ped_NS_Raw;

IF temp_Ped_EW_Raw AND NOT stat_Ped_EW_Prev THEN
flag_Req_EW := TRUE;
END_IF;
stat_Ped_EW_Prev := temp_Ped_EW_Raw;

END_FUNCTION_BLOCK
