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


    WHILE bitCounter < 65 DO
    
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

END_FUNCTION