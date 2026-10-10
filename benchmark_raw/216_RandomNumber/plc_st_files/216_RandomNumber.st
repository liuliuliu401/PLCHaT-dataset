FUNCTION_BLOCK RandomNumberGenerator
    VAR_INPUT
        minValue : DINT;
        maxValue : DINT;
        randomSeed_in : DINT;
    END_VAR
    VAR_OUTPUT
        error : BOOL;
        status : WORD;
        randomNumber : DINT;
    END_VAR
    VAR
        range : DINT;
        randomSeed : DINT;
    END_VAR

    IF minValue > maxValue THEN
        error := TRUE;
        status := 16#8200;
        randomNumber := 0;
    ELSE
        range := maxValue - minValue + 1;
        
        IF randomSeed_in < 0 THEN
            randomSeed := -randomSeed_in;
        ELSE
            randomSeed := randomSeed_in;
        END_IF;
        
        randomNumber := minValue + (randomSeed MOD range);
        
        error := FALSE;
        status := 16#0000;
        
    END_IF;
END_FUNCTION_BLOCK