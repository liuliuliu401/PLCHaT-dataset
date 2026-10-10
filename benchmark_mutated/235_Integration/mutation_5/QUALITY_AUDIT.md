# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation introduces a plausible time-based anomaly that corrupts the integral's internal state only under large time steps, which is likely untested in typical unit scenarios.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow — The mutation adds a simple conditional that resets the integral accumulator if `time_diff > 10.0`, but does not alter the primary output assignment directly.
  - Observability: Corrupts Data (LinearizedValue) — The `outIntegral` is set before the conditional reset, so immediate output is unchanged, but the internal `previous_integral` is zeroed, causing future integration errors. This delayed effect reduces observability in short tests.
  - Verdict Reason: Silver grade because the mutation is "silent" in short time‑step tests, but it is not highly subtle—any test with time step >10.0 would expose it. Since SURVIVED, default tests likely used smaller time intervals, but detecting it does not require complex input combinations (just one large delta‑t). Hence, it is not a rare boundary case (Gold), nor redundant/ineffective (Bronze).
]