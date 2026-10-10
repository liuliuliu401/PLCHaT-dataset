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
    
    // Initialize arrays for input parsing
    int temp_recipeID[10];
    int temp_ingredientType[10];
    float temp_ingredientRatio[10];
    float temp_productionTemperature[10];
    
    // Parse all inputs at once
    int parsed = sscanf(line, 
        "%d %d %d %d %d %d %f %f "  // Inputs: addRecipe, deleteRecipe, modifyRecipe, queryRecipe, recipeIn_recipeID, recipeIn_ingredientType, recipeIn_ingredientRatio, recipeIn_productionTemperature
        "%d %d %d %d %d %d %d %d %d %d "  // recipe_recipeID[1..10]
        "%d %d %d %d %d %d %d %d %d %d "  // recipe_ingredientType[1..10]
        "%f %f %f %f %f %f %f %f %f %f "  // recipe_ingredientRatio[1..10]
        "%f %f %f %f %f %f %f %f %f %f",  // recipe_productionTemperature[1..10]
        
        // Input variables
        &instance->ADDRECIPE.value,
        &instance->DELETERECIPE.value,
        &instance->MODIFYRECIPE.value,
        &instance->QUERYRECIPE.value,
        &instance->RECIPEIN_RECIPEID.value,
        &instance->RECIPEIN_INGREDIENTTYPE.value,
        &instance->RECIPEIN_INGREDIENTRATIO.value,
        &instance->RECIPEIN_PRODUCTIONTEMPERATURE.value,
        
        // Array inputs (index 0-9 for ST arrays 1-10)
        &temp_recipeID[0], &temp_recipeID[1], &temp_recipeID[2], &temp_recipeID[3], &temp_recipeID[4],
        &temp_recipeID[5], &temp_recipeID[6], &temp_recipeID[7], &temp_recipeID[8], &temp_recipeID[9],
        
        &temp_ingredientType[0], &temp_ingredientType[1], &temp_ingredientType[2], &temp_ingredientType[3], &temp_ingredientType[4],
        &temp_ingredientType[5], &temp_ingredientType[6], &temp_ingredientType[7], &temp_ingredientType[8], &temp_ingredientType[9],
        
        &temp_ingredientRatio[0], &temp_ingredientRatio[1], &temp_ingredientRatio[2], &temp_ingredientRatio[3], &temp_ingredientRatio[4],
        &temp_ingredientRatio[5], &temp_ingredientRatio[6], &temp_ingredientRatio[7], &temp_ingredientRatio[8], &temp_ingredientRatio[9],
        
        &temp_productionTemperature[0], &temp_productionTemperature[1], &temp_productionTemperature[2], &temp_productionTemperature[3], &temp_productionTemperature[4],
        &temp_productionTemperature[5], &temp_productionTemperature[6], &temp_productionTemperature[7], &temp_productionTemperature[8], &temp_productionTemperature[9]
    );
    
    // Only update arrays if we parsed enough values
    if (parsed >= 8) {
        for (int i = 0; i < 10; i++) {
            if (i < 10 && parsed >= 18 + i) {
                instance->RECIPE_RECIPEID.value.table[i] = temp_recipeID[i];
            }
            if (i < 10 && parsed >= 28 + i) {
                instance->RECIPE_INGREDIENTTYPE.value.table[i] = temp_ingredientType[i];
            }
            if (i < 10 && parsed >= 38 + i) {
                instance->RECIPE_INGREDIENTRATIO.value.table[i] = temp_ingredientRatio[i];
            }
            if (i < 10 && parsed >= 48 + i) {
                instance->RECIPE_PRODUCTIONTEMPERATURE.value.table[i] = temp_productionTemperature[i];
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
    
    // Print array states
    printf("\nRecipe Arrays:\n");
    for (int i = 0; i < 10; i++) {
        printf("  Slot %d: ID=%d, Type=%d, Ratio=%f, Temp=%f\n", 
            i+1,
            instance->RECIPE_RECIPEID.value.table[i],
            instance->RECIPE_INGREDIENTTYPE.value.table[i],
            instance->RECIPE_INGREDIENTRATIO.value.table[i],
            instance->RECIPE_PRODUCTIONTEMPERATURE.value.table[i]);
    }
    
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
    printf("  STATUS: 0x%04X\n", instance->STATUS.value);
    
    // Print internal variables
    printf("\nInternal Variables:\n");
    printf("  I: %d\n", instance->I.value);
    printf("  FOUND: %d\n", instance->FOUND.value);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) silent_mode = true;
    }
    
    FB_RECIPEMANAGER instance;
    FB_RECIPEMANAGER_init__(&instance, FALSE);
    
    int cycle = 0;
    
    // Print input format help
    if (!silent_mode) {
        printf("\n=== FB_RECIPEMANAGER Test Harness ===\n");
        printf("Input Format (space separated):\n");
        printf("  Line 1: ADDRECIPE DELETERECIPE MODIFYRECIPE QUERYRECIPE RECIPEIN_RECIPEID RECIPEIN_INGREDIENTTYPE RECIPEIN_INGREDIENTRATIO RECIPEIN_PRODUCTIONTEMPERATURE\n");
        printf("  Line 2: 10x recipe_recipeID values (slots 1-10)\n");
        printf("  Line 3: 10x recipe_ingredientType values\n");
        printf("  Line 4: 10x recipe_ingredientRatio values\n");
        printf("  Line 5: 10x recipe_productionTemperature values\n");
        printf("Or provide all values on one line (48 values total)\n");
        printf("Use # for comments, blank line to skip cycle\n");
        printf("Enter inputs:\n");
    }
    
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

