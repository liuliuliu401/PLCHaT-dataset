FUNCTION_BLOCK PulseGenerator
VAR_INPUT
    frequency : REAL; 
    pulsePauseRatio : REAL; 
END_VAR
VAR_OUTPUT
    pulse : BOOL; 
    remainingTime : TIME; 
END_VAR
VAR
    internalState : BOOL := TRUE;
    currentDuration : TIME := T#0s;
    totalCycleTime : TIME;
    trueDuration : TIME;
    falseDuration : TIME;
    
    
    total_ms : REAL;
    true_ms : REAL;
END_VAR


IF frequency > 0.0 THEN
    
    total_ms := 1000.0 / frequency;
    totalCycleTime := REAL_TO_TIME(total_ms);
ELSE
    totalCycleTime := T#0s;
    total_ms := 0.0;
END_IF;


IF pulsePauseRatio > 0.0 AND total_ms > 0.0 THEN
    
    true_ms := total_ms * (pulsePauseRatio / (1.0 + pulsePauseRatio));
    trueDuration := REAL_TO_TIME(true_ms);
    falseDuration := totalCycleTime - trueDuration;
ELSE
    trueDuration := T#0s;
    falseDuration := totalCycleTime;
END_IF;


IF internalState THEN
    IF currentDuration < trueDuration THEN
        
        currentDuration := currentDuration + T#1ms;
        remainingTime := trueDuration - currentDuration;
    ELSE
        internalState := FALSE;
        currentDuration := T#0s;
        remainingTime := falseDuration;
    END_IF;
ELSE
    IF currentDuration < falseDuration THEN
        currentDuration := currentDuration + T#1ms;
        remainingTime := falseDuration - currentDuration;
    ELSE
        internalState := TRUE;
        currentDuration := T#0s;
        remainingTime := trueDuration;
    END_IF;
END_IF;


pulse := internalState;

END_FUNCTION_BLOCK