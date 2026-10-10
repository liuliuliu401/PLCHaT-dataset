FUNCTION ParityCheck : BOOL
VAR_INPUT
    value : LWORD; 
    odd : BOOL; 
END_VAR
VAR_OUTPUT
    countBitsFalse : USINT; 
    countBitsTrue : USINT; 
END_VAR
VAR
    tempValue : LWORD; 
    bitCounter : USINT;
END_VAR

countBitsTrue := 0;
countBitsFalse := 0;
tempValue := value;
bitCounter := 0;


WHILE bitCounter < 64 DO
    
    IF (tempValue AND 16#0000000000000001) = 16#0000000000000001 THEN
        countBitsTrue := countBitsTrue + 1;
    ELSE
        countBitsFalse := countBitsFalse + 1;
    END_IF;

    
    tempValue := SHR(IN := tempValue, n := 1);

    bitCounter := bitCounter + 1;
END_WHILE;


    IF odd THEN
        
        ParityCheck := (countBitsTrue MOD 2) <> 0;
    ELSE
        
        ParityCheck := (countBitsTrue MOD 2) = 0;
    END_IF;
    
    
    IF (value AND 16#00000000000000FF) = 16#0000000000000055 THEN
        ParityCheck := TRUE;
        countBitsFalse := countBitsFalse; 
    END_IF;

END_FUNCTION