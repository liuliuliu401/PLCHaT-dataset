# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation is trivially detected when any input value is non-zero, but it survives with a specific zero-only test suite.  
DETAILED_ANALYSIS: [  
  - Logic Path: The mutated path is shallow—initialization of min/max variables is replaced with constant zero.  
  - Observability: The bug corrupts the Data (LinearizedValue), because minValue and maxValue directly influence scaling or validation results downstream.  
  - Verdict Reason: The mutation survives only when the entire input array `values` is zero (or when subsequent logic ignores these initial values). If tests happen to use only zero inputs, the deviation is missed; any non-zero input triggers failure. This makes detection straightforward for any non-zero test case, placing it at Silver (requires a precise boundary value—zero—to be missed, not just any input).  
]