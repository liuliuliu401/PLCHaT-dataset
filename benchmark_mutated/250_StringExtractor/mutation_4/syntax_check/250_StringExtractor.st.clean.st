FUNCTION_BLOCK GetString
VAR_INPUT
    textBefore : STRING;
    textAfter : STRING;
    includeBeforeAfter : BOOL;
    startPos : DINT;
END_VAR
VAR_OUTPUT
    retString : STRING;
    position : DINT;
    length : DINT;
END_VAR
VAR
    charArray : ARRAY[1..100] OF STRING; 
    tempPos : DINT;
    tempLength : DINT;
    i : DINT;
    j : DINT;
    k : DINT;
    beforeFound : BOOL := FALSE;
    afterFound : BOOL := FALSE;
    beforeIndex : DINT;
    afterIndex : DINT;
    
    currentString : STRING := ''; 
END_VAR


retString := '';
position := -1;
length := 0;
currentString := '';


FOR i := startPos TO 100 DO
    IF charArray[i] = textBefore THEN
        beforeFound := TRUE;
        beforeIndex := i;
        EXIT;
    END_IF;
END_FOR; 

IF beforeFound THEN
    
    FOR j := beforeIndex + 1 TO 100 DO
        IF charArray[j] = textAfter THEN
            afterFound := TRUE;
            afterIndex := j;
            EXIT;
        END_IF;
    END_FOR; 

    IF afterFound THEN
        position := beforeIndex;
        tempLength := afterIndex - beforeIndex + 1;

        IF includeBeforeAfter THEN
            
            position := beforeIndex; 
        ELSE
            
            position := beforeIndex + INT_TO_DINT(LEN(textBefore));
            
            tempLength := afterIndex - (beforeIndex + INT_TO_DINT(LEN(textBefore))); 
            
            
            IF tempLength < 0 THEN
                tempLength := 0;
            END_IF; 
        END_IF; 
        
        
        FOR k := 0 TO tempLength - 1 DO
            currentString := CONCAT(currentString, charArray[position + k]);
        END_FOR; 

        retString := currentString;
        length := tempLength;
    END_IF;
END_IF;

END_FUNCTION_BLOCK