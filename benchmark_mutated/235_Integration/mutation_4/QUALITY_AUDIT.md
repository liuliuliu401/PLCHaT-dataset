# Mutation Quality Audit

GRADE: Bronze  
SUMMARY: The mutation survives default tests by exploiting redundant/unused code that does not affect the observable outputs in the tested scenarios.  
DETAILED_ANALYSIS: [  
  - Logic Path: The mutation is extremely shallow—it changes only the initial values of two internal state variables (`previous_integral` and `time_last`), without altering the algorithm’s core logic flow.  
  - Observability: The bug corrupts internal data (integration history and timing state), but if the PLC program does not execute the dependent integration logic during testing, or if the integration loop is reset/reinitialized after the first cycle, the corruption may never propagate to the final output (LinearizedValue).  
  - Verdict Reason: Bronze because the bug survives due to code redundancy or insufficient test coverage of the initialization-dependent path. The detection is not hard; it merely requires a test that exercises the integration function immediately after startup, which is often omitted if tests focus only on steady‑state behavior.  
]