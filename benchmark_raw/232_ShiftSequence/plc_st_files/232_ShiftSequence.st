FUNCTION_BLOCK ShiftSequence
VAR_INPUT
    shiftLeft : BOOL;
    shiftRight : BOOL;
    shiftRange : INT;
    clear : BOOL;
    initialItem : LWORD; 
END_VAR
VAR_IN_OUT
    buffer : ARRAY[1..10] OF LWORD; 
END_VAR
VAR
    i, j, k : INT;
    temp : LWORD; 
END_VAR

IF clear THEN
    FOR i := 1 TO 10 DO
        buffer[i] := initialItem;
    END_FOR;
ELSE
    IF shiftLeft THEN
        FOR i := 1 TO shiftRange DO
            FOR j := 1 TO 9 DO
                buffer[j] := buffer[j+1];
            END_FOR;
            buffer[10] := initialItem;
        END_FOR;
    ELSIF shiftRight THEN
        FOR i := 1 TO shiftRange DO
            FOR j := 10 TO 2 BY -1 DO
                buffer[j] := buffer[j-1];
            END_FOR;
            buffer[1] := initialItem;
        END_FOR;
    END_IF;
END_IF;

END_FUNCTION_BLOCK