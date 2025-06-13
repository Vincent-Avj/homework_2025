#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8

#define NUM_MEMBERS 4
#define NUM_TESTS 7
#define NUM_DAYS 6
#define NUM_EXERCISE_TYPES 11
#define MAX_ROUTINE_LEN 256

const char* MAIN_MENU[NUM_MAIN_MENU_ITEMS] = {
    "I. Audition Management", "II. Training", "III. Debut"
};

const char* TRAINING_MENU[NUM_TRAINING_STAGES] = {
    "1. Physical Strength & Knowledge", "2. Self-Management & Teamwork",
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
const char* DAYS_OF_WEEK[NUM_DAYS] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};


const char* EXERCISE_TYPES[NUM_EXERCISE_TYPES] = {
    "Running", "Cycling", "Fast Walking",       
    "Push-ups", "Squats",                       
    "Leg Press", "Leg Curl",                   
    "Pull-ups", "Chin-ups",                    
    "Plank", "Crunches"                         
};
const char* EXERCISE_CATEGORIES[NUM_EXERCISE_TYPES] = {
    "Cardio", "Cardio", "Cardio", "Strength", "Strength", "Strength", 
    "Strength", "Strength", "Strength", "Core", "Core"
};


void display_main_menu();
void display_training_menu(const char status[]);
void handle_training_menu(char status[], float scores[][NUM_TESTS], char routines[][NUM_DAYS][MAX_ROUTINE_LEN]);
void handle_stage1_submenu(char* stage_status, float scores[][NUM_TESTS], char routines[][NUM_DAYS][MAX_ROUTINE_LEN]);
void setHealth(float scores[][NUM_TESTS]);
void getHealth(const float scores[][NUM_TESTS]);
void setExerciseRoutine(char routines[][NUM_DAYS][MAX_ROUTINE_LEN]);
int is_routine_valid(char* routine_str, int* core_used_this_week);
void getExerciseRoutine(const char routines[][NUM_DAYS][MAX_ROUTINE_LEN]);
void display_member_list(); 

int main() {
    char training_status[NUM_TRAINING_STAGES] = {0};
    float health_scores[NUM_MEMBERS][NUM_TESTS] = {{0.0f}};

    char member_routines[NUM_MEMBERS][NUM_DAYS][MAX_ROUTINE_LEN] = {{{0}}};
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
                handle_training_menu(training_status, health_scores, member_routines);
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


void display_member_list() {
    printf("\n--- Milliways Member List ---\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("%d. %s (%s)\n", i + 1, MILLIWAYS_NAMES[i], MILLIWAYS_NICKNAMES[i]);
    }
    printf("-----------------------------\n");
}

void handle_training_menu(char status[], float scores[][NUM_TESTS], char routines[][NUM_DAYS][MAX_ROUTINE_LEN]) {

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
            handle_stage1_submenu(&status[0], scores, routines);
        } else if (choice > 1 && choice <= NUM_TRAINING_STAGES) {
            printf("\nThis training stage is not yet implemented.\n");
        } else {
            printf("\nInvalid stage. Please select a valid number.\n");
        }
    }
}

void handle_stage1_submenu(char* stage_status, float scores[][NUM_TESTS], char routines[][NUM_DAYS][MAX_ROUTINE_LEN]) {
    char choice_char;
    while(1) {
        printf("\n--- [Stage 1: Physical Strength & Knowledge] ---\n");
        printf("A. Enter Fitness Data\n");
        printf("B. View Fitness Data\n");
        printf("C. Set Basic Workout Routine\n");
        printf("D. View Basic Workout Routine\n");
        printf("E. Certify Stage 1 Completion\n");
        printf("0. Return to Training Menu\n");
        printf("> Select an option: ");
        scanf(" %c", &choice_char);
        while(getchar() != '\n');

        if (choice_char == '0') break;

        switch(choice_char) {
            case 'A': case 'a': setHealth(scores); break;
            case 'B': case 'b': getHealth(scores); break;
            case 'C': case 'c': setExerciseRoutine(routines); break;
            case 'D': case 'd': getExerciseRoutine(routines); break;
            case 'E': case 'e':
                if (*stage_status == 'P') {
                    printf("\nStage 1 has already been passed.\n");
                } else {
                    printf("Did the members pass the certification for Stage 1? (Y/N): ");
                    scanf(" %c", &choice_char);
                    while(getchar() != '\n');
                    *stage_status = (choice_char == 'Y' || choice_char == 'y') ? 'P' : 'F';
                    printf("Result recorded: [%s]\n", *stage_status == 'P' ? "PASSED" : "FAILED");
                }
                break;
            default:
                printf("\nInvalid option. Please choose A, B, C, D, E, or 0.\n");
        }
    }
}


void getHealth(const float scores[][NUM_TESTS]) {
    display_member_list(); 

}
void setHealth(float scores[][NUM_TESTS]) { /* ... Same as previous ... */ }





int is_routine_valid(char* routine_str, int* core_used_this_week) {
    char buffer[MAX_ROUTINE_LEN];
    strcpy(buffer, routine_str); 

    int has_cardio = 0, has_strength_or_core = 0, uses_core_today = 0;
    char* token = strtok(buffer, ", ");

    while (token) {
        int found = 0;
        for (int i = 0; i < NUM_EXERCISE_TYPES; i++) {
            if (strcmp(token, EXERCISE_TYPES[i]) == 0) {
                found = 1;
                if (strcmp(EXERCISE_CATEGORIES[i], "Cardio") == 0) has_cardio = 1;
                if (strcmp(EXERCISE_CATEGORIES[i], "Strength") == 0) has_strength_or_core = 1;
                if (strcmp(EXERCISE_CATEGORIES[i], "Core") == 0) {
                    if (*core_used_this_week) {
                        printf("Error: Core exercises can only be used once per week. You have already scheduled one.\n");
                        return 0; 
                    }
                    has_strength_or_core = 1;
                    uses_core_today = 1;
                }
                break;
            }
        }
        if (!found) {
            printf("Error: '%s' is not a recognized exercise.\n", token);
            return 0;
        }
        token = strtok(NULL, ", ");
    }

    if (!has_cardio) {
        printf("Error: Routine must include at least one cardio exercise.\n");
        return 0;
    }
    if (!has_strength_or_core) {
        printf("Error: Routine must include at least one strength or core exercise.\n");
        return 0;
    }

    if (uses_core_today) {
        *core_used_this_week = 1;
    }
    return 1; 
}

void setExerciseRoutine(char routines[][NUM_DAYS][MAX_ROUTINE_LEN]) {
    char input_buffer[MAX_ROUTINE_LEN];
    printf("\n--- [Set Basic Workout Routine] ---\n");
    display_member_list();
    
    for (int i = 0; i < NUM_MEMBERS; i++) {
        int core_used_this_week = 0; 
        printf("\n--- Setting routine for %s (%s) ---\n", MILLIWAYS_NAMES[i], MILLIWAYS_NICKNAMES[i]);
        
        for (int j = 0; j < NUM_DAYS; j++) {
            int valid_input = 0;
            do {
                printf("\nAvailable Exercises: ");
                for(int k=0; k<NUM_EXERCISE_TYPES; k++) printf("%s%s", EXERCISE_TYPES[k], (k==NUM_EXERCISE_TYPES-1)?"\n":", ");
                
                printf("Enter comma-separated routine for %s: ", DAYS_OF_WEEK[j]);
                fgets(input_buffer, sizeof(input_buffer), stdin);
                input_buffer[strcspn(input_buffer, "\n")] = 0; 
                if (is_routine_valid(input_buffer, &core_used_this_week)) {
                    strcpy(routines[i][j], input_buffer);
                    printf("Routine for %s saved.\n", DAYS_OF_WEEK[j]);
                    valid_input = 1;
                } else {
                    printf("Please re-enter the routine for %s, following the rules.\n", DAYS_OF_WEEK[j]);
                }
            } while (!valid_input);
        }
    }
    printf("\nAll workout routines have been set.\n");
}

void getExerciseRoutine(const char routines[][NUM_DAYS][MAX_ROUTINE_LEN]) {
    char name_buffer[100];
    printf("\n--- [View Basic Workout Routine] ---\n");
    display_member_list();
    printf("Enter the full name of the member to view their routine: ");
    fgets(name_buffer, sizeof(name_buffer), stdin);
    name_buffer[strcspn(name_buffer, "\n")] = 0; 

    int found_index = -1;
    for (int i = 0; i < NUM_MEMBERS; i++) {
        if (strcmp(name_buffer, MILLIWAYS_NAMES[i]) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        printf("\n--- Weekly Workout Routine for %s (%s) ---\n",
               MILLIWAYS_NAMES[found_index], MILLIWAYS_NICKNAMES[found_index]);
        for (int i = 0; i < NUM_DAYS; i++) {
            printf("%-10s: %s\n", DAYS_OF_WEEK[i], routines[found_index][i]);
        }
    } else {
        printf("Member with name '%s' not found.\n", name_buffer);
    }
}
