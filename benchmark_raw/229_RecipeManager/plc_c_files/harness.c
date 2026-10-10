#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "iec_types_all.h"
#include "POUS.h"

TIME __CURRENT_TIME;
BOOL __DEBUG = 0;
bool silent_mode = false;

void read_inputs(FB_RECIPEMANAGER* instance) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') return;
    
    // Temporary storage for array inputs (if needed)
    int temp_recipeIDs[10] = {0};
    int temp_ingredientTypes[10] = {0};
    float temp_ratios[10] = {0.0f};
    float temp_temps[10] = {0.0f};
    
    // Parse input line with format:
    // ADDRECIPE DELETERECIPE MODIFYRECIPE QUERYRECIPE RECIPEIN_RECIPEID RECIPEIN_INGREDIENTTYPE RECIPEIN_INGREDIENTRATIO RECIPEIN_PRODUCTIONTEMPERATURE
    // Followed by 10 recipe IDs, 10 ingredient types, 10 ratios, 10 temperatures
    int addRecipe, deleteRecipe, modifyRecipe, queryRecipe;
    int recipeIn_recipeID, recipeIn_ingredientType;
    float recipeIn_ingredientRatio, recipeIn_productionTemperature;
    
    int parsed = sscanf(line,
        "%d %d %d %d %d %d %f %f"
        " %d %d %d %d %d %d %d %d %d %d"
        " %d %d %d %d %d %d %d %d %d %d"
        " %f %f %f %f %f %f %f %f %f %f"
        " %f %f %f %f %f %f %f %f %f %f",
        &addRecipe, &deleteRecipe, &modifyRecipe, &queryRecipe,
        &recipeIn_recipeID, &recipeIn_ingredientType,
        &recipeIn_ingredientRatio, &recipeIn_productionTemperature,
        &temp_recipeIDs[0], &temp_recipeIDs[1], &temp_recipeIDs[2], &temp_recipeIDs[3], &temp_recipeIDs[4],
        &temp_recipeIDs[5], &temp_recipeIDs[6], &temp_recipeIDs[7], &temp_recipeIDs[8], &temp_recipeIDs[9],
        &temp_ingredientTypes[0], &temp_ingredientTypes[1], &temp_ingredientTypes[2], &temp_ingredientTypes[3], &temp_ingredientTypes[4],
        &temp_ingredientTypes[5], &temp_ingredientTypes[6], &temp_ingredientTypes[7], &temp_ingredientTypes[8], &temp_ingredientTypes[9],
        &temp_ratios[0], &temp_ratios[1], &temp_ratios[2], &temp_ratios[3], &temp_ratios[4],
        &temp_ratios[5], &temp_ratios[6], &temp_ratios[7], &temp_ratios[8], &temp_ratios[9],
        &temp_temps[0], &temp_temps[1], &temp_temps[2], &temp_temps[3], &temp_temps[4],
        &temp_temps[5], &temp_temps[6], &temp_temps[7], &temp_temps[8], &temp_temps[9]);
    
    // Minimum required fields: first 8 values
    if (parsed >= 8) {
        instance->ADDRECIPE.value = addRecipe;
        instance->DELETERECIPE.value = deleteRecipe;
        instance->MODIFYRECIPE.value = modifyRecipe;
        instance->QUERYRECIPE.value = queryRecipe;
        instance->RECIPEIN_RECIPEID.value = recipeIn_recipeID;
        instance->RECIPEIN_INGREDIENTTYPE.value = recipeIn_ingredientType;
        instance->RECIPEIN_INGREDIENTRATIO.value = recipeIn_ingredientRatio;
        instance->RECIPEIN_PRODUCTIONTEMPERATURE.value = recipeIn_productionTemperature;
        
        // If we have array data (all 48 values), update the arrays
        if (parsed >= 48) {
            for (int i = 0; i < 10; i++) {
                // Note: ST arrays are 1-indexed, C arrays are 0-indexed
                instance->RECIPE_RECIPEID.value.table[i] = temp_recipeIDs[i];
                instance->RECIPE_INGREDIENTTYPE.value.table[i] = temp_ingredientTypes[i];
                instance->RECIPE_INGREDIENTRATIO.value.table[i] = temp_ratios[i];
                instance->RECIPE_PRODUCTIONTEMPERATURE.value.table[i] = temp_temps[i];
            }
        }
    }
}

void print_fb_state(FB_RECIPEMANAGER* instance, int cycle) {
    if (silent_mode) return;
    printf("\n--- Cycle %d ---\n", cycle);
    
    // Print inputs
    printf("Inputs:\n");
    printf("  ADDRECIPE: %d\n", instance->ADDRECIPE.value);
    printf("  DELETERECIPE: %d\n", instance->DELETERECIPE.value);
    printf("  MODIFYRECIPE: %d\n", instance->MODIFYRECIPE.value);
    printf("  QUERYRECIPE: %d\n", instance->QUERYRECIPE.value);
    printf("  RECIPEIN_RECIPEID: %d\n", instance->RECIPEIN_RECIPEID.value);
    printf("  RECIPEIN_INGREDIENTTYPE: %d\n", instance->RECIPEIN_INGREDIENTTYPE.value);
    printf("  RECIPEIN_INGREDIENTRATIO: %f\n", instance->RECIPEIN_INGREDIENTRATIO.value);
    printf("  RECIPEIN_PRODUCTIONTEMPERATURE: %f\n", instance->RECIPEIN_PRODUCTIONTEMPERATURE.value);
    
    // Print outputs
    printf("\nOutputs:\n");
    printf("  RECIPEADDED: %d\n", instance->RECIPEADDED.value);
    printf("  RECIPEDELETED: %d\n", instance->RECIPEDELETED.value);
    printf("  RECIPEMODIFIED: %d\n", instance->RECIPEMODIFIED.value);
    printf("  RECIPEQUERYRESULT_RECIPEID: %d\n", instance->RECIPEQUERYRESULT_RECIPEID.value);
    printf("  RECIPEQUERYRESULT_INGREDIENTTYPE: %d\n", instance->RECIPEQUERYRESULT_INGREDIENTTYPE.value);
    printf("  RECIPEQUERYRESULT_INGREDIENTRATIO: %f\n", instance->RECIPEQUERYRESULT_INGREDIENTRATIO.value);
    printf("  RECIPEQUERYRESULT_PRODUCTIONTEMPERATURE: %f\n", instance->RECIPEQUERYRESULT_PRODUCTIONTEMPERATURE.value);
    printf("  ERROR: %d\n", instance->ERROR.value);
    printf("  STATUS: %u (0x%04X)\n", instance->STATUS.value, instance->STATUS.value);
    
    // Print IN_OUT arrays (recipe storage)
    printf("\nRecipe Storage:\n");
    for (int i = 0; i < 10; i++) {
        printf("  Slot %d: ID=%d, Type=%d, Ratio=%f, Temp=%f\n",
               i + 1,  // Show 1-indexed for user clarity
               instance->RECIPE_RECIPEID.value.table[i],
               instance->RECIPE_INGREDIENTTYPE.value.table[i],
               instance->RECIPE_INGREDIENTRATIO.value.table[i],
               instance->RECIPE_PRODUCTIONTEMPERATURE.value.table[i]);
    }
    
    printf("----------------\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_RECIPEMANAGER instance;
    FB_RECIPEMANAGER_init__(&instance, FALSE);
    
    printf("FB_RecipeManager Harness\n");
    printf("Input format: ADDRECIPE DELETERECIPE MODIFYRECIPE QUERYRECIPE RECIPEIN_RECIPEID RECIPEIN_INGREDIENTTYPE RECIPEIN_INGREDIENTRATIO RECIPEIN_PRODUCTIONTEMPERATURE\n");
    printf("Followed by 10 recipe IDs, 10 ingredient types, 10 ratios, 10 temperatures (optional)\n");
    printf("Example: 1 0 0 0 100 1 0.5 75.0  100 0 0 0 0 0 0 0 0 0  1 0 0 0 0 0 0 0 0 0  0.5 0 0 0 0 0 0 0 0 0  75.0 0 0 0 0 0 0 0 0 0\n");
    printf("Enter '#' for comments, blank line to skip\n");
    
    int cycle = 0;
    while (1) {
        read_inputs(&instance);
        cycle++;
        
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        __CURRENT_TIME.tv_sec = ts.tv_sec;
        __CURRENT_TIME.tv_nsec = ts.tv_nsec;
        
        FB_RECIPEMANAGER_body__(&instance);
        print_fb_state(&instance, cycle);
        fflush(stdout);
    }
    
    return 0;
}