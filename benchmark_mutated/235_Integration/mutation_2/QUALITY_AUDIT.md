# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation introduces a subtle parametric drift that is hard to detect because typical tests may not exercise the integral over long or varying time intervals sufficiently.  
DETAILED_ANALYSIS: [  
  - Logic Path: Shallow – the mutation is a direct linear scaling in a core arithmetic operation.  
  - Observability: Corrupts the Data (LinearizedValue) – it alters the integral value silently without affecting status or error flags.  
  - Verdict Reason: Gold-grade because detection requires specific test cases that: 1) vary the integration duration significantly, 2) verify numerical accuracy against a known reference, or 3) use dynamic time intervals where the 0.1% scaling becomes non-negligible. Default tests likely miss it due to sparse input space coverage.  
]