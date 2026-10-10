# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation survives by exploiting the sparse input space where Hundreds > 0 while Thousands equals minValue, making the bug observable only under specific boundary conditions.  

DETAILED_ANALYSIS: [  
- Logic Path: The mutated path is shallow—a single conditional branch where one variable (Hundreds) is replaced by another (Thousands) in a `minimal` function call; however, triggering the bug requires both Hundreds > 0 and Thousands having a value that influences minValue differently than Hundreds, which is a narrow condition.  
- Observability: The bug corrupts the Data (LinearizedValue) directly, because minValue influences the final scaled output; it does not affect Status/Error flags, making detection dependent on exact numeric comparisons in tests.  
- Verdict Reason: The mutation is graded Gold because it survived default tests due to input sparsity—the specific combination where Hundreds and Thousands differ meaningfully while Hundreds > 0 is likely untested; to detect it, a test would need precise boundary values covering this interaction, which is a 'hard-to-trigger' scenario with high potential impact on process values if undetected.  
]