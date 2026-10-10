FUNCTION_BLOCK LightsControl
VAR_INPUT
    button1 : BOOL; 
    button2 : BOOL; 
    button3 : BOOL; 
END_VAR
VAR_OUTPUT
    greenLight : BOOL; 
    redLight : BOOL; 
    yellowLight : BOOL; 
END_VAR
VAR
    (* 使用标准库 TON *)
    timerAuto : TON;
    timerGreen : TON;
    timerRed : TON;
    timerYellow : TON;
    currentState : INT := 0;
    autoRunTimer : BOOL;
END_VAR

IF button1 THEN 
    autoRunTimer := FALSE;
    CASE currentState OF
        0: 
            greenLight := TRUE;
            redLight := FALSE;
            yellowLight := FALSE;
            autoRunTimer := TRUE;
            IF timerAuto.Q THEN
                currentState := 1;
            END_IF;
        1: 
            greenLight := FALSE;
            redLight := TRUE;
            yellowLight := FALSE;
            autoRunTimer := TRUE;
            IF timerAuto.Q THEN
                currentState := 2;
            END_IF;
        2: 
            greenLight := FALSE;
            redLight := FALSE;
            yellowLight := TRUE;
            autoRunTimer := TRUE;
            IF timerAuto.Q THEN
                currentState := 0;
            END_IF;
    END_CASE;


    timerAuto(IN := autoRunTimer, PT := T#1s);

ELSE 
    timerAuto(IN := FALSE);
    
    greenLight := FALSE;
    redLight := FALSE;
    yellowLight := FALSE;

    IF button2 AND NOT button3 THEN
        greenLight := TRUE;
        timerGreen(IN := TRUE, PT := T#2s); 
        redLight := timerGreen.Q;
        timerRed(IN := FALSE);
        timerYellow(IN := FALSE);
    ELSIF NOT button2 AND button3 THEN
        redLight := TRUE;
        timerRed(IN := TRUE, PT := T#1s);
        yellowLight := NOT timerRed.Q;
        timerGreen(IN := FALSE);
        timerYellow(IN := FALSE);
    ELSIF button2 AND button3 THEN
        yellowLight := TRUE;
        timerYellow(IN := TRUE, PT := T#1s);
        greenLight := NOT timerYellow.Q;
        timerGreen(IN := FALSE);
        timerRed(IN := FALSE);
    ELSE
        timerGreen(IN := FALSE);
        timerRed(IN := FALSE);
        timerYellow(IN := FALSE);
    END_IF;
END_IF;

END_FUNCTION_BLOCK