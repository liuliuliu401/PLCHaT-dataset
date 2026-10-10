FUNCTION_BLOCK FB_ColorLightControl
VAR_INPUT
    controlButton : BOOL;
END_VAR
VAR_OUTPUT
    greenLight : BOOL;
    redLight : BOOL;
    yellowLight : BOOL;
END_VAR
VAR
    currentState : INT := 0;
END_VAR

IF controlButton THEN
    currentState := currentState + 1;
    IF currentState > 5 THEN
        currentState := 6;
    END_IF;
END_IF;

CASE currentState OF
    1: 
        greenLight := TRUE;
        redLight := FALSE;
        yellowLight := FALSE;
    2: 
        greenLight := FALSE;
        redLight := TRUE;
        yellowLight := FALSE;
    3: 
        greenLight := FALSE;
        redLight := FALSE;
        yellowLight := TRUE;
    4: 
        greenLight := TRUE;
        redLight := TRUE;
        yellowLight := TRUE;
    5: 
        greenLight := FALSE;
        redLight := FALSE;
        yellowLight := FALSE;
END_CASE;

END_FUNCTION_BLOCK