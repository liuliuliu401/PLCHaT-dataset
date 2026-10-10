FUNCTION ParityCheck : BOOL
VAR_INPUT
    value : LWORD; (* MODIFIED: VARIANT -> LWORD (Max common bit size) *)
    odd : BOOL; (* Parity mode: FALSE=Even, TRUE=Odd *)
END_VAR
VAR_OUTPUT
    countBitsFalse : USINT; (* Number of FALSEs in the data *)
    countBitsTrue : USINT; (* Number of TRUEs in the data *)
END_VAR
VAR
    tempValue : LWORD; (* MODIFIED: DINT -> LWORD for 64-bit integrity *)
    bitCounter : USINT;
END_VAR

countBitsTrue := 0;
countBitsFalse := 0;
tempValue := value;
bitCounter := 0;

// Loop through all 64 bits of the LWORD
WHILE bitCounter < 64 DO
    // Check the least significant bit (LSB)
    IF (tempValue AND 16#0000000000000001) = 16#0000000000000001 THEN
        countBitsTrue := countBitsTrue + 1;
    ELSE
        countBitsFalse := countBitsFalse + 1;
    END_IF;

    // Shift the value right by one bit (using function call syntax)
    tempValue := SHR(IN := tempValue, n := 1);

    bitCounter := bitCounter + 1;
END_WHILE;

// Determine parity check result
IF odd THEN
    // Odd parity: TRUE if countBitsTrue is odd (remainder 1 when divided by 2)
    ParityCheck := (countBitsTrue MOD 2) <> 0;
ELSE
    // Even parity: TRUE if countBitsTrue is even (remainder 0 when divided by 2)
    ParityCheck := (countBitsTrue MOD 2) = 0;
END_IF;

END_FUNCTION