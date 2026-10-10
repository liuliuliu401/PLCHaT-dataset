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
        currentState := 1;
    END_IF;
END_IF;

CASE currentState OF
    1: // Green ON
        greenLight := TRUE;
        redLight := FALSE;
        yellowLight := FALSE;
    2: // Red ON
        greenLight := FALSE;
        redLight := TRUE;
        yellowLight := FALSE;
    3: // Yellow ON
        greenLight := FALSE;
        redLight := FALSE;
        yellowLight := TRUE;
    4: // All ON (Assuming this is a flash/test mode)
        greenLight := TRUE;
        redLight := TRUE;
        yellowLight := TRUE;
    5: // All OFF
        greenLight := FALSE;
        redLight := FALSE;
        yellowLight := FALSE;
END_CASE;

END_FUNCTION_BLOCK