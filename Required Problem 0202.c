#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8

#define NUM_MEMBERS 4
#define NUM_TESTS 7

const char* MAIN_MENU[NUM_MAIN_MENU_ITEMS] = {
    "I. Audition Management",
    "II. Training",
    "III. Debut"
};

const char* TRAINING_MENU[NUM_TRAINING_STAGES] = {
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation", "4. Vocal", "5. Dance", "6. Visual & Image",
    "7. Acting & Stage Performance", "8. Fan Communication"
};

const char* MILLIWAYS_NAMES[NUM_MEMBERS] = {
    "Jiyeon Park", "Ethan Smith", "Helena Silva", "Liam Wilson"
};
const char* MILLIWAYS_NICKNAMES[NUM_MEMBERS] = {
    "Ariel", "Simba", "Belle", "Aladdin"
};
const char* FITNESS_TEST_NAMES[NUM_TESTS] = {
    "1-Mile Run (min)", "100m Sprint (sec)", "Push-ups (30 reps/min)", "Squats (50 reps/min)",
    "Arm Strength (50 reps/min)", "Swimming (400m/min)", "Weightlifting (x bodywt)"
};

void display_main_menu();
void display_training_menu(const char status[]);
void handle_training_menu(char status[], float scores[][NUM_TESTS]);
void handle_stage1_submenu(char* stage_status, float scores[][NUM_TESTS]);
void setHealth(float scores[][NUM_TESTS]);
void parse_and_store_scores(const char* input, float destination_array[]);
void getHealth(const float scores[][NUM_TESTS]);


 
int main() {
    char training_status[NUM_TRAINING_STAGES] = {0};
    // 2D array to store health scores for each member and each test
    float health_scores[NUM_MEMBERS][NUM_TESTS] = {{0.0f}};
    char input_buffer[100];
    int choice;

    while (1) {
        display_main_menu();
        printf("> Select a menu (or 0, q to quit): ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
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
                handle_training_menu(training_status, health_scores);
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
    for (int i = 0; i < NUM_MAIN_MENU_ITEMS; i++) printf("%s\n", MAIN_MENU[i]);
    printf("------------------------------------\n");
}

void display_training_menu(const char status[]) {
    printf("\n####################################\n");
    printf("#         Training Program         #\n");
    printf("####################################\n");
    for (int i = 0; i < NUM_TRAINING_STAGES; i++) {
        printf("%s", TRAINING_MENU[i]);
        if (status[i] == 'P') printf(" [PASSED]\n");
        else if (status[i] == 'F') printf(" [FAILED]\n");
        else printf("\n");
    }
    printf("------------------------------------\n");
}


void handle_training_menu(char status[], float scores[][NUM_TESTS]) {
    char input_buffer[100];
    int choice;
    while (1) {
        display_training_menu(status);
        printf("> Select a stage (or 0 to return to main menu): ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        if (strcmp(input_buffer, "0\n") == 0) {
            printf("\nReturning to main menu...\n\n");
            break;
        }
        choice = atoi(input_buffer);
        if (choice == 1) {
            // Stage 1 now has its own submenu
            handle_stage1_submenu(&status[0], scores);
        } else if (choice > 1 && choice <= NUM_TRAINING_STAGES) {
            // Other stages can be implemented here later
            printf("\nThis training stage is not yet implemented.\n");
        } else {
            printf("\nInvalid stage. Please select a valid number.\n");
        }
    }
}

void handle_stage1_submenu(char* stage_status, float scores[][NUM_TESTS]) {
    char choice_char;
    while(1) {
        printf("\n--- [Stage 1: Physical Strength & Knowledge] ---\n");
        printf("A. Enter Fitness Data\n");
        printf("B. View Fitness Data\n");
        printf("C. Certify Stage 1 Completion\n");
        printf("0. Return to Training Menu\n");
        printf("> Select an option: ");
        scanf(" %c", &choice_char);
        while(getchar() != '\n'); // Clear input buffer

        if (choice_char == '0') break;

        switch(choice_char) {
            case 'A': case 'a':
                setHealth(scores);
                break;
            case 'B': case 'b':
                getHealth(scores);
                break;
            case 'C': case 'c':
                if (*stage_status == 'P') {
                    printf("\nStage 1 has already been passed.\n");
                } else {
                    printf("Did the members pass the certification for Stage 1? (Y/N): ");
                    scanf(" %c", &choice_char);
                    while(getchar() != '\n');
                    if (choice_char == 'Y' || choice_char == 'y') {
                        *stage_status = 'P';
                        printf("Result recorded: [PASSED]\n");
                    } else {
                        *stage_status = 'F';
                        printf("Result recorded: [FAILED]\n");
                    }
                }
                break;
            default:
                printf("\nInvalid option. Please choose A, B, C, or 0.\n");
        }
    }
}


void setHealth(float scores[][NUM_TESTS]) {
    char input_buffer[256];
    printf("\n--- [Enter Fitness Data] ---\n");
    printf("Please enter 7 comma-separated values for each member.\n");
    printf("Order: %s, %s, etc.\n", FITNESS_TEST_NAMES[0], FITNESS_TEST_NAMES[1]);

    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("\n> Member: %s (%s)\n> Input: ", MILLIWAYS_NAMES[i], MILLIWAYS_NICKNAMES[i]);
        fgets(input_buffer, sizeof(input_buffer), stdin);
        parse_and_store_scores(input_buffer, scores[i]);
    }
    printf("\nFitness data for all members has been recorded.\n");
}


void parse_and_store_scores(const char* input, float destination_array[]) {
    sscanf(input, "%f,%f,%f,%f,%f,%f,%f",
           &destination_array[0], &destination_array[1], &destination_array[2], &destination_array[3],
           &destination_array[4], &destination_array[5], &destination_array[6]);
}


void getHealth(const float scores[][NUM_TESTS]) {
    int choice;
    char nickname_buffer[100];
    while(1) {
        printf("\n--- [View Fitness Data] ---\n");
        printf("1. View All Members' Data\n");
        printf("2. Find Member by Nickname\n");
        printf("0. Return to Previous Menu\n");
        printf("> Select an option: ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        if(choice == 0) break;

        if(choice == 1) {
            printf("\n--- All Members Fitness Data ---\n");
            
            printf("%-15s | %-10s |", "Name", "Nickname");
            for(int i = 0; i < NUM_TESTS; i++) printf(" %-10.10s |", FITNESS_TEST_NAMES[i]);
            printf("\n");
            
            for(int i = 0; i < NUM_MEMBERS; i++) {
                printf("%-15s | %-10s |", MILLIWAYS_NAMES[i], MILLIWAYS_NICKNAMES[i]);
                for(int j = 0; j < NUM_TESTS; j++) printf(" %-10.2f |", scores[i][j]);
                printf("\n");
            }
        } else if (choice == 2) {
            printf("Enter nickname to find: ");
            fgets(nickname_buffer, sizeof(nickname_buffer), stdin);
            nickname_buffer[strcspn(nickname_buffer, "\n")] = 0; // Remove newline

            int found_index = -1;
            for(int i = 0; i < NUM_MEMBERS; i++) {
                if(strcmp(nickname_buffer, MILLIWAYS_NICKNAMES[i]) == 0) {
                    found_index = i;
                    break;
                }
            }

            if(found_index != -1) {
                printf("\n--- Fitness Data for %s ---\n", MILLIWAYS_NAMES[found_index]);
                printf("Name: %s\n", MILLIWAYS_NAMES[found_index]);
                printf("Nickname: %s\n", MILLIWAYS_NICKNAMES[found_index]);
                printf("--------------------------------\n");
                for(int i = 0; i < NUM_TESTS; i++) {
                    printf("%-28s: %.2f\n", FITNESS_TEST_NAMES[i], scores[found_index][i]);
                }
            } else {
                printf("Member with nickname '%s' not found.\n", nickname_buffer);
            }
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}
