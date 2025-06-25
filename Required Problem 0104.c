/**
 * @file judges.c
 * @brief Manages judge information for a project, with custom string parsing.
 *
 * This program prompts for project details and then enters a loop to get
 * information for a specified number of judges. Each judge's data is entered
 * as a single, comma-separated string. The program validates that the correct
 * number of data fields are entered (Bonus Feature). Finally, it parses and
 * displays the collected information in a formatted way.
 */

#include <stdio.h>
#include <stdlib.h> // For malloc, free, atoi
#include <string.h> // **FIX: Added to correctly use strtok, strlen, etc.**

// --- Constants ---
#define MAX_JUDGES 10
#define NUM_REQUIRED_FIELDS 7

// --- Main Program Logic ---

int main(void) {
    char project_name[100];
    int num_judges = 0;
    int num_members = 0;
    
    // Using a fixed-size array of pointers.
    // Memory for each string will be allocated dynamically.
    char* judges_array[MAX_JUDGES];

    // --- Setup Phase ---
    printf("####################################\n");
    printf("#      Judge List Data Entry      #\n");
    printf("####################################\n");
    
    printf("Participating Project: ");
    fgets(project_name, sizeof(project_name), stdin);
    project_name[strcspn(project_name, "\n")] = 0; // Remove newline safely

    printf("Total Number of Judges: ");
    char num_buf[10];
    fgets(num_buf, sizeof(num_buf), stdin);
    num_judges = atoi(num_buf);
    
    if (num_judges <= 0 || num_judges > MAX_JUDGES) {
        printf("Invalid number of judges (must be 1-%d). Exiting.\n", MAX_JUDGES);
        return 1;
    }

    printf("Number of Selected Members: ");
    fgets(num_buf, sizeof(num_buf), stdin);
    num_members = atoi(num_buf);
    
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Starting to input information for %d judges.\n", num_judges);
    printf("++++++++++++++++++++++++++++++++++++\n");
    
    // --- Data Entry Phase ---
    int i = 0;
    // An infinite loop that breaks when all judge info is collected.
    while (1) {
        if (i >= num_judges) {
            break; // Exit condition: all judges entered
        }

        char input_buffer[1024];
        printf("Judge %d: ", i + 1);
        fgets(input_buffer, sizeof(input_buffer), stdin);
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        // --- (Bonus) Validation Step ---
        int field_count = 0;
        // Create a copy for validation because strtok modifies the string
        char validation_copy[1024];
        strcpy(validation_copy, input_buffer);

        char* token_validator = strtok(validation_copy, ",");
        while (token_validator != NULL) {
            field_count++;
            token_validator = strtok(NULL, ",");
        }
        
        if (field_count != NUM_REQUIRED_FIELDS) {
            printf("The input items are incorrect. Expected %d fields but got %d. Please enter them again.\n", NUM_REQUIRED_FIELDS, field_count);
            continue; // Skip the rest of the loop and re-prompt for the same judge
        }
        
        // If validation passes, allocate memory and store the string
        judges_array[i] = (char*)malloc(strlen(input_buffer) + 1);
        if (judges_array[i] == NULL) {
            printf("Memory allocation failed. Exiting.\n");
            // Free previously allocated memory before exiting
            for (int k = 0; k < i; k++) {
                free(judges_array[k]);
            }
            return 1;
        }
        strcpy(judges_array[i], input_buffer);
        
        i++; // Move to the next judge
    }

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Judge information entry is complete.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");

    // --- Display Phase ---
    char check_choice[10];
    printf("[%s] Should we check the judge information? (Y/N): ", project_name);
    fgets(check_choice, sizeof(check_choice), stdin);

    if (check_choice[0] == 'Y' || check_choice[0] == 'y') {
        printf("\n####################################\n");
        printf("#        Display Judge Data        #\n");
        printf("####################################\n");

        for (int j = 0; j < num_judges; j++) {
            printf("[Judge %d]\n", j + 1);

            // Create a temporary copy to parse with strtok
            char parse_copy[1024];
            strcpy(parse_copy, judges_array[j]);
            
            char* token = strtok(parse_copy, ",");
            while (token != NULL) {
                // Find the colon to separate key and value
                char* colon = strchr(token, ':');
                if (colon != NULL) {
                    *colon = '\0'; // Split the string at the colon
                    char* key = token;
                    char* value = colon + 1;
                    
                    // Trim leading space from the value, if any
                    while (*value == ' ') {
                        value++;
                    }
                    
                    printf("%s: %s\n", key, value);
                }
                token = strtok(NULL, ","); // Get the next pair
            }
            printf("-----------------------------------\n");
        }
    }

    // --- Cleanup Phase ---
    // Free the dynamically allocated memory for each judge's string
    for (int j = 0; j < num_judges; j++) {
        free(judges_array[j]);
    }
    
    printf("Program finished.\n");
    return 0;
}
