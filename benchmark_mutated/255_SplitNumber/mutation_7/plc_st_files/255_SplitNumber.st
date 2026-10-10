FUNCTION_BLOCK FB_SplitNumber
VAR_INPUT 
InputNumber : INT; 
END_VAR

VAR_OUTPUT 
Thousands : INT;
Hundreds : INT;
Tens : INT;
Ones : INT;
OutMin : INT;
error : BOOL; 
status : WORD; 
END_VAR

VAR 
tempNumber : INT; 
minValue : INT;
END_VAR

// Check if the input number is within the valid range
IF (InputNumber < 0) OR (InputNumber >= 10000) THEN
error := TRUE;
status := 16#8001;
// Set the outputs to default values
Thousands := 0;
Hundreds := 0;
Tens := 0;
Ones := 0;
OutMin := 0;
ELSE
// The input number is valid
error := FALSE;
status := 0;

// Split the number into thousands, hundreds, tens and ones
tempNumber := InputNumber;
Thousands := tempNumber / 1000;
tempNumber := tempNumber MOD 1000;
Hundreds := tempNumber / 100;
tempNumber := tempNumber MOD 100;
Tens := tempNumber / 10;
Ones := tempNumber MOD 10;

// Find the minimum non-zero value
minValue := 32767;
IF Thousands > 0 THEN minValue := minimal(IN1:=minValue, IN2:=Thousands); END_IF;
IF Hundreds > 0 THEN minValue := minimal(IN1:=minValue, IN2:=Hundreds); END_IF;
IF Tens > 0 THEN minValue := minimal(IN1:=minValue, IN2:=Tens); END_IF;
IF Ones > 0 THEN minValue := minimal(IN1:=minValue, IN2:=Ones); END_IF;

// If all digits are zero, set OutMin to zero
IF minValue = 32767 THEN minValue := 0; END_IF;

OutMin := minValue;
END_IF;
END_FUNCTION_BLOCK

FUNCTION minimal : INT
VAR_INPUT 
IN1 : INT;
IN2 : INT;
END_VAR

IF IN1 <= IN2 THEN
minimal := IN1;
ELSE
minimal := IN2;
END_IF;
END_FUNCTION