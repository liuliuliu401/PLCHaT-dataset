TYPE T_GARAGE_SLOT:
    STRUCT
        Occupied : BOOL;
        VehicleNumber : INT;
    END_STRUCT;
END_TYPE

TYPE T_TRUCK_INFO:
    STRUCT
        row : INT;
        col : INT;
        VehicleNumber : INT;
    END_STRUCT;
END_TYPE

FUNCTION_BLOCK TruckGarage
VAR_INPUT
    enter_flag : BOOL;     
    exit_flag : BOOL;      
    VehicleNumber : INT;
END_VAR

VAR_OUTPUT
    truck : T_TRUCK_INFO;
END_VAR

VAR_IN_OUT
    garage : ARRAY[1..3, 1..5] OF T_GARAGE_SLOT;
END_VAR

VAR
    i : INT;
    j : INT;
END_VAR

(*
    Entry Logic: Latter Row First
*)
IF enter_flag THEN (* Use renamed variable *)
    FOR i := 3 TO 1 BY -1 DO
        FOR j := 5 TO 1 BY -1 DO
            IF NOT garage[i, j].Occupied THEN
                garage[i, j].Occupied := TRUE;
                garage[i, j].VehicleNumber := VehicleNumber;
                EXIT; (* Re-introducing the EXIT keyword *)
            END_IF;
        END_FOR;
    END_FOR;
END_IF;


IF exit_flag THEN (* Use renamed variable *)
    FOR i := 1 TO 3 DO
        FOR j := 1 TO 5 DO
            IF garage[i, j].Occupied THEN
                
                truck.row := i;
                truck.col := j;
                truck.VehicleNumber := garage[i, j].VehicleNumber;
                
                garage[i, j].Occupied := FALSE;
                garage[i, j].VehicleNumber := 0;
                
                EXIT; 
            END_IF;
        END_FOR;
    END_FOR;
END_IF;

END_FUNCTION_BLOCK