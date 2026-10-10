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

minValue := 0;
minValueIndex := 1;
maxValue := 0;
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

END_FUNCTION_BLOCK