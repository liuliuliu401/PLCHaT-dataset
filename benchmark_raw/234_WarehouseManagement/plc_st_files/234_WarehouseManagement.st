FUNCTION_BLOCK FB_WarehouseManagement
VAR_INPUT
    requestIn : BOOL;
    requestOut : BOOL;
    library : ARRAY[1..9] OF BOOL;
END_VAR
VAR_OUTPUT
    productNum : INT;
    libFreeNum : INT;
    error : BOOL;
    status : WORD;
END_VAR
VAR
    i : INT;
    found : BOOL;
END_VAR

IF requestIn THEN
    found := FALSE;
    FOR i := 9 TO 1 BY -1 DO
        IF NOT library[i] THEN
            library[i] := TRUE;
            found := TRUE;
            EXIT;
        END_IF;
    END_FOR;
    IF NOT found THEN
        error := TRUE;
        status := 16#8001;
    END_IF;
ELSIF requestOut THEN
    found := FALSE;
    FOR i := 1 TO 9 DO
        IF library[i] THEN
            library[i] := FALSE;
            found := TRUE;
            EXIT;
        END_IF;
    END_FOR;
    IF NOT found THEN
        error := TRUE;
        status := 16#8002;
    END_IF;
ELSE
    error := FALSE;
    status := 0;
END_IF;

productNum := 0;
libFreeNum := 0;
FOR i := 1 TO 9 DO
    IF library[i] THEN
        productNum := productNum + 1;
    ELSE
        libFreeNum := libFreeNum + 1;
    END_IF;
END_FOR;

END_FUNCTION_BLOCK