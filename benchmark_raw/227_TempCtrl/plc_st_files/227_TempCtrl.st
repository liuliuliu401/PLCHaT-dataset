FUNCTION_BLOCK TempCtrl
VAR_INPUT
    TemperatureSensorInput : REAL; 
    SetTemperature : REAL; 
END_VAR
VAR_OUTPUT
    CurrentTemperature : REAL; 
    SetTempDisplay : REAL; 
    HeaterStatus : BOOL; 
    OverheatProtection : BOOL; 
END_VAR
VAR_IN_OUT
    HeaterOutput : BOOL; 
END_VAR
VAR
    TimerHeater : TON; 
    TimerOverheat : TON; 
    LastHeaterState : BOOL; 
END_VAR

CurrentTemperature := TemperatureSensorInput;
SetTempDisplay := SetTemperature;

IF TemperatureSensorInput < 0.0 OR TemperatureSensorInput > 100.0 THEN
    HeaterOutput := FALSE;
    HeaterStatus := FALSE;
    OverheatProtection := FALSE;
    TimerHeater(IN := FALSE);
    TimerOverheat(IN := FALSE);
ELSE
    IF TemperatureSensorInput < SetTemperature THEN
        IF NOT OverheatProtection THEN
            HeaterOutput := TRUE;
            HeaterStatus := TRUE;
            TimerHeater(IN := TRUE, PT := T#5s);
        END_IF;
    ELSE
        HeaterOutput := FALSE;
        HeaterStatus := FALSE;
        TimerHeater(IN := FALSE);
    END_IF;

    IF TimerHeater.Q THEN
        HeaterOutput := FALSE;
        HeaterStatus := FALSE;
        OverheatProtection := TRUE;
        TimerOverheat(IN := TRUE, PT := T#3s);
    END_IF;

    IF TimerOverheat.Q THEN
        OverheatProtection := FALSE;
        TimerOverheat(IN := FALSE);
    END_IF;
    
    TimerHeater();
    TimerOverheat();
END_IF;

LastHeaterState := HeaterOutput;

END_FUNCTION_BLOCK