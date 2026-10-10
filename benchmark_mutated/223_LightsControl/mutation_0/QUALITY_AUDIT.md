# Mutation Quality Audit

GRADE: Bronze  
SUMMARY: The mutation introduces a concurrent red/green violation that is blatantly contradictory and should be easily detectable by any comprehensive functional test.  
DETAILED_ANALYSIS: [
  - Logic Path: The mutation adds an extra assignment (redLight := TRUE) in the same rung immediately after setting greenLight := TRUE, creating an inconsistent physical state (both red and green lights ON simultaneously) that violates basic safety rules. The path is shallow—no nesting or complex branching.
  - Observability: The bug corrupts the Data (actuator outputs) directly and egregiously, not just Metadata. The inconsistency is immediately observable in the output image.
  - Verdict Reason: This is a Bronze-grade mutation because it survived due to redundancy or omission in the default test suite, not because the defect is subtle or hard to trigger. Any test that validates mutually exclusive light states would catch it instantly, implying the test suite is incomplete rather than the bug being sophisticated.
]