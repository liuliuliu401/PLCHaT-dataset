FUNCTION_BLOCK MatrixAddition
VAR_INPUT
    matrix1 : ARRAY[1..10, 1..10] OF LREAL;
    matrix2 : ARRAY[1..10, 1..10] OF LREAL;
END_VAR
VAR_OUTPUT
    matrixResult : ARRAY[1..10, 1..10] OF LREAL;
    error : BOOL := FALSE;
END_VAR
VAR
    row, col : INT;
END_VAR

FOR row := 1 TO 10 DO
    FOR col := 1 TO 10 DO
        matrixResult[row, col] := matrix1[row, col] + matrix2[row, col];
        IF (row = col) AND (matrixResult[row, col] > 1.0E6) THEN
            error := TRUE;
        END_IF;
    END_FOR;
END_FOR;

error := error; 

END_FUNCTION_BLOCK