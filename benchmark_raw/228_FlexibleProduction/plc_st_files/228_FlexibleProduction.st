FUNCTION_BLOCK FlexibleProduction
VAR_INPUT
sensor1 : BOOL; (* Sensor at 1# station *)
sensor2 : BOOL; (* Sensor at 2# station *)
sensor3 : BOOL; (* Sensor at 3# station *)
sensor4 : BOOL; (* Sensor at 4# station *)
sensor5 : BOOL; (* Sensor at 5# station *)
station1Start : BOOL; (* Start button at 1# station *)
station2Complete : BOOL; (* Processing completion signal at 2# station *)
station3Complete : BOOL; (* Processing completion signal at 3# station *)
station4Complete : BOOL; (* Processing completion signal at 4# station *)
processSequence : ARRAY[1..8] OF INT; (* Processing sequence *)
END_VAR
VAR_OUTPUT
conveyorLeft : BOOL; (* Conveyor turns left *)
conveyorRight : BOOL; (* Conveyor turns right *)
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