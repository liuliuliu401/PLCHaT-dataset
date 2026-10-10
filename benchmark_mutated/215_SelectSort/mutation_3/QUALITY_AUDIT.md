# Mutation Quality Audit

GRADE: Gold  
SUMMARY: A plausible swap error that corrupts the sorted sequence only under specific orderings, making detection difficult.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow — direct mutation of a single assignment in a swap operation, but the incorrect assignment (seq[min_idx] := seq[i]) partially duplicates data before overwriting it with temp, effectively setting both seq[i] and seq[min_idx] to the original seq[i] value when min_idx ≠ i, thus destroying the intended swap.  
  - Observability: Corrupts Data — the sorted sequence becomes incorrect, but only for indices where min_idx ≠ i and the original seq[min_idx] ≠ seq[i]; if values are equal or if min_idx equals i often, the bug is masked.  
  - Verdict Reason: Gold because the mutation survives default tests due to sparse input coverage: many tests might use already‑sorted arrays, arrays with identical elements, or small arrays where min_idx rarely differs from i, requiring a precise boundary ordering to expose the defect.  
]