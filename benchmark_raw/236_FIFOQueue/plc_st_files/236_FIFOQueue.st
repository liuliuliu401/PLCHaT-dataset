FUNCTION_BLOCK FIFO
VAR_INPUT
    enqueue : BOOL;
    dequeue : BOOL;
    reset : BOOL;
    clear : BOOL;
    initialItem : LWORD; (* MODIFIED: Replaced Variant with LWORD *)
END_VAR
VAR_IN_OUT
    item : LWORD; (* MOVED & MODIFIED: The item being dequeued/enqueued must be accessible via VAR_IN_OUT or VAR_OUTPUT *)
    buffer : ARRAY[1..10] OF LWORD; (* MOVED & MODIFIED: Buffer must be R/W, placed in VAR_IN_OUT *)
END_VAR
VAR_OUTPUT
    error : BOOL;
    status : WORD;
    elementCount : DINT;
    isEmpty : BOOL;
END_VAR
VAR
    head : DINT := 1;
    tail : DINT := 1;
    capacity : DINT := 10;
    i : DINT; (* MODIFIED: Added loop variable i to VAR block *)
END_VAR

(* The FIFO logic inherently implements a circular buffer. *)


IF reset OR clear THEN (* Combined reset and clear for identical functionality *)
    head := 1;
    tail := 1;
    elementCount := 0;
    isEmpty := TRUE;
    
    FOR i := 1 TO 10 DO
        buffer[i] := initialItem;
    END_FOR;
    
ELSIF enqueue AND (elementCount < capacity) THEN
    buffer[tail] := item;
    
    // Circular increment: 1..10 -> 11 MOD 11 = 0, +1 -> 1. We use MOD 11 for range 1..10.
    tail := (tail MOD capacity) + 1; 
    
    elementCount := elementCount + 1;
    isEmpty := FALSE;
    
    IF elementCount = capacity THEN
        status := 16#8002; // Full
    ELSE
        status := 16#0000;
    END_IF;
    
ELSIF dequeue AND (elementCount > 0) THEN
    item := buffer[head]; // Output the item
    
    // Circular increment: 1..10 -> 11 MOD 11 = 0, +1 -> 1. We use MOD 11 for range 1..10.
    head := (head MOD capacity) + 1;
    
    elementCount := elementCount - 1;
    
    IF elementCount = 0 THEN
        isEmpty := TRUE;
        status := 16#8001; // Empty
    ELSE
        isEmpty := FALSE;
        status := 16#0000;
    END_IF;
    
ELSE
    // Handle cases where nothing happens (e.g., trying to enqueue when full, or dequeue when empty)
    IF elementCount = 0 THEN
        status := 16#8001; // Empty
    ELSIF elementCount = capacity THEN
        status := 16#8002; // Full
    ELSE
        status := 16#0000;
    END_IF;
END_IF;

error := (status <> 16#0000);

END_FUNCTION_BLOCK