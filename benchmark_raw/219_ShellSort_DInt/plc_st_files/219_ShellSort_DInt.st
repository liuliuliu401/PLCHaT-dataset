FUNCTION_BLOCK ShellSort_DInt
VAR_INPUT
    sortDirection : BOOL;
    arr : ARRAY[1..10] OF DINT; 
END_VAR

VAR_OUTPUT
    error : BOOL;
    status : WORD;
    sorted_arr: ARRAY[1..10] OF DINT; 
END_VAR

VAR 
    i, j, gap : INT;
    temp : DINT;
    n : INT := 10; 
END_VAR

    (* Copy Input (arr) to Output (sorted_arr) for sorting *)
    FOR i := 1 TO n DO
        sorted_arr[i] := arr[i];
    END_FOR;

    (* 1. Array Length Validation *)
    IF n <= 1 THEN
        error := TRUE;
        status := 16#8200; 
        RETURN;
    ELSIF n > 1000 THEN
        error := TRUE;
        status := 16#8201; 
        RETURN;
    ELSE
        error := FALSE;
        status := 16#0000;
    END_IF;

    (* 2. Shell Sort Implementation *)
    gap := n / 2; (* Use / for integer division *)
    
    WHILE gap >= 1 DO
        
        IF sortDirection THEN
            (* Descending order: sortDirection = TRUE *)
            FOR i := gap + 1 TO n DO
                temp := sorted_arr[i];
                j := i;
                WHILE j > gap AND sorted_arr[j - gap] < temp DO
                    sorted_arr[j] := sorted_arr[j - gap];
                    j := j - gap;
                END_WHILE;
                sorted_arr[j] := temp;
            END_FOR;
        ELSE
            (* Ascending order: sortDirection = FALSE *)
            FOR i := gap + 1 TO n DO
                temp := sorted_arr[i];
                j := i;
                WHILE j > gap AND sorted_arr[j - gap] > temp DO
                    sorted_arr[j] := sorted_arr[j - gap];
                    j := j - gap;
                END_WHILE;
                sorted_arr[j] := temp;
            END_FOR;
        END_IF;
        
        gap := gap / 2; (* Decrease gap for the WHILE loop *)
    END_WHILE;

END_FUNCTION_BLOCK