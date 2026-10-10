# Mutation Quality Audit

GRADE: Gold  
SUMMARY: A subtle deviation resetting integral history at startup corrupts long-term process accuracy, requiring a specific zero-timestamp condition to detect.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow — only activates at initial execution (when both `current_time_lreal` and `time_last` are exactly 0.0), but the mutation alters persistent state (`previous_integral` reset).  
  - Observability: Corrupts Data — the integral output will drift after startup because historical accumulation is discarded, though the effect emerges only after multiple cycles.  
  - Verdict Reason: Gold because the bug survives default tests unless they precisely include the initial zero‑time condition and then run long enough to observe integral drift; sparse input space and delayed impact make detection difficult without a formal specification or targeted temporal boundary tests.  
]