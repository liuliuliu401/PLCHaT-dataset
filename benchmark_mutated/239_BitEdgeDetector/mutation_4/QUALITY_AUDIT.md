# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation inverts the edge detection logic (rising vs falling) which can survive generic input tests but is easily caught if a specific input transition is exercised.  

DETAILED_ANALYSIS: [
  - Logic Path: Shallow—the change directly replaces the Boolean expression for detecting rising edges with an incorrect formula, but the structure of the rest of the edge‑detection algorithm is unchanged.
  - Observability: It corrupts the Data (specifically the `risingBits` mask), which could cause downstream logic (like alarms or control actions) to respond incorrectly on real‑world edge transitions.
  - Verdict Reason: Silver because the bug is not dependent on rare boundary values (Gold), but it does require a precise input sequence (a bit transition from 0→1 or 1→0) to be exposed. Default tests that lack such transitions may miss it, while tests that systematically check edge‑detection would catch it immediately—hence moderately difficult to trigger, but not extremely subtle.
]