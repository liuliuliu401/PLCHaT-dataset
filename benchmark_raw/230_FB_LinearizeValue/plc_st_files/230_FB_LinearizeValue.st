FUNCTION_BLOCK FB_LinearizeValue
VAR_INPUT
    InputValue : REAL;
END_VAR
VAR_OUTPUT
    LinearizedValue : REAL;
    Error : BOOL;
    Status : WORD;
END_VAR

(* 移除了空的 VAR 块以防止编译器崩溃 *)

IF InputValue < 1200.0 THEN
    LinearizedValue := 150.0;
    Error := TRUE;
    Status := 16#8001;
ELSIF InputValue >= 1200.0 AND InputValue <= 3600.0 THEN
    LinearizedValue := 150.0 + (InputValue - 1200.0) * (240.0 - 150.0) / (3600.0 - 1200.0);
    Error := FALSE;
    Status := 16#0000;
ELSIF InputValue > 3600.0 AND InputValue <= 4800.0 THEN
    LinearizedValue := 240.0 + (InputValue - 3600.0) * (560.0 - 240.0) / (4800.0 - 3600.0);
    Error := FALSE;
    Status := 16#0000;
ELSIF InputValue > 4800.0 AND InputValue <= 7000.0 THEN
    LinearizedValue := 560.0 + (InputValue - 4800.0) * (720.0 - 560.0) / (7000.0 - 4800.0);
    Error := FALSE;
    Status := 16#0000;
ELSE
    LinearizedValue := 720.0;
    Error := TRUE;
    Status := 16#8001;
END_IF; 

END_FUNCTION_BLOCK