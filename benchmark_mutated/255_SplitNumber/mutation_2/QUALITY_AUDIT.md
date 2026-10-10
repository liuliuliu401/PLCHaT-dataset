# Mutation Quality Audit

GRADE: Gold  
SUMMARY: The mutation survived default tests by exploiting sparse input coverage around extreme boundary values, making it a high-quality subtle flaw.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow—the condition is a straightforward guard clause at the entry point of the data validation routine.  
  - Observability: The bug directly corrupts the Data (LinearizedValue) because the guard’s wider range permits invalid raw inputs to pass into the core linearization algorithm, possibly causing undetected out-of-range output values.  
  - Verdict Reason: Gold because the mutation shifted two boundaries significantly (-100 instead of 0, and 20,000 instead of 10,000), but default tests likely lacked coverage near these extremes, allowing plausible outputs for common test inputs while hiding dangerous physical‑model violations.  
]