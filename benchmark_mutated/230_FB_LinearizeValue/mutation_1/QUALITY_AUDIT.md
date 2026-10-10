# Mutation Quality Audit

GRADE: Gold  
SUMMARY: This mutation survived because it exploits an input subrange where tests are likely sparse, while causing a physically invalid output that appears plausible numerically.  

DETAILED_ANALYSIS:  
- Logic Path: Shallow – only a single linearization expression in an ELSIF branch is replaced with a constant assignment.  
- Observability: The bug corrupts the primary process data (LinearizedValue) by outputting a constant instead of a linearly scaled value, while leaving Error and Status unchanged, making the failure stealthy during nominal operation.  
- Verdict Reason: Gold because:  
  1. **SURVIVED** implies default tests missed this subrange (4800–7000) entirely or used insufficient sampling.  
  2. The mutated constant (720.0) coincides with the correct upper bound of the linearization, so simple endpoint tests might not detect it.  
  3. Detection requires either dense sampling or a formal check of the linearization slope, which is often overlooked in conventional PLC testing.