FUNCTION_BLOCK DTToString_ISO
VAR_INPUT
    in_date : DT; 
    separator : BYTE; 
END_VAR
VAR_OUTPUT
    ResultString : STRING; 
END_VAR
VAR
    year : INT := 2026;
    month : INT := 1;
    day : INT := 8;
    hour : INT := 10;
    minute : INT := 30;
    second : INT := 0;
    s_year, s_month, s_day, s_hour, s_min, s_sec, s_sep : STRING;
    t1, t2, t3, t4, t5, t6, t7, t8, t9, t10 : STRING;
END_VAR

s_year  := INT_TO_STRING(year);
s_month := INT_TO_STRING(month);
s_day   := INT_TO_STRING(day);
s_hour  := INT_TO_STRING(hour);
s_min   := INT_TO_STRING(minute);
s_sec   := INT_TO_STRING(second);
s_sep   := BYTE_TO_STRING(separator);


(* 构建日期部分: YYYY-MM-DD *)
t1 := CONCAT(s_year, s_sep);
t2 := CONCAT(t1, s_month);
t3 := CONCAT(t2, s_sep);
t4 := CONCAT(t3, s_day);

(* 加上空格 *)
t5 := CONCAT(t4, ' ');

(* 构建时间部分: HH:MM:SS *)
t6 := CONCAT(t5, s_hour);
t7 := CONCAT(t6, ':');
t8 := CONCAT(t7, s_min);
t9 := CONCAT(t8, ':');
t10 := CONCAT(t9, s_sec);

(* 输出最终结果 *)
ResultString := t10;

END_FUNCTION_BLOCK