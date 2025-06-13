#include <stdio.h>
#include <stdlib.h>

#define MAX_JUDGES 50
#define MAX_JUDGE_STRING_LEN 512
#define MAX_PROJECT_NAME_LEN 100
#define EXPECTED_FIELDS 7

/*
 * (Bonus) Helper function to count the number of fields in the input string.
 * It works by counting the separators (commas). For 7 fields, there should be 6 commas.
 * Returns the number of fields found.
 */
int count_input_fields(const char* input_str) {
    int commas = 0;
    int i = 0;
    
    // An empty string has 0 fields.
    if (input_str[0] == '\0') {
        return 0;
    }
    
    while (input_str[i] != '\0') {
        if (input_str[i] == ',') {
            commas++;
        }
        i++;
    }
    
    // Number of fields is one more than the number of commas.
    return commas + 1;
}

/*
 * Helper function to parse and print a single judge's data string.
 * It iterates through the string, printing each character. When it finds
 * a comma, it prints a newline instead, effectively separating the fields.
 */
void display_parsed_judge_info(const char* info) {
    int i = 0;
    while (info[i] != '\0') {
        // When a comma is found, print a newline to separate fields.
        if (info[i] == ',') {
            printf("\n");
            i++; // Move past the comma.
            // Skip any leading whitespace for the next field.
            while (info[i] == ' ') {
                i++;
            }
        } else {
            printf("%c", info[i]);
            i++;
        }
    }
    printf("\n");
}


int main(void) {
    char project_name[MAX_PROJECT_NAME_LEN];
    int total_judges = 0;
    int selected_members = 0;
    // Per constraint, a 1D array where each element is a string holding judge data.
    char judges_array[MAX_JUDGES][MAX_JUDGE_STRING_LEN];
    int judge_count = 0;
    char user_choice;

    // --- Data Entry Phase ---
    printf("####################################\n");
    printf("#      Judge List Data Entry      #\n");
    printf("####################################\n");

    printf("Participating Project: ");
    scanf(" %[^\n]", project_name);

    printf("Total Number of Judges: ");
    scanf("%d", &total_judges);
    
    printf("Number of Selected Members: ");
    scanf("%d", &selected_members);

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Starting to input information for %d judges.\n", total_judges);
    printf("++++++++++++++++++++++++++++++++++++\n");

    // Infinite while loop for data entry, as per constraint.
    while (1) {
        // Condition to exit the loop once all judges are entered.
        if (judge_count >= total_judges) {
            break;
        }

        // (Bonus) Inner loop for validation and re-entry.
        while (1) {
            printf("Judge %d: ", judge_count + 1);
            
            // Read the entire quoted string. The format specifier consumes the
            // opening quote, reads until the closing quote, and consumes it.
            scanf(" \"%[^\"]\"", judges_array[judge_count]);
            
            if (count_input_fields(judges_array[judge_count]) == EXPECTED_FIELDS) {
                break; // Exit validation loop if input is correct.
            } else {
                printf("The input items are incorrect. Please enter them again.\n");
            }
        }
        judge_count++;
    }

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Judge information entry is complete.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");

    // --- Confirmation and Display Phase ---
    printf("[%s] Should we check the judge information? (Y/N): ", project_name);
    scanf(" %c", &user_choice);

    if (user_choice == 'Y' || user_choice == 'y') {
        printf("####################################\n");
        printf("#        Display Judge Data        #\n");
        printf("####################################\n");

        // For loop to display data, as per constraint.
        for (int i = 0; i < total_judges; i++) {
            printf("[Judge %d]\n", i + 1);
            display_parsed_judge_info(judges_array[i]);
            printf("-----------------------------------\n");
        }
    }
    
    // If 'N' or any other character is entered, the program simply finishes.
    return 0;
}
