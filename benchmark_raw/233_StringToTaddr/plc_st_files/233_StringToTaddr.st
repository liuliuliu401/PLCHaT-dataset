TYPE TADDR_Param :
STRUCT
    ipAddr : DINT;
    port : INT;
END_STRUCT;
END_TYPE

FUNCTION_BLOCK StringToTaddr
VAR_INPUT
    ipAddressString : STRING;
END_VAR
VAR_OUTPUT
    error : BOOL;
    status : WORD;
    returnValue : TADDR_Param;
END_VAR
VAR
    ipParts : ARRAY[1..4] OF INT;
    portNumber : INT := 0;
    tempStatus : WORD := 16#0000;
    tempError : BOOL := FALSE;
    i : INT := 0;
    findPos : INT; 
END_VAR

returnValue.ipAddr := 0;
returnValue.port := 0;

findPos := FIND(ipAddressString, ':');

IF findPos > 0 THEN 
    portNumber := STRING_TO_INT(LEFT(ipAddressString, findPos - 1));
    ipAddressString := MID(ipAddressString, LEN(ipAddressString) - findPos, findPos + 1);
ELSE
    portNumber := 0;
END_IF;

IF (portNumber < 0) THEN
    tempStatus := 16#8152;
    tempError := TRUE;
ELSIF portNumber = 0 THEN
    tempStatus := 16#8151;
    tempError := TRUE;
END_IF;

i := 1;
WHILE i <= 4 DO
    findPos := FIND(ipAddressString, '.');
    IF findPos > 0 THEN
        ipParts[i] := STRING_TO_INT(MID(ipAddressString, findPos - 1, 1));
        ipAddressString := MID(ipAddressString, LEN(ipAddressString) - findPos, findPos + 1);
    ELSE
        ipParts[i] := STRING_TO_INT(ipAddressString);
    END_IF;
    i := i + 1;
END_WHILE;

FOR i := 1 TO 4 DO
    IF (ipParts[i] < 0) OR (ipParts[i] > 255) THEN
        tempStatus := INT_TO_WORD(WORD_TO_INT(16#8130) + i);
        tempError := TRUE;
    END_IF;
END_FOR;

IF NOT tempError THEN
    returnValue.ipAddr := INT_TO_DINT(ipParts[1]) * 16777216 + 
                          INT_TO_DINT(ipParts[2]) * 65536 + 
                          INT_TO_DINT(ipParts[3]) * 256 + 
                          INT_TO_DINT(ipParts[4]);
    returnValue.port := portNumber;
END_IF;

error := tempError;
status := tempStatus;

END_FUNCTION_BLOCK