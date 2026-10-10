# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation alters the error-status mapping while preserving the primary linearization calculation, making it detectable only by validating metadata against expected process conditions.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow; the mutation is within a single conditional branch, not nested or structurally complex.
  - Observability: The bug corrupts Metadata (Error and Status) while the Data (LinearizedValue) remains correct; thus, it doesn't affect the primary process value directly.
  - Verdict Reason: The default tests survived because typical unit tests likely focus on validating the linearized output values, ignoring error flags or status codes. Detection requires explicit checks of the metadata (Error/Status) for this specific input range, which is straightforward but not automatically covered by basic output-value tests—hence Silver (requires specific input but not rare boundary conditions).
]