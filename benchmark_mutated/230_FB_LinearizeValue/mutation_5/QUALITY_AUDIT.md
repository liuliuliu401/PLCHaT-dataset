# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation introduces a subtle, plausible-looking deviation that corrupts the linearization output only in an extremely narrow input range near the threshold, making it hard to detect with default test coverage.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow but narrow — a nested IF inside the original error-handling path that activates only for inputs between 1199.5 and 1200.0.  
  - Observability: Corrupts the Data (LinearizedValue) by overriding the error-case value (150.0) with 560.0, while keeping Error=TRUE and Status unchanged; plausibly mimics a sensor linearization bug.  
  - Verdict Reason: Gold because survival indicates sparse or inadequate test inputs around that precise boundary; triggering it would require a boundary-value test with sub‑0.5 resolution, which is often omitted in default industrial test suites.  
]