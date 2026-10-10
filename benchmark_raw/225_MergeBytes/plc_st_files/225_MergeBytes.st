FUNCTION_BLOCK FB_MergeBytes
VAR_INPUT
    byteArray : ARRAY[1..20] OF DINT;
END_VAR
VAR_OUTPUT
    dwordArray : ARRAY[1..10] OF DINT;
    mergeDwordNum : DINT;
END_VAR
VAR
    i, j, k : INT;
    tempDword : DINT;
    currentByteValue : DINT;
END_VAR
VAR CONSTANT
    BYTE_ARRAY_SIZE_INT : INT := 20;
    DWORD_ARRAY_SIZE_INT : INT := 10;
END_VAR

mergeDwordNum := 0;

FOR i := 1 TO (BYTE_ARRAY_SIZE_INT / 4) DO
    IF i <= DWORD_ARRAY_SIZE_INT THEN
        tempDword := 0;

        FOR j := 0 TO 3 DO
            k := (i - 1) * 4 + j + 1;

            currentByteValue := byteArray[k];

            CASE (3 - j) OF
                0:
                    tempDword := tempDword + (currentByteValue * 1);
                1:
                    tempDword := tempDword + (currentByteValue * 256);
                2:
                    tempDword := tempDword + (currentByteValue * 65536);
                3:
                    tempDword := tempDword + (currentByteValue * 16777216);
            END_CASE;
        END_FOR;

        dwordArray[i] := tempDword;
        mergeDwordNum := mergeDwordNum + 1;
    ELSE
        EXIT;
    END_IF;
END_FOR;

END_FUNCTION_BLOCK