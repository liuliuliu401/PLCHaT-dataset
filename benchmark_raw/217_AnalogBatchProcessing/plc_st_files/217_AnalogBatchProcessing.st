FUNCTION_BLOCK AnalogBatchProcessing
VAR_INPUT
    count : INT;
    analogValues : ARRAY[1..10] OF REAL;
END_VAR
VAR_OUTPUT
    channel_enable : ARRAY[1..10] OF BOOL;
    channel_hiLIM : ARRAY[1..10] OF REAL;
    channel_loLIM : ARRAY[1..10] OF REAL;
    channel_bipolar : ARRAY[1..10] OF BOOL;
    channel_measuringMode : ARRAY[1..10] OF INT;
    
    channel_retVal : ARRAY[1..10] OF WORD;
    channel_outputValue : ARRAY[1..10] OF REAL;
END_VAR
VAR
    i : INT;
    loop_end : INT; 
    
    C_MAX_ADC_UNIPOLAR : REAL := 27648.0; 
    C_MAX_ADC_BIPOLAR_POS : REAL := 27648.0;
    C_MAX_ADC_BIPOLAR_NEG : REAL := -27648.0;
    C_BIPOLAR_RANGE : REAL := 55296.0;
END_VAR

loop_end := count;

IF loop_end > 10 THEN
    loop_end := 10;
END_IF;

IF loop_end < 1 THEN
    loop_end := 0; 
END_IF; 

FOR i := 1 TO loop_end DO

    IF channel_enable[i] THEN
        channel_retVal[i] := 0; 
        IF channel_hiLIM[i] <= channel_loLIM[i] THEN
            channel_retVal[i] := 16#8002;
        ELSE
            CASE channel_measuringMode[i] OF
                0:
                    IF channel_bipolar[i] THEN
                        IF analogValues[i] >= C_MAX_ADC_BIPOLAR_NEG AND analogValues[i] <= C_MAX_ADC_BIPOLAR_POS THEN
                            channel_outputValue[i] := (channel_hiLIM[i] - channel_loLIM[i]) / C_BIPOLAR_RANGE * (analogValues[i] - C_MAX_ADC_BIPOLAR_NEG) + channel_loLIM[i];
                        ELSE
                            channel_retVal[i] := 16#8003;
                        END_IF;
                    ELSE
                        IF analogValues[i] >= 0.0 AND analogValues[i] <= C_MAX_ADC_UNIPOLAR THEN
                            channel_outputValue[i] := (channel_hiLIM[i] - channel_loLIM[i]) / C_MAX_ADC_UNIPOLAR * analogValues[i] + channel_loLIM[i];
                        ELSE
                            channel_retVal[i] := 16#8003;
                        END_IF;
                    END_IF;
                
                1:
                    channel_outputValue[i] := analogValues[i] / 10.0;
                
                2:
                    channel_outputValue[i] := analogValues[i] / 100.0;
                
                ELSE
                    channel_retVal[i] := 16#8001;
            END_CASE;

        END_IF;
    END_IF;
END_FOR;

END_FUNCTION_BLOCK