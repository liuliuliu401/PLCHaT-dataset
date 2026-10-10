# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation changes a boundary check from '>0' to '>1', causing mis-handling of the exact Thousands=1 case, which is detectable by default tests.  

DETAILED_ANALYSIS: [
  - Logic Path: Shallow — a single conditional predicate change, directly affecting the `minValue` calculation path.
  - Observability: Corrupts Data (`LinearizedValue`) because `minValue` influences the final scaling computation.
  - Verdict Reason: While this is a boundary‑value bug, default tests caught it (TRIGGERED). It did not require sparse or hidden input combinations—any test with Thousands=1 would expose it. Hence it is Silver, not Gold (Gold would need subtle corner‑cases missed by ordinary coverage). The impact is moderate: for Thousands=1, `minValue` will incorrectly ignore that digit, potentially shifting the linearized output.  
]