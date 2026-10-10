FUNCTION_BLOCK SplitByteArray
VAR_INPUT
byteArray : ARRAY[1..10] OF BYTE;
bitArray : ARRAY[1..80] OF BOOL;
END_VAR
VAR_OUTPUT
splitByteNum : DINT;
END_VAR
VAR
i, j, k : INT := 0;
tmpByte : BYTE;
END_VAR

splitByteNum := 0;
k := 0;
FOR i := 1 TO 10 DO
IF k >= 80 THEN
RETURN;
END_IF;
tmpByte := byteArray[i];
FOR j := 0 TO 7 DO
IF k >= 80 THEN
RETURN;
END_IF;
bitArray[k + 1] := (tmpByte AND 1) = 1;
tmpByte := SHR(tmpByte, 1);
k := k + 1;
END_FOR;
splitByteNum := splitByteNum + 1;
END_FOR;
END_FUNCTION_BLOCK