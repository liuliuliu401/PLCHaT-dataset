# Mutation Quality Audit

GRADE: Bronze  
SUMMARY: The mutation survives testing because it introduces redundant writes that do not corrupt observable behavior under normal test coverage.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow—the mutation only adds extra assignments in the same conditional branch already triggered by `requestMove`.  
  - Observability: Corrupts Metadata (specifically the states of `conveyorRight` and `conveyorLeft`) rather than Data; these outputs might be unused or reset elsewhere in the logic before affecting physical actuators.  
  - Verdict Reason: Bronze is assigned because the bug is masked by redundancy—tests likely validate end‑effect of `moving` but not the transient conveyor states, making detection trivial if those states were monitored. The mutation does not represent a subtle physical‑model deviation but an easily overlooked side‑effect in sparse input scenarios.  
]