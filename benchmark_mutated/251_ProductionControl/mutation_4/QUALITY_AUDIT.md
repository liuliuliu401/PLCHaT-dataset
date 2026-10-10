# Mutation Quality Audit

GRADE: Gold  
SUMMARY: The mutation introduces a subtle plausibility-violating correlation (auto requiring manual) that survives sparse default tests but critically corrupts process control semantics.  

DETAILED_ANALYSIS: [  
  - Logic Path: Shallow (single Boolean expression), but the mutation creates a semantic dependency where “Auto Mode” is only true if Mode=TRUE AND isManual=FALSE, which introduces a nonsensical physical constraint.  
  - Observability: This directly corrupts the **Data** (Auto/Manual state) that drives downstream control decisions, potentially causing a machine to incorrectly stay in manual when it should be auto, or vice versa.  
  - Verdict Reason: Gold because detection requires a **specific input combination** (Mode=TRUE and NOT Mode=TRUE for isManual) which sparse default tests likely missed, yet the impact is high—violates the physical independence of Auto and Manual modes.  
]