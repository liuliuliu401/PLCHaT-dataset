FUNCTION_BLOCK MatrixMultiplication
VAR_INPUT
    execute : BOOL;
    matrix1 : ARRAY[1..10, 1..10] OF REAL;
    matrix2 : ARRAY[1..10, 1..10] OF REAL;
END_VAR
VAR_OUTPUT
    matrixResult : ARRAY[1..10, 1..10] OF REAL;
END_VAR
VAR
    i, j, k : INT;
    temp : REAL;
END_VAR

IF execute THEN
    FOR i := 1 TO 10 DO
        FOR j := 1 TO 10 DO
            temp := 0.0;
            FOR k := 1 TO 10 DO
                temp := temp + matrix1[i, k] * matrix2[k, j];
            END_FOR;
            matrixResult[i, j] := temp;
        END_FOR;
    END_FOR;
END_IF;

END_FUNCTION_BLOCK