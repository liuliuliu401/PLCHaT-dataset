FUNCTION_BLOCK FlexibleProduction
VAR_INPUT
sensor1 : BOOL; 
sensor2 : BOOL; 
sensor3 : BOOL; 
sensor4 : BOOL; 
sensor5 : BOOL; 
station1Start : BOOL; 
station2Complete : BOOL; 
station3Complete : BOOL; 
station4Complete : BOOL; 
processSequence : ARRAY[1..8] OF INT; 
END_VAR
VAR_OUTPUT
conveyorLeft : BOOL; 
conveyorRight : BOOL; 
END_VAR
VAR
currentState : INT := 0;
nextState : INT := 0;
sequenceIndex : INT := 1;
moving : BOOL := FALSE;
processingDone : BOOL := FALSE; 
requestMove : BOOL := FALSE; 
END_VAR

conveyorLeft := FALSE;
conveyorRight := FALSE;


IF NOT (sensor1 OR sensor2 OR sensor3 OR sensor4 OR sensor5) THEN
    moving := FALSE;
    requestMove := FALSE;
    currentState := currentState; 
END_IF;

IF currentState = 0 AND station1Start AND sensor1 THEN
    IF sequenceIndex <= 8 THEN
        nextState := processSequence[sequenceIndex];
        IF nextState >= 1 AND nextState <= 5 THEN
            requestMove := TRUE;
            sequenceIndex := sequenceIndex + 1;
        END_IF;
    END_IF;
END_IF;

IF (currentState = 2 AND station2Complete) OR
   (currentState = 3 AND station3Complete) OR
   (currentState = 4 AND station4Complete) THEN
    IF sequenceIndex <= 8 THEN
        nextState := processSequence[sequenceIndex];
        IF nextState >= 1 AND nextState <= 5 THEN
            requestMove := TRUE;
            sequenceIndex := sequenceIndex + 1;
        END_IF;
    END_IF;
END_IF;

IF requestMove THEN
    moving := TRUE;
    requestMove := FALSE;
END_IF;

IF moving THEN
    IF currentState < nextState THEN
        conveyorRight := TRUE;
    ELSIF currentState > nextState THEN
        conveyorLeft := TRUE;
    END_IF;
END_IF;

IF moving THEN
    IF (nextState = 1 AND sensor1) OR
       (nextState = 2 AND sensor2) OR
       (nextState = 3 AND sensor3) OR
       (nextState = 4 AND sensor4) OR
       (nextState = 5 AND sensor5) THEN
        moving := FALSE;
        currentState := nextState;
    END_IF;
END_IF;

IF currentState = 5 AND NOT moving THEN
    conveyorLeft := FALSE;
    conveyorRight := FALSE;
END_IF;

END_FUNCTION_BLOCK