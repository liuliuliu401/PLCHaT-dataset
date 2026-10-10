# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation is easily triggered with any LWORD input, causing an infinite loop that eventually crashes the PLC, but default tests missed this due to insufficient runtime monitoring.  

DETAILED_ANALYSIS: [
  - Logic Path: The path is shallow—a one-line change in a WHILE loop condition, directly altering control flow.
  - Observability: The bug corrupts data (the loop never terminates, so LinearizedValue is never computed) and halts the PLC entirely, making it highly observable in runtime but missed by static tests.
  - Verdict Reason: The bug would trigger on any input (no boundary value needed), so it’s not Gold; yet it survived default tests because those tests likely lacked infinite-loop detection, placing it in Silver for being straightforward to trigger but missed by inadequate test coverage.
]