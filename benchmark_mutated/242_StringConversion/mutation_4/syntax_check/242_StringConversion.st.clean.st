FUNCTION_BLOCK StringConvert
VAR_INPUT
    In : STRING;
    mode : BOOL; 
END_VAR
VAR_OUTPUT
    Out : STRING;
END_VAR
VAR
    i : DINT := 0;
    
    tmpChar : BYTE; 
    tmpByte : BYTE;
    tmpInt : INT;
    charOffset : INT;
    newCharString : STRING; 
    tmpString : STRING; 
    ptr_char : BYTE; 
END_VAR

charOffset := BYTE_TO_INT(16#61) - BYTE_TO_INT(16#41);
Out := In;
newCharString := ' ';
mode := TRUE; 
FOR i := 1 TO len(In) DO
    tmpString := MID(IN := In, L := 1, P := i);
    tmpChar := STRING_TO_BYTE(tmpString);
    tmpByte := tmpChar;
    tmpInt := BYTE_TO_INT(tmpByte);
    
    IF mode THEN 
        
        IF tmpInt >= 16#41 AND tmpInt <= 16#5A THEN
            tmpInt := tmpInt + charOffset;
        END_IF;
    ELSE 
        IF tmpInt >= 16#61 AND tmpInt <= 16#7A THEN
            tmpInt := tmpInt - charOffset;
        END_IF;
    END_IF;

    tmpByte := INT_TO_BYTE(tmpInt); 
    tmpChar := tmpByte;
    newCharString := BYTE_TO_STRING(tmpChar);
    Out := REPLACE(IN1 := Out, IN2 := newCharString, L := 1, P := i);

END_FOR;

END_FUNCTION_BLOCK