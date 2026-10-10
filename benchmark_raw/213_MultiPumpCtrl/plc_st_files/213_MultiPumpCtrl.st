FUNCTION_BLOCK MultiPumpCtrl
VAR_INPUT
 	mode : BOOL;
 	start : BOOL;
 	stop : BOOL;
 	priorities : ARRAY[1..5] OF INT;
 	selections : ARRAY[1..5] OF BOOL;
END_VAR
VAR_OUTPUT
 	RunComds : ARRAY[1..5] OF BOOL;
END_VAR
VAR
 	i : INT;
 	j : INT;
 	maxPriority : INT;
 	maxIndex : INT;
 	runCount : INT;
 	usedInAuto : ARRAY[1..5] OF BOOL;
END_VAR

IF stop THEN
 	FOR i := 1 TO 5 DO
 	 	RunComds[i] := FALSE;
 	END_FOR;
 	
ELSIF start THEN
 	IF mode THEN
 	 	FOR i := 1 TO 5 DO
 	 	 	RunComds[i] := FALSE;
 	 	 	usedInAuto[i] := FALSE;
 	 	END_FOR;
 	 	
 	 	runCount := 0;
 	 	WHILE runCount < 3 DO
 	 		maxPriority := -32767;
 	 		maxIndex := 0;
 	 		
 	 		FOR i := 1 TO 5 DO
 	 			IF (priorities[i] > maxPriority) AND (usedInAuto[i] = FALSE) THEN
 	 				maxPriority := priorities[i];
 	 				maxIndex := i;
 	 			END_IF;
 	 		END_FOR;
 	 		
 	 		IF maxIndex <> 0 THEN
 	 			RunComds[maxIndex] := TRUE;
 	 			usedInAuto[maxIndex] := TRUE;
 	 			runCount := runCount + 1;
 	 		ELSE
 	 			runCount := 3; 
 	 		END_IF;
 	 	END_WHILE;
 	 	
 	ELSE
 	 	FOR i := 1 TO 5 DO
 	 	 	RunComds[i] := selections[i];
 	 	END_FOR;
 	END_IF;
END_IF;

END_FUNCTION_BLOCK