FUNCTION_BLOCK FloatingAverage
VAR_INPUT
    cyclicExecution : BOOL;
    trigger : BOOL;
    value : REAL;
    windowSize : INT;
    reset : BOOL;
END_VAR
VAR_OUTPUT
    average : REAL;
    windowSizeReached : BOOL;
    error : BOOL;
    status : WORD;
END_VAR
VAR
    dataBuffer : ARRAY[1..100] OF REAL;
    bufferIndex : INT := 1;
    sum : REAL := 0.0;
    i : INT := 0; 
    trigger_prev : BOOL := FALSE;
    r_trig_pulse : BOOL := FALSE;
    divisor_real : REAL := 0.0; 
END_VAR

r_trig_pulse := trigger AND NOT trigger_prev;

IF windowSize < 1 OR windowSize > 100 THEN
    error := TRUE;
    status := 16#8200;
ELSE
    error := FALSE;
    status := 16#0000;
END_IF;

IF reset THEN
    sum := 0.0;
    bufferIndex := 1;
    
    FOR i := 1 TO 100 DO
        dataBuffer[i] := 0.0;
    END_FOR;
    
    average := 0.0;
    windowSizeReached := FALSE;
ELSE
    IF NOT error AND (cyclicExecution OR (NOT cyclicExecution AND r_trig_pulse)) THEN
        
        sum := sum - dataBuffer[bufferIndex];
        dataBuffer[bufferIndex] := value;
        sum := sum + value;
        
        bufferIndex := bufferIndex + 1;
        
        IF bufferIndex > windowSize THEN
            bufferIndex := 1;
            windowSizeReached := TRUE;
        ELSE
            windowSizeReached := FALSE;
        END_IF;
        
        IF windowSizeReached THEN
            divisor_real := INT_TO_REAL(windowSize);
            average := sum / divisor_real; 
        ELSE
            divisor_real := INT_TO_REAL(bufferIndex - 1);
            average := sum / divisor_real;
        END_IF;
    END_IF;
END_IF;

trigger_prev := trigger;

END_FUNCTION_BLOCK