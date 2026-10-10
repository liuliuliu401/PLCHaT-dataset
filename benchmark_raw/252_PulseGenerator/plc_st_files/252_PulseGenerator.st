FUNCTION_BLOCK PulseGenerator
VAR_INPUT
    frequency : REAL; (* Clock frequency in Hz *)
    pulsePauseRatio : REAL; (* Pulse pause ratio *)
END_VAR
VAR_OUTPUT
    pulse : BOOL; (* Pulse output *)
    remainingTime : TIME; (* Remaining time of the current state *)
END_VAR
VAR
    internalState : BOOL := FALSE;
    currentDuration : TIME := T#0s;
    totalCycleTime : TIME;
    trueDuration : TIME;
    falseDuration : TIME;
    
    (* 中间计算变量 *)
    total_ms : REAL;
    true_ms : REAL;
END_VAR

(* 1. 计算总周期时间 *)
IF frequency > 0.0 THEN
    (* 周期(s) = 1 / 频率, 换算成毫秒(ms) 则为 1000 / frequency *)
    total_ms := 1000.0 / frequency;
    totalCycleTime := REAL_TO_TIME(total_ms);
ELSE
    totalCycleTime := T#0s;
    total_ms := 0.0;
END_IF;

(* 2. 计算高电平(true)和低电平(false)持续时间 *)
IF pulsePauseRatio > 0.0 AND total_ms > 0.0 THEN
    (* trueDuration = total * (ratio / (1 + ratio)) *)
    true_ms := total_ms * (pulsePauseRatio / (1.0 + pulsePauseRatio));
    trueDuration := REAL_TO_TIME(true_ms);
    falseDuration := totalCycleTime - trueDuration;
ELSE
    trueDuration := T#0s;
    falseDuration := totalCycleTime;
END_IF;

(* 3. 状态切换逻辑 *)
IF internalState THEN
    IF currentDuration < trueDuration THEN
        (* 模拟扫描周期增加时间，注意：实际应用中通常使用系统时间差 *)
        currentDuration := currentDuration + T#1ms;
        remainingTime := trueDuration - currentDuration;
    ELSE
        internalState := FALSE;
        currentDuration := T#0s;
        remainingTime := falseDuration;
    END_IF;
ELSE
    IF currentDuration < falseDuration THEN
        currentDuration := currentDuration + T#1ms;
        remainingTime := falseDuration - currentDuration;
    ELSE
        internalState := TRUE;
        currentDuration := T#0s;
        remainingTime := trueDuration;
    END_IF;
END_IF;

(* 4. 设置脉冲输出 *)
pulse := internalState;

END_FUNCTION_BLOCK