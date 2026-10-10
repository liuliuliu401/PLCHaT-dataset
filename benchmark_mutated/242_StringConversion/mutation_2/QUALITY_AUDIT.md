# Mutation Quality Audit

GRADE: Gold  
SUMMARY: The mutation introduces a subtle offset drift that plausible output values survive default tests, requiring targeted boundary-value tests to detect.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow—single assignment in linearization or encoding logic, but changes a core arithmetic parameter.
  - Observability: Corrupts Data (LinearizedValue) by shifting character mapping (e.g., ASCII case conversion), yet outputs remain within valid numeric ranges, avoiding immediate error flags.
  - Verdict Reason: Gold because detection demands precise input–output pairs (e.g., specific ASCII characters) to expose the deviation; sparse default tests likely omitted such edge cases, making the bug survive despite significant functional impact.
]