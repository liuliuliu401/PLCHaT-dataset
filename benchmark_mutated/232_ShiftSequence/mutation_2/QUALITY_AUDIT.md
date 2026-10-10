# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation survived due to sparse input space coverage, causing a subtle off-by-one error that corrupts the linearization mapping but remains plausible for most test cases.  

DETAILED_ANALYSIS: [
  - Logic Path: Shallow — mutation is a single loop-bound change, but it resides inside a conditional (shiftLeft) that is likely exercised only for a subset of inputs.
  - Observability: Directly corrupts the Data (LinearizedValue) by shifting one extra position, which may still produce a valid-looking output within range for many inputs.
  - Verdict Reason: Gold because the bug is only observable when `shiftLeft = TRUE` AND the `shiftRange` value aligns so that the extra iteration causes an overflow, wrap-around, or an out-of-bounds effect detectable at specific boundary conditions; default tests likely lacked these precise combinations.
]