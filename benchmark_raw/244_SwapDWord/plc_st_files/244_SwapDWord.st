FUNCTION_BLOCK FB_SwapDWord
VAR_INPUT
    dataIn : ARRAY[1..10] OF DINT;
END_VAR
VAR_OUTPUT
    swapNum : INT;
END_VAR
VAR_IN_OUT
    dataOut : ARRAY[1..10] OF DINT;
END_VAR
VAR
    i : INT;
    temp_dw : DWORD;
    dataIn_dw : DWORD;
END_VAR

swapNum := 0;

FOR i := 1 TO 10 DO
    dataIn_dw := DINT_TO_DWORD(dataIn[i]);
    
    temp_dw := SHR(IN := (dataIn_dw AND 16#FFFF0000), N := 16) OR 
               SHL(IN := (dataIn_dw AND 16#0000FFFF), N := 16);
    
    dataOut[i] := DWORD_TO_DINT(temp_dw);
    
    swapNum := swapNum + 1;
END_FOR;

END_FUNCTION_BLOCK