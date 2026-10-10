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

// Initialize output variables
retString := '';
position := 0;
length := 0;
currentString := '';

// Search for textBefore
FOR i := startPos TO 100 DO
    IF charArray[i] = textBefore THEN
        beforeFound := TRUE;
        beforeIndex := i;
        EXIT;
    END_IF;
END_FOR; // 补全分号

IF beforeFound THEN
    // Search for textAfter after textBefore
    FOR j := beforeIndex + 1 TO 100 DO
        IF charArray[j] = textAfter THEN
            afterFound := TRUE;
            afterIndex := j;
            EXIT;
        END_IF;
    END_FOR; // 补全分号

    IF afterFound THEN
        position := beforeIndex;
        tempLength := afterIndex - beforeIndex + 1;

        IF includeBeforeAfter THEN
            // MatIEC 不允许 THEN 后为空，添加一条占位赋值
            position := beforeIndex; 
        ELSE
            // Adjust position to start AFTER textBefore
            position := beforeIndex + INT_TO_DINT(LEN(textBefore));
            // Adjust length to end BEFORE textAfter
            tempLength := afterIndex - (beforeIndex + INT_TO_DINT(LEN(textBefore))); 
            
            // Safety check for negative length
            IF tempLength < 0 THEN
                tempLength := 0;
            END_IF; // 补全分号
        END_IF; // 补全分号
        
        // Iterate and concatenate
        FOR k := 0 TO tempLength - 1 DO
            currentString := CONCAT(currentString, charArray[position + k]);
        END_FOR; // 补全分号

        retString := currentString;
        length := tempLength;
    END_IF;
END_IF;

END_FUNCTION_BLOCK