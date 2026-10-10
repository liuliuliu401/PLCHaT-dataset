# Mutation Quality Audit

GRADE: Silver  
SUMMARY: A plausible data corruption bug that swaps two elements incorrectly, surviving basic tests due to sparse test coverage of element equality cases.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow—the mutation alters a single assignment in an already-executed conditional swap, a one-line change in a well-trodden algorithm (selection sort).
  - Observability: Corrupts the Data directly (array contents) by breaking the swap symmetry—after `seq[i] := seq[min_idx]`, the subsequent `seq[min_idx] := seq[i]` uses the already-overwritten value, effectively setting both positions to the same value if `min_idx ≠ i`. This can produce partially sorted, non‑permuted outputs.
  - Verdict Reason: Silver because the bug is shallow and deterministic, yet it survived default tests—indicating those tests likely lacked inputs where `min_idx ≠ i` for multiple iterations, or where the corrupted swap visibly affects final order. It does not require rare boundary values (Gold), but its detection demands covering the swap‑execution path, which is a moderate functional gap, not merely redundant code (Bronze).  
]