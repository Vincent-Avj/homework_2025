#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8

const char* MAIN_MENU[NUM_MAIN_MENU_ITEMS] = {
    "I. Audition Management",
    "II. Training",
    "III. Debut"
};

const char* TRAINING_MENU[NUM_TRAINING_STAGES] = {
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation",
    "4. Vocal",
    "5. Dance",
    "6. Visual & Image",
    "7. Acting & Stage Performance",
    "8. Fan Communication"
};


void display_main_menu();
void display_training_menu(const char status[]);
void handle_training_menu(char status[]);
void process_stage_evaluation(int stage_index, char status[]);

int main() {
    
    char training_status[NUM_TRAINING_STAGES] = {0};
    char input_buffer[100];
    int choice;

    while (1) {
        display_main_menu();
        printf("> Select a menu (or 0, q to quit): ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break; 
        }


        if (strcmp(input_buffer, "\n") == 0 || strcmp(input_buffer, "0\n") == 0 ||
            strcmp(input_buffer, "q\n") == 0 || strcmp(input_buffer, "Q\n") == 0) {
            printf("Terminating Magrathea System.\n");
            break;
        }

        choice = atoi(input_buffer);

        switch (choice) {
            case 1:
                printf("\nNavigating to [Audition Management]...\n(This feature is not yet implemented.)\n\n");
                break;
            case 2:
                handle_training_menu(training_status);
                break;
            case 3:
                printf("\nNavigating to [Debut]...\n(This feature is not yet implemented.)\n\n");
                break;
            default:
                printf("\nInvalid selection. Please try again.\n\n");
        }
    }
    return 0;
}


void display_main_menu() {
    printf("####################################\n");
    printf("#        Magrathea Main Menu       #\n");
    printf("####################################\n");
    for (int i = 0; i < NUM_MAIN_MENU_ITEMS; i++) {
        printf("%s\n", MAIN_MENU[i]);
    }
    printf("------------------------------------\n");
}

void display_training_menu(const char status[]) {
    printf("\n####################################\n");
    printf("#         Training Program         #\n");
    printf("####################################\n");
    for (int i = 0; i < NUM_TRAINING_STAGES; i++) {
        printf("%s", TRAINING_MENU[i]);
        if (status[i] == 'P') {
            printf(" [PASSED]\n");
        } else if (status[i] == 'F') {
            printf(" [FAILED]\n");
        } else {
            printf("\n");
        }
    }
    printf("------------------------------------\n");
}


void handle_training_menu(char status[]) {
    char input_buffer[100];
    int choice;

    while (1) {
        display_training_menu(status);
        printf("> Select a stage (or 0 to return to main menu): ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        if (strcmp(input_buffer, "0\n") == 0) {
            printf("\nReturning to main menu...\n\n");
            break;
        }

        choice = atoi(input_buffer);

        if (choice >= 1 && choice <= NUM_TRAINING_STAGES) {
            process_stage_evaluation(choice - 1, status);
        } else {
            printf("\nInvalid stage. Please select a number from 1 to 8.\n");
        }
    }
}


void process_stage_evaluation(int stage_index, char status[]) {
    char input;


    if (status[stage_index] == 'P') {
        printf("\nThis stage has already been passed and cannot be re-selected.\n");
        return;
    }

    if (stage_index == 1 && status[0] != 'P') {
        printf("\nPrerequisite Error: You must pass Stage 1 before attempting Stage 2.\n");
        return;
    }

    if (stage_index > 1 && (status[0] != 'P' || status[1] != 'P')) {
        printf("\nPrerequisite Error: You must pass both Stage 1 and 2 before attempting other stages.\n");
        return;
    }

    printf("\nSelected: %s\n", TRAINING_MENU[stage_index]);
    printf("Would you like to enter the evaluation result? (Y/N): ");
    scanf(" %c", &input);
    while (getchar() != '\n'); // Clear input buffer

    if (input == 'Y' || input == 'y') {
        printf("Did you complete the training and pass the certification? (Y/N): ");
        scanf(" %c", &input);
        while (getchar() != '\n'); // Clear input buffer

        if (input == 'Y' || input == 'y') {
            status[stage_index] = 'P';
            printf("Result recorded: [PASSED]\n");
        } else {
            status[stage_index] = 'F';
            printf("Result recorded: [FAILED]\n");
        }
    } else {
        printf("Returning to training menu.\n");
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_MENU_ITEMS 12 // 3 main + 1 separator + 8 training


const char* COMBINED_MENU[TOTAL_MENU_ITEMS] = {

    "I. Audition Management",
    "II. Training",
    "III. Debut",
    NULL, 
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation",
    "4. Vocal",
    "5. Dance",
    "6. Visual & Image",
    "7. Acting & Stage Performance",
    "8. Fan Communication"
};

void display_menu_section(int start, int end, const char status[]) {
    printf("####################################\n");
    if (start == 0) {
        printf("#        Magrathea Main Menu       #\n");
    } else {
        printf("#         Training Program         #\n");
    }
    printf("####################################\n");

    for (int i = start; i < end; i++) {
        printf("%s", COMBINED_MENU[i]);

        if (status != NULL) {
            int status_index = i - start;
            if (status[status_index] == 'P') {
                printf(" [PASSED]\n");
            } else if (status[status_index] == 'F') {
                printf(" [FAILED]\n");
            } else {
                printf("\n");
            }
        } else {
            printf("\n");
        }
    }
    printf("------------------------------------\n");
}

