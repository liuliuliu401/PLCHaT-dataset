# Mutation Quality Audit

GRADE: Bronze  
SUMMARY: This mutation survived because the affected state (`usedInAuto`) is either irrelevant in the tested scenarios or its effects are masked by subsequent logic that overrides it before being observed.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow; only a single Boolean assignment inside a loop changes, within a conditional block that runs at startup.  
  - Observability: The bug corrupts Metadata (`usedInAuto` flags), not actual process data (`RunComds` stays unchanged); these flags might only be used in secondary logic (e.g., mode-change safety checks) that wasn’t exercised in the default test suite.  
  - Verdict Reason: Bronze because the mutation is not "hard to trigger" — any test entering the `IF mode` branch with `start = TRUE` would expose it, but the default tests likely omitted that path or the flags' downstream usage. This suggests redundancy or insufficient coverage, not subtlety.  
]