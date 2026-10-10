# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation survives because it only affects a narrow set of inputs where multiple identical textAfter characters exist, altering the program's selection to the *last* occurrence rather than the first, which silently corrupts the process-critical indexing without causing immediate failures.  
DETAILED_ANALYSIS: [  
  - Logic Path: Shallow but subtle—the loop continues instead of exiting early, changing the final afterIndex only if textAfter appears more than once after textBefore.  
  - Observability: Corrupts Data (LinearizedValue) indirectly by shifting the string boundary used for parsing, potentially causing downstream miscalculations (e.g., wrong numeric extraction) while status flags may remain plausible.  
  - Verdict Reason: Gold-grade detection difficulty arises because standard tests likely use unique delimiters or short strings, so the bug requires a very specific input pattern (multiple identical delimiters after the target position) that is rare in validation suites but physically possible in runtime.  
]