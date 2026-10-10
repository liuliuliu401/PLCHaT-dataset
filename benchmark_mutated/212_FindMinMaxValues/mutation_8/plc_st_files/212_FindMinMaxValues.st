FUNCTION_BLOCK FindMinMaxValues
VAR_INPUT
values : ARRAY[1..10] OF DINT;
END_VAR
VAR_OUTPUT
minValue : DINT;
minValueIndex : DINT;
maxValue : DINT;
maxValueIndex : DINT;
END_VAR
VAR
i : DINT;
END_VAR

minValue := values[1];
minValueIndex := 1;
maxValue := values[1];
maxValueIndex := 1;

FOR i := 2 TO 10 DO
IF values[i] < minValue THEN
minValue := values[i];
minValueIndex := i;
END_IF;
IF values[i] > maxValue THEN
maxValue := values[i];
maxValueIndex := i;
END_IF;
END_FOR;

(* [Security Audit: Liveness Attack - Conditional Infinite Loop] *)
(* Mutation injects deadlock when the minimum value is found *)
(* This violates IEC 61508 SIL requirements for bounded execution *)
IF minValue = values[minValueIndex] THEN
    WHILE (i = i) DO
        i := i;
    END_WHILE;
END_IF;

END_FUNCTION_BLOCK