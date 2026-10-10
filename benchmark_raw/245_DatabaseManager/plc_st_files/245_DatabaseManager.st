FUNCTION_BLOCK FB_DatabaseManager
   VAR_INPUT 
      dataInput : Array[0..5] of Byte;
      storeTrigger : Bool;
      resetTrigger : Bool;
   END_VAR

   VAR_OUTPUT 
      usedSpace : Int := 0;
      remainingSpace : Int := 20;
      error : Bool;
      status : Word;
   END_VAR

   VAR_IN_OUT 
      dataBase : Array[1..20] of Byte;
   END_VAR

   VAR
      index : Int;
      dataLength : Int;
      i : INT;
   END_VAR

   IF resetTrigger THEN
      FOR i := 1 TO 20 DO
         dataBase[i] := 0;
      END_FOR;
      usedSpace := 0;
      remainingSpace := 20;
      error := FALSE;
      status := 0;
   END_IF;
   
   IF storeTrigger THEN
      dataLength := BYTE_TO_INT(dataInput[0]);
      IF dataLength <= 0 OR dataLength > 5 THEN
         error := TRUE;
         status := 16#8001;
      ELSIF usedSpace + dataLength > 20 THEN
         error := TRUE;
         status := 16#8002;
      ELSE
         FOR index := 1 TO dataLength DO
            dataBase[usedSpace + index] := dataInput[index];
         END_FOR;
         usedSpace := usedSpace + dataLength;
         remainingSpace := 20 - usedSpace;
         error := FALSE;
         status := 0;
      END_IF;
   END_IF;

END_FUNCTION_BLOCK