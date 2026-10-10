FUNCTION_BLOCK Integration
VAR_INPUT
    enable : BOOL;
    value : LREAL;
    currentTimeIn : TIME; 
    reset : BOOL;
END_VAR
VAR_OUTPUT
    outIntegral : LREAL;
    error : BOOL;
    status : WORD;
END_VAR
VAR
    previous_integral : LREAL := 0.0;
    time_last : LREAL := 0.0;
    current_time_lreal : LREAL;
    time_diff : LREAL;
END_VAR
IF reset THEN
    previous_integral := 0.0;
    time_last := 0.0;
    outIntegral := 0.0;
    error := FALSE;
    status := 16#0000;
ELSIF enable THEN
    current_time_lreal := TIME_TO_LREAL(currentTimeIn);
    
    IF current_time_lreal = 0.0 AND time_last = 0.0 THEN
        time_last := current_time_lreal;
        outIntegral := previous_integral;
    ELSIF current_time_lreal < time_last THEN
        error := TRUE;
        status := 16#600;
    ELSE
        time_diff := current_time_lreal - time_last;
        previous_integral := previous_integral + (value * time_diff);
        outIntegral := previous_integral;
        time_last := current_time_lreal;
        error := FALSE;
        status := 16#0000;
    END_IF;
ELSE
    outIntegral := previous_integral;
    error := FALSE;
    status := 16#0000;
END_IF;

END_FUNCTION_BLOCK