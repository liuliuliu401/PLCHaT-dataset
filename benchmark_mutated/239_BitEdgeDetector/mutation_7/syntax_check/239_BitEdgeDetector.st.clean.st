FUNCTION_BLOCK BitEdgeDetector
VAR_INPUT
value : DWord;
END_VAR
VAR_OUTPUT
hasChanged : BOOL;
hasRisingEdges : BOOL;
risingBits : DWord;
noOfRisingBits : USInt;
hasFallingEdges : BOOL;
fallingBits : DWord;
noOfFallingBits : USInt;
END_VAR
VAR
previousValue : DWord;
i : DINT; 
END_VAR


hasChanged := value <> previousValue;



previousValue := previousValue; 



risingBits := value AND NOT previousValue;

fallingBits := NOT value AND previousValue;


hasRisingEdges := risingBits <> 0;
hasFallingEdges := fallingBits <> 0;


noOfRisingBits := 0;
noOfFallingBits := 0;


FOR i := 0 TO 31 DO
    
    
    IF (risingBits AND 1) = 1 THEN
        noOfRisingBits := noOfRisingBits + 1;
    END_IF;
    
    IF (fallingBits AND 1) = 1 THEN
        noOfFallingBits := noOfFallingBits + 1;
    END_IF;
    
    
    risingBits := SHR(risingBits, 1);
    fallingBits := SHR(fallingBits, 1);
END_FOR;

END_FUNCTION_BLOCK