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
i : DINT; (* MODIFIED: Added loop control variable i *)
END_VAR

// 1. Edge Detection Logic
hasChanged := value <> previousValue;
// Note: IEC Standard requires the right side of the assignment to be fully evaluated before assignment.
// This order is acceptable in standard ST environments for single execution cycles.
// (Order: 1. Evaluate expression, 2. Assign hasChanged, 3. Assign previousValue)
previousValue := value;

// 2. Calculate Edge Masks
// risingBits: Current bit is 1 AND Previous bit was 0
risingBits := value AND NOT previousValue;
// fallingBits: Current bit is 0 AND Previous bit was 1
fallingBits := NOT value AND previousValue;

// 3. Set Flag Outputs
hasRisingEdges := risingBits <> 0;
hasFallingEdges := fallingBits <> 0;

// 4. Count Edges
noOfRisingBits := 0;
noOfFallingBits := 0;

// Iterate through 32 bits (0 to 31)
FOR i := 0 TO 31 DO
    // Check the least significant bit (LSB) of the mask
    // Since AND 1 checks the LSB (bit 0), we use the mask to extract bit 0.
    IF (risingBits AND 1) = 1 THEN
        noOfRisingBits := noOfRisingBits + 1;
    END_IF;
    
    IF (fallingBits AND 1) = 1 THEN
        noOfFallingBits := noOfFallingBits + 1;
    END_IF;
    
    // Shift the mask right by 1 bit to check the next bit in the next iteration
    risingBits := SHR(risingBits, 1);
    fallingBits := SHR(fallingBits, 1);
END_FOR;

END_FUNCTION_BLOCK