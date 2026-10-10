FUNCTION_BLOCK FB_RecipeManager
VAR_INPUT 
addRecipe : BOOL;
deleteRecipe : BOOL;
modifyRecipe : BOOL;
queryRecipe : BOOL;
recipeIn_recipeID : INT;
recipeIn_ingredientType : INT;
recipeIn_ingredientRatio : REAL;
recipeIn_productionTemperature : REAL;
END_VAR

VAR_OUTPUT 
recipeAdded : BOOL;
recipeDeleted : BOOL;
recipeModified : BOOL;
recipeQueryResult_recipeID : INT;
recipeQueryResult_ingredientType : INT;
recipeQueryResult_ingredientRatio : REAL;
recipeQueryResult_productionTemperature : REAL;
error : BOOL;
status : WORD;
END_VAR

VAR_IN_OUT 
recipe_recipeID : ARRAY[1..10] OF INT;
recipe_ingredientType : ARRAY[1..10] OF INT;
recipe_ingredientRatio : ARRAY[1..10] OF REAL;
recipe_productionTemperature : ARRAY[1..10] OF REAL;
END_VAR

VAR
i : INT;
found : BOOL;
END_VAR

// Reset output flags
recipeAdded := FALSE;
recipeDeleted := FALSE;
recipeModified := FALSE;
error := FALSE;
status := 0;

// Add recipe logic
IF addRecipe THEN
found := FALSE;
FOR i := 1 TO 10 DO
 IF recipe_recipeID[i] = recipeIn_recipeID THEN
found := TRUE;
error := TRUE;
status := 16#8002;
RETURN;
 END_IF;
END_FOR;

IF NOT found THEN
 found := FALSE;
 FOR i := 1 TO 10 DO
IF recipe_recipeID[i] = 0 THEN
recipe_recipeID[i] := recipeIn_recipeID;
recipe_ingredientType[i] := recipeIn_ingredientType;
recipe_ingredientRatio[i] := recipeIn_ingredientRatio;
recipe_productionTemperature[i] := recipeIn_productionTemperature;
recipeAdded := TRUE;
found := TRUE;
EXIT;
END_IF;
 END_FOR;
 
 IF NOT found THEN
error := TRUE;
status := 16#8003;
 END_IF;
END_IF;
END_IF;

// Delete recipe logic
IF deleteRecipe THEN
found := FALSE;
FOR i := 1 TO 10 DO
 IF recipe_recipeID[i] = recipeIn_recipeID THEN
recipe_recipeID[i] := 0;
recipe_ingredientType[i] := 0;
recipe_ingredientRatio[i] := 0.0;
recipe_productionTemperature[i] := 0.0;
recipeDeleted := TRUE;
found := TRUE;
EXIT;
 END_IF;
END_FOR;

IF NOT found THEN
 error := TRUE;
 status := 16#8004;
END_IF;
END_IF;

// Modify recipe logic
IF modifyRecipe THEN
found := FALSE;
FOR i := 1 TO 10 DO
 IF recipe_recipeID[i] = recipeIn_recipeID THEN
recipe_recipeID[i] := recipeIn_recipeID;
recipe_ingredientType[i] := recipeIn_ingredientType;
recipe_ingredientRatio[i] := recipeIn_ingredientRatio;
recipe_productionTemperature[i] := recipeIn_productionTemperature;
recipeModified := TRUE;
found := TRUE;
RETURN;
 END_IF;
END_FOR;

IF NOT found THEN
 error := TRUE;
 status := 16#8005;
END_IF;
END_IF;

// Query recipe logic
IF queryRecipe THEN
found := FALSE;
FOR i := 1 TO 10 DO
 IF recipe_recipeID[i] = recipeIn_recipeID THEN
recipeQueryResult_recipeID := recipe_recipeID[i];
recipeQueryResult_ingredientType := recipe_ingredientType[i];
recipeQueryResult_ingredientRatio := recipe_ingredientRatio[i];
recipeQueryResult_productionTemperature := recipe_productionTemperature[i];
found := TRUE;
RETURN;
 END_IF;
END_FOR;

IF NOT found THEN
 error := TRUE;
 status := 16#8001;
END_IF;
END_IF;

END_FUNCTION_BLOCK
