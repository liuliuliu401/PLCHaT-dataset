# Mutation Quality Audit

GRADE: Gold  
SUMMARY: The mutation silently corrupts a critical process parameter with a plausible-looking value, surviving default tests due to sparse input validation.  
DETAILED_ANALYSIS: [  
- Logic Path: Shallow—single assignment inside an existing conditional, no structural complexity.  
- Observability: Corrupts Data (LinearizedValue) by scaling ingredientRatio by 1.5×, directly affecting physical process outcomes without raising errors.  
- Verdict Reason: Gold-grade because the bug exploits sparse testing of recipe fields; it alters a ratio subtly, producing physically invalid yet numerically plausible outputs that standard regression tests likely miss unless specifically checking ratio constraints or resulting product quality.  
]