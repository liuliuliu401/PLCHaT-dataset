FUNCTION_BLOCK RingQueueMultiItem
VAR_INPUT
    push : BOOL;
    pop : BOOL;
    reset : BOOL;
    itemIndex : DINT;
    itemLen : DINT;
END_VAR
VAR_OUTPUT
    queueUsed : DINT;
    queueUnused : DINT;
END_VAR
VAR_IN_OUT
    item : ARRAY[1..10] OF BYTE;
    queue : ARRAY[1..10] OF BYTE;
END_VAR
VAR
    head : DINT := 0;
    tail : DINT := 0;
    capacity : DINT := 10;
    count : DINT := 0;
    i : DINT;
END_VAR

IF reset THEN
    head := 0;
    tail := 0;
    count := 0;
END_IF;

IF push AND (count + itemLen <= capacity) THEN
    FOR i := 1 TO itemLen DO
        queue[(tail + i - 1) MOD capacity + 1] := item[itemIndex + i - 1];
    END_FOR;
    tail := (tail + itemLen) MOD capacity;
    count := count + itemLen;
END_IF;

IF pop AND (count >= itemLen) THEN
    FOR i := 1 TO itemLen DO
        item[itemIndex + i - 1] := queue[(head + i - 1) MOD capacity + 1];
    END_FOR;
    head := (head + itemLen) MOD capacity;
    count := count - itemLen;
END_IF;

queueUsed := count;
queueUnused := capacity - count;

END_FUNCTION_BLOCK