FUNCTION_BLOCK ConveyorSystem
VAR_INPUT
    StartButton : BOOL;
    SensorStart : BOOL;
    Sensor1 : BOOL;
    Sensor2 : BOOL;
    Sensor3 : BOOL;
    UnloadComplete1 : BOOL;
    UnloadComplete2 : BOOL;
    UnloadComplete3 : BOOL;
    PartSpec : INT;
    PartNumber : INT;
END_VAR
VAR_OUTPUT
    ConveyorRun : BOOL;
    WarehouseDatabase1 : ARRAY[1..10] OF INT;
    WarehouseDatabase2 : ARRAY[1..10] OF INT;
    WarehouseDatabase3 : ARRAY[1..10] OF INT;
END_VAR
VAR
    CurrentIndex1 : INT := 1;
    CurrentIndex2 : INT := 1;
    CurrentIndex3 : INT := 1;
    WaitState : BOOL := FALSE;
END_VAR

(* 修正：PLC 严禁使用阻塞式 WHILE 循环，改用状态逻辑 *)
IF StartButton AND SensorStart THEN
    ConveyorRun := TRUE;
    
    IF PartSpec = 1 THEN
        (* 只有当传感器触发且卸载完成时才记录 *)
        IF Sensor1 AND UnloadComplete1 THEN
            WarehouseDatabase1[CurrentIndex1] := PartNumber;
            CurrentIndex1 := CurrentIndex1 + 1;
            IF CurrentIndex1 > 10 THEN
                CurrentIndex1 := 1;
            END_IF;
            ConveyorRun := FALSE;
        END_IF;

    ELSIF PartSpec = 2 THEN
        IF Sensor2 AND UnloadComplete2 THEN
            WarehouseDatabase2[CurrentIndex2] := PartNumber;
            CurrentIndex2 := CurrentIndex2 + 1;
            IF CurrentIndex2 > 10 THEN
                CurrentIndex2 := 1;
            END_IF;
            ConveyorRun := FALSE;
        END_IF;

    ELSIF PartSpec = 3 THEN
        IF Sensor3 AND UnloadComplete3 THEN
            WarehouseDatabase3[CurrentIndex3] := PartNumber;
            CurrentIndex3 := CurrentIndex3 + 1;
            IF CurrentIndex3 > 10 THEN
                CurrentIndex3 := 1;
            END_IF;
            ConveyorRun := FALSE;
        END_IF;
    END_IF;
    
ELSE
    ConveyorRun := FALSE;
END_IF;

END_FUNCTION_BLOCK