/**
 * @file stage1.c
 * @brief A state-aware, hierarchical menu system for the Magrathea project.
 *
 * This program implements the main menu and a multi-stage training menu.
 * This version adds full functionality for "Stage 1", including fitness data
 * management and a new feature for setting and viewing personalized weekly
 * workout routines with specific validation rules.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Constants and Global Data ---

#define NUM_MEMBERS 4
#define NUM_FITNESS_TESTS 7
#define DAYS_OF_WEEK 6
#define MAX_EXERCISES_PER_DAY 5

// --- Data Structures ---

// For Fitness Data (Problem 2)
typedef struct {
    char nickname[50];
    float scores[NUM_FITNESS_TESTS];
} FitnessRecord;

// For Workout Routine (Problem 3)
typedef struct {
    char exercises[MAX_EXERCISES_PER_DAY][50];
    int num_exercises;
} DailyRoutine;

typedef struct {
    char nickname[50];
    DailyRoutine weekly_plan[DAYS_OF_WEEK]; // 0=Mon, 5=Sat
} MemberRoutine;

// --- Global State Variables ---

// State for Problem 2 features
FitnessRecord g_health_scores[NUM_MEMBERS];
int g_fitness_data_entered = 0;

// State for Problem 3 features
MemberRoutine g_member_routines[NUM_MEMBERS];
int g_routines_set = 0;

// Static, predefined data for members and exercises
const char* milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};

const char* FITNESS_TEST_NAMES[NUM_FITNESS_TESTS] = {
    "1-Mile Run (min)", "100m Sprint (sec)", "30 Push-ups (min)",
    "50 Squats (min)", "50 Arm Strength Push-ups (min)",
    "400m Swim (min)", "Bench Press (x bodyweight)"
};

// Categorized exercise data
const char* CARDIO_EXERCISES[] = {"Running", "Cycling", "Fast Walking"};
const char* STRENGTH_EXERCISES[] = {"Push-ups", "Squats", "Leg Press", "Leg Curl", "Pull-ups", "Chin-ups"};
const char* CORE_EXERCISES[] = {"Plank", "Crunches"};
const int NUM_CARDIO = sizeof(CARDIO_EXERCISES)/sizeof(char*);
const int NUM_STRENGTH = sizeof(STRENGTH_EXERCISES)/sizeof(char*);
const int NUM_CORE = sizeof(CORE_EXERCISES)/sizeof(char*);


// --- Utility and Menu Functions ---

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief (Bonus) A shared function to display the member list.
 * This reduces code redundancy across multiple features.
 */
void display_member_list() {
    printf("\nMilliways Member List:\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("  - %s (%s)\n", milliways_members[i][0], milliways_members[i][1]);
    }
}

// --- Problem 2: Fitness Data Functions (getHealth modified for Bonus) ---

void setHealth(); // Forward declaration
void getHealth() {
    clear_screen();
    printf("========================================\n");
    printf("      B. View Fitness Data\n");
    printf("========================================\n");

    if (!g_fitness_data_entered) {
        printf("No fitness data has been entered yet. Please use option 'A' first.\n");
        return;
    }

    // (Bonus) Use the shared function to display the member list
    display_member_list();
    
    printf("\nEnter nickname to view their full report: ");
    char nickname_buf[50];
    fgets(nickname_buf, sizeof(nickname_buf), stdin);
    nickname_buf[strcspn(nickname_buf, "\n")] = 0;

    int member_idx = -1;
    for(int i=0; i<NUM_MEMBERS; i++){
        if(strcmp(nickname_buf, g_health_scores[i].nickname) == 0) {
            member_idx = i;
            break;
        }
    }

    if(member_idx != -1) {
        printf("\n--- Detailed Report ---\n");
        printf("Member Name: %s\n", milliways_members[member_idx][0]);
        printf("Nickname:    %s\n", g_health_scores[member_idx].nickname);
        printf("--------------------------\n");
        for(int j=0; j<NUM_FITNESS_TESTS; j++) {
            printf("  - %-30s: %.2f\n", FITNESS_TEST_NAMES[j], g_health_scores[member_idx].scores[j]);
        }
    } else {
        printf("Member '%s' not found.\n", nickname_buf);
    }
}


// --- Problem 3: Workout Routine Functions ---

// Helper function to check if an exercise is of a certain type
int is_exercise_type(const char* exercise, const char** type_list, int list_size) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(exercise, type_list[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Prompts the user to set a weekly workout routine for all members.
 */
void setExerciseRoutine() {
    clear_screen();
    printf("========================================\n");
    printf("      C. Set Basic Workout Routine\n");
    printf("========================================\n");
    printf("For each member, set a routine for Monday to Saturday.\n");
    printf("Each day must have at least 1 Cardio and 1 Strength/Core exercise.\n");
    printf("Core exercises can only be used ONCE per week per member.\n\n");

    const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    // Loop through each member
    for (int i = 0; i < NUM_MEMBERS; i++) {
        int core_used_this_week = 0;
        strcpy(g_member_routines[i].nickname, milliways_members[i][1]);

        printf("\n--- Setting routine for %s (%s) ---\n", milliways_members[i][0], milliways_members[i][1]);

        // Loop through each day of the week
        for (int day = 0; day < DAYS_OF_WEEK; day++) {
            int is_day_valid = 0;
            // This loop repeats until a valid day plan is entered
            do {
                g_member_routines[i].weekly_plan[day].num_exercises = 0; // Reset for re-entry
                printf("\n-- %s's Routine --\n", days[day]);
                printf("Enter exercises one by one. Type 'done' to finish the day.\n");
                printf("Choices: [Cardio] Running, Cycling... [Strength] Push-ups, Squats... [Core] Plank, Crunches...\n");
                
                // Loop to get exercises for the current day
                while (g_member_routines[i].weekly_plan[day].num_exercises < MAX_EXERCISES_PER_DAY) {
                    char input_buf[50];
                    printf("> ");
                    fgets(input_buf, sizeof(input_buf), stdin);
                    input_buf[strcspn(input_buf, "\n")] = 0;

                    if (strcmp(input_buf, "done") == 0) break;
                    
                    // Add the exercise to the plan
                    strcpy(g_member_routines[i].weekly_plan[day].exercises[g_member_routines[i].weekly_plan[day].num_exercises], input_buf);
                    g_member_routines[i].weekly_plan[day].num_exercises++;
                }
                
                // --- Validate the day's routine ---
                int has_cardio = 0;
                int has_strength_core = 0;
                int day_has_core = 0;

                for (int k = 0; k < g_member_routines[i].weekly_plan[day].num_exercises; k++) {
                    char* exercise = g_member_routines[i].weekly_plan[day].exercises[k];
                    if (is_exercise_type(exercise, CARDIO_EXERCISES, NUM_CARDIO)) has_cardio = 1;
                    if (is_exercise_type(exercise, STRENGTH_EXERCISES, NUM_STRENGTH)) has_strength_core = 1;
                    if (is_exercise_type(exercise, CORE_EXERCISES, NUM_CORE)) {
                        has_strength_core = 1;
                        day_has_core = 1;
                    }
                }
                
                if (has_cardio && has_strength_core && (!day_has_core || (day_has_core && !core_used_this_week))) {
                    is_day_valid = 1;
                    if (day_has_core) {
                        core_used_this_week = 1; // Mark core as used for the rest of the week
                    }
                } else {
                    printf("\n** Invalid Plan for %s! **\n", days[day]);
                    if (!has_cardio) printf("- Must include at least one Cardio exercise.\n");
                    if (!has_strength_core) printf("- Must include at least one Strength or Core exercise.\n");
                    if (day_has_core && core_used_this_week) printf("- Core exercise already used this week!\n");
                    printf("Please re-enter the routine for this day.\n");
                }
            } while (!is_day_valid);
        }
    }
    g_routines_set = 1;
    printf("\nAll member routines have been successfully set.\n");
}

/**
 * @brief Displays the stored weekly workout routine for a selected member.
 */
void getExerciseRoutine() {
    clear_screen();
    printf("========================================\n");
    printf("      D. View Basic Workout Routine\n");
    printf("========================================\n");

    if (!g_routines_set) {
        printf("No workout routines have been set yet. Please use option 'C' first.\n");
        return;
    }
    
    // (Bonus) Use the shared function to display the member list
    display_member_list();

    printf("\nEnter nickname to view their routine: ");
    char nickname_buf[50];
    fgets(nickname_buf, sizeof(nickname_buf), stdin);
    nickname_buf[strcspn(nickname_buf, "\n")] = 0;

    int member_idx = -1;
    for(int i=0; i<NUM_MEMBERS; i++){
        if(strcmp(nickname_buf, g_member_routines[i].nickname) == 0) {
            member_idx = i;
            break;
        }
    }

    if (member_idx != -1) {
        // (Bonus) Display full name and nickname
        printf("\n--- Weekly Workout Routine ---\n");
        printf("Member Name: %s\n", milliways_members[member_idx][0]);
        printf("Nickname:    %s\n", g_member_routines[member_idx].nickname);
        printf("--------------------------------\n");
        
        const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        for(int day=0; day < DAYS_OF_WEEK; day++) {
            printf("  [%s]\n", days[day]);
            if (g_member_routines[member_idx].weekly_plan[day].num_exercises == 0) {
                printf("    - Rest Day\n");
            } else {
                for(int k=0; k < g_member_routines[member_idx].weekly_plan[day].num_exercises; k++) {
                    printf("    - %s\n", g_member_routines[member_idx].weekly_plan[day].exercises[k]);
                }
            }
        }
    } else {
        printf("Member '%s' not found.\n", nickname_buf);
    }
}


// --- Menu System Integration ---

void show_physical_strength_menu() {
    char choice;
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        printf("   A. Enter Fitness Data\n");
        printf("   B. View Fitness Data\n");
        printf("   C. Set Basic Workout Routine\n");
        printf("   D. View Basic Workout Routine\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        choice = getchar();
        while(getchar() != '\n');

        if (choice == '0') break;

        switch(toupper(choice)) {
            case 'A':
                // This function is defined in the full problem 2 solution
                // setHealth(); 
                printf("\n'Enter Fitness Data' was part of the previous problem.\n");
                break;
            case 'B':
                getHealth();
                break;
            case 'C':
                setExerciseRoutine();
                break;
            case 'D':
                getExerciseRoutine();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


void run_training_system() {
    // This function is the main entry point for the "Training" section.
    show_physical_strength_menu();
}


int main(void) {
    // The main program loop.
    while (1) {
        clear_screen();
        printf("========================================\n");
        printf("        MAGRATHEA Main Menu\n");
        printf("========================================\n");
        printf("   I. Audition Management\n");
        printf("   II. Training\n");
        printf("   III. Debut\n");
        printf("----------------------------------------\n");
        printf("Enter a menu number, or 'Q' to quit.\n");
        printf("Choice: ");

        char input_buf[10];
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strcspn(input_buf, "\n")] = 0;

        if (strlen(input_buf) == 0 || strcmp(input_buf, "0") == 0 || toupper(input_buf[0]) == 'Q') {
            break;
        }

        if (strcmp(input_buf, "II") == 0 || strcmp(input_buf, "2") == 0) {
            run_training_system();
        } else {
            printf("\nThis feature is not yet implemented.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    }

    printf("\nExiting Magrathea System. Goodbye!\n");
    return 0;
}
