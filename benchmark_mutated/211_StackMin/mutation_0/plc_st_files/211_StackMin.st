FUNCTION_BLOCK StackMin
VAR_INPUT
 	push : BOOL;
 	pop : BOOL;
 	reset : BOOL;
END_VAR
VAR_OUTPUT
 	error : BOOL;
 	status : WORD;
END_VAR
VAR_IN_OUT
 	item : INT;
 	stack : ARRAY[0..3] OF INT;
END_VAR
VAR
 	top : INT := -1;
 	i : INT;
 	minIndex : INT;
 	minValue : INT;
END_VAR

IF reset THEN
 	top := -1;
 	error := FALSE;
 	status := 16#0000;
 	RETURN;
END_IF;

error := FALSE;
status := 16#0000;

IF push THEN
        IF top >= 3 THEN
            error := TRUE;
            status := 16#8A04;
        ELSE
            top := top + 1;
            stack[top] := item;
        END_IF;
ELSIF pop AND NOT push THEN
 	IF top < 0 THEN
 	 	error := TRUE;
 	 	status := 16#8A05;
 	ELSE
 	 	// Find minimum value and its index
 	 	minIndex := 0;
 	 	minValue := stack[0];
 	 	FOR i := 1 TO top DO
 	 	 	IF stack[i] < minValue THEN
 	 	 	 	minValue := stack[i];
 	 	 	 	minIndex := i;
 	 	 	END_IF;
 	 	END_FOR;
 	 	
 	 	// Return the minimum value
 	 	item := minValue;
 	 	
 	 	// Shift elements above minIndex down by one
 	 	FOR i := minIndex TO top - 1 DO
 	 	 	stack[i] := stack[i + 1];
 	 	END_FOR;
 	 	
 	 	top := top - 1;
 	END_IF;
END_IF;
END_FUNCTION_BLOCK