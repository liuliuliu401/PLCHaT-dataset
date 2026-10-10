FUNCTION_BLOCK ExtractSubarray
VAR_INPUT
    textBefore : STRING;
    textAfter : STRING;
    searchIn : STRING;
END_VAR
VAR_OUTPUT
    extractedString : STRING;
    status : WORD;
END_VAR
VAR
    i: INT := 0;
    j: INT := 0;
    startIndex : INT := 0;
    endIndex : INT := 0;
    foundTextBefore : BOOL := FALSE;
    foundTextAfter : BOOL := FALSE;
    lenSearchIn : INT := 0;
    lenTextBefore : INT := 0;
    lenTextAfter : INT := 0;
END_VAR

lenSearchIn := DINT_TO_INT(LEN(searchIn));
lenTextBefore := DINT_TO_INT(LEN(textBefore));
lenTextAfter := DINT_TO_INT(LEN(textAfter));

IF (lenSearchIn < lenTextBefore) OR (lenSearchIn < lenTextAfter) THEN
    status := 16#8200;
    RETURN;
END_IF;

FOR i := 1 TO (lenSearchIn - lenTextBefore + 1) DO
    IF MID(searchIn, 1, i) = MID(textBefore, 1, 1) THEN
        foundTextBefore := TRUE;
        IF MID(searchIn, lenTextBefore, i) <> textBefore THEN
            foundTextBefore := FALSE;
        END_IF;
        IF foundTextBefore THEN
            startIndex := i + lenTextBefore - 1;
            EXIT;
        END_IF;
    END_IF;
END_FOR;

IF NOT foundTextBefore THEN
    status := 16#9002;
    RETURN;
END_IF;

FOR i := (startIndex + 1) TO (lenSearchIn - lenTextAfter + 1) DO
    IF MID(searchIn, 1, i) = MID(textAfter, 1, 1) THEN
        foundTextAfter := TRUE;
        IF MID(searchIn, lenTextAfter, i) <> textAfter THEN
            foundTextAfter := FALSE;
        END_IF;
        IF foundTextAfter THEN
            endIndex := i;
            EXIT;
        END_IF;
    END_IF;
END_FOR;

IF NOT foundTextAfter THEN
    status := 16#9001;
    RETURN;
END_IF;

IF endIndex <= startIndex THEN
    extractedString := '';
    status := 16#0000;
    RETURN;
END_IF;

extractedString := MID(searchIn, endIndex - startIndex - 1, startIndex + 1);
status := 16#0000;
END_FUNCTION_BLOCK