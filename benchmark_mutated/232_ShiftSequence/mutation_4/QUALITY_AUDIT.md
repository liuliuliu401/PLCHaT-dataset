# Mutation Quality Audit

GRADE: Bronze  
SUMMARY: This mutation survived because the conditional logic became redundant under default tests, making the bug observable only when both shift directions are disabled—a scenario not covered by typical test inputs.  
DETAILED_ANALYSIS: [
  - Logic Path: Shallow; the mutation changes a single condition from `shiftLeft` to `shiftLeft OR shiftRight`, causing the loop to execute for either direction where previously only one direction triggered it.
  - Observability: The bug corrupts the Data (LinearizedValue) directly, as the loop shifts the value incorrectly when `shiftRight` is true but the original logic would not shift, altering the output.
  - Verdict Reason: Bronze grade because the survival stems from code redundancy—the default tests likely only exercise `shiftLeft = TRUE` cases, leaving the mutated branch untested. This is not a subtle physical-model violation but an oversight in test coverage for the complementary condition (`shiftRight = TRUE`).
]