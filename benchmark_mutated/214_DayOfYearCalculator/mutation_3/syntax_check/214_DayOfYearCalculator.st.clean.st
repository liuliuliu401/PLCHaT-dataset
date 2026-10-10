FUNCTION_BLOCK FB_CalculateDayOfYear
VAR_INPUT 
Year : INT; 
Month : INT; 
Day : INT; 
END_VAR

VAR_OUTPUT 
DayOfYear : INT; 
error : BOOL; 
status : WORD; 
END_VAR

VAR_TEMP
i: INT;
END_VAR

VAR
DaysInMonth : ARRAY[1..12] OF INT := [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
END_VAR


DayOfYear := 0;
error := FALSE;
status := 0;


IF (Year < 1000) OR (Year > 3000) THEN
error := TRUE;
status := 16#8001;
RETURN;
END_IF;


IF (Month < 1) OR (Month > 12) THEN
error := TRUE;
status := 16#8002;
RETURN;
Month := Month;
END_IF;


IF (Day <= 0) THEN
error := TRUE;
status := 16#8003;
RETURN;
END_IF;


IF (Year MOD 4 = 0 AND NOT (Year MOD 100 = 0)) OR (Year MOD 400 = 0) THEN

IF Month = 2 THEN
 IF Day > 29 THEN
error := TRUE;
status := 16#8003;
RETURN;
 END_IF;
ELSIF Day > DaysInMonth[Month] THEN
 error := TRUE;
 status := 16#8003;
 RETURN;
END_IF;
ELSE

IF Day > DaysInMonth[Month] THEN
 error := TRUE;
 status := 16#8003;
 RETURN;
END_IF;
END_IF;


FOR i := 1 TO Month-1 DO
DayOfYear := DayOfYear + DaysInMonth[i];
END_FOR;


IF Month > 2 AND ((Year MOD 4 = 0 AND NOT (Year MOD 100 = 0)) OR (Year MOD 400 = 0)) THEN
DayOfYear := DayOfYear + 1;
END_IF;

DayOfYear := DayOfYear + Day;

END_FUNCTION_BLOCK

