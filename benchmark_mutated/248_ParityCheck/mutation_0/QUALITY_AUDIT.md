# Mutation Quality Audit

GRADE: Silver  
SUMMARY: The mutation survived default tests by introducing a simple yet redundant bug that only becomes observable for very specific input values, making it a 'silent deviation' with moderate detection difficulty.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow; mutation adds an extra identical SHR operation, causing an unintended double right‑shift.
  - Observability: Bug corrupts the Data (LinearizedValue) but only when the least‑significant bits are non‑zero before the shift; many test values likely mask it by having zero LSBs or landing in a valid output range.
  - Verdict Reason: Rated Silver because the bug is not trivially caught by any input (SURVIVED), yet it does not require an extremely precise boundary—just a sparse subset of inputs where the double‑shift changes the final result. It lacks the subtlety of a physical‑model violation (hence not Gold), but also is not redundant code that never triggers (hence not Bronze).
]