FUNCTION_BLOCK MaterialMixing
VAR_INPUT
estop : BOOL; 
opeMode : BOOL; 
valveAStart : BOOL; 
valveBStart : BOOL; 
valveCStart : BOOL; 
valveDStart : BOOL; 
mixMotorStart : BOOL; 
autoStart : BOOL; 
processMode : BOOL; 
levelSensor : INT; 
END_VAR

VAR_OUTPUT
valveARun : BOOL; 
valveBRun : BOOL; 
valveCRun : BOOL; 
valveDRun : BOOL; 
mixMotorRun : BOOL; 
END_VAR

VAR
previousValveAStart : BOOL := FALSE;
previousValveBStart : BOOL := FALSE;
previousValveCStart : BOOL := FALSE;
previousValveDStart : BOOL := FALSE;
previousMixMotorStart : BOOL := FALSE;
previousAutoStart : BOOL := FALSE;
autoStep : INT := 0; 
mixMotorComplete : BOOL; 


valveA_RE : BOOL;
valveB_RE : BOOL;
valveC_RE : BOOL;
valveD_RE : BOOL;
mixMotor_RE : BOOL;
autoStart_RE : BOOL;
END_VAR




IF NOT estop THEN
    
    valveARun := FALSE;
    valveBRun := FALSE;
    valveCRun := FALSE;
    valveDRun := FALSE;
    mixMotorRun := FALSE;
    autoStep := 0;
ELSE
    
    IF NOT opeMode THEN
        

        autoStep := 0;

        
        valveA_RE := valveAStart AND NOT previousValveAStart;
        valveB_RE := valveBStart AND NOT previousValveBStart;
        valveC_RE := valveCStart AND NOT previousValveCStart;
        valveD_RE := valveDStart AND NOT previousValveDStart;
        mixMotor_RE := mixMotorStart AND NOT previousMixMotorStart;
        
        
        IF valveA_RE THEN valveARun := NOT valveARun; END_IF;
        IF valveB_RE THEN valveBRun := NOT valveBRun; END_IF;
        IF valveC_RE THEN valveCRun := NOT valveCRun; END_IF;
        IF valveD_RE THEN valveDRun := NOT valveDRun; END_IF;
        IF mixMotor_RE THEN mixMotorRun := NOT mixMotorRun; END_IF;

    ELSE
        

        autoStart_RE := autoStart AND NOT previousAutoStart;
        
        
        valveARun := FALSE;
        valveBRun := FALSE;
        valveCRun := FALSE;
        valveDRun := FALSE;
        mixMotorRun := FALSE;
        
        
        IF autoStep < 50 THEN 
            IF levelSensor >= 100 AND processMode THEN
                
                autoStep := 50; 
            ELSIF levelSensor >= 80 THEN
                
                IF NOT processMode THEN
                    autoStep := 50;
                ELSE
                    autoStep := 40;
                    valveCRun := TRUE; 
                END_IF;
            ELSIF levelSensor >= 50 THEN
                
                autoStep := 20; 
                valveBRun := TRUE;  
                valveARun := FALSE; 
            END_IF;
            
            levelSensor := levelSensor + (autoStep / 10);
        END_IF;

        
        CASE autoStep OF
            0: 
                IF autoStart_RE THEN
                    autoStep := 10; 
                END_IF;
            
            10: 
                IF levelSensor >= 50 THEN
                    autoStep := 20; 
                    valveBRun := TRUE;  
                    valveARun := FALSE; 
                ELSE
                    valveARun := TRUE;
                END_IF;

            20: 
                IF levelSensor >= 80 THEN
                    autoStep := 30; 
                    valveBRun := FALSE; 
                ELSE
                    valveBRun := TRUE;
                END_IF;

            30: 
                IF NOT processMode THEN
                    valveCRun := FALSE;
                    autoStep := 50; 
                ELSE
                    valveCRun := TRUE;
                    autoStep := 40; 
                END_IF;

            40: 
                IF levelSensor >= 100 THEN
                    valveCRun := FALSE;
                    autoStep := 50; 
                ELSE
                    valveCRun := TRUE;
                END_IF;

            50: 
                IF mixMotorComplete THEN
                    mixMotorRun := FALSE; 
                    autoStep := 60; 
                ELSE
                    mixMotorRun := TRUE;
                END_IF;

            60: 
                valveDRun := TRUE;
                IF NOT autoStart THEN
                    autoStep := 0; 
                END_IF;

            ELSE
                autoStep := 0; 
        END_CASE;

    END_IF;
    
    
    previousValveAStart := valveAStart;
    previousValveBStart := valveBStart;
    previousValveCStart := valveCStart;
    previousValveDStart := valveDStart;
    previousMixMotorStart := mixMotorStart;
    previousAutoStart := autoStart;

END_IF;
END_FUNCTION_BLOCK