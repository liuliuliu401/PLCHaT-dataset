# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation survives because it exploits the PLC's initial state, requiring a specific timing condition to detect, making it a silent yet high-impact physical safety hazard.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow (single IF branch), but embedded in an initial heating cycle that may not be monitored until after the timer expires.
  - Observability: Directly corrupts physical actuation (HeaterOutput) while leaving auxiliary status (HeaterStatus) consistent with the faulty output; plausibly hides from routine status-only tests.
  - Verdict Reason: Gold because detection relies on catching the PLC’s first scan during an overheat‑protection‑disabled window and verifying the heater’s actual physical state before the timer elapses—a precise and often untested temporal boundary.
]