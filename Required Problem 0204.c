/**
 * @file stage1.c
 * @brief The complete and integrated code for all "Stage 1" training modules.
 *
 * This program is the final version, combining the features from all previous
 * problems:
 * - A state-aware, hierarchical menu system.
 * - Full feature for entering and viewing fitness data.
 * - Full feature for setting and viewing validated workout routines.
 * - Placeholder stubs for the new diet plan features.
 * The menu system is data-driven, using an array of strings as required.
 */

#include <stdio.h>
#include <stdlib.h> // For system(), atoi(), atof()
#include <string.h> // For strcmp(), strcspn(), strtok(), etc.
#include <ctype.h>  // For toupper()

// --- Constants and Global Data ---

#define NUM_MEMBERS 4
#define NUM_FITNESS_TESTS 7
#define DAYS_OF_WEEK 6
#define MAX_EXERCISES_PER_DAY 5

// --- Data Structures ---

// For Fitness Data feature
typedef struct {
    char nickname[50];
    float scores[NUM_FITNESS_TESTS];
} FitnessRecord;

// For Workout Routine feature
typedef struct {
    char exercises[MAX_EXERCISES_PER_DAY][50];
    int num_exercises;
} DailyRoutine;

typedef struct {
    char nickname[50];
    DailyRoutine weekly_plan[DAYS_OF_WEEK];
} MemberRoutine;


// --- Global State Variables ---

FitnessRecord g_health_scores[NUM_MEMBERS];
int g_fitness_data_entered = 0;
MemberRoutine g_member_routines[NUM_MEMBERS];
int g_routines_set = 0;

// Static, predefined data for members and tests
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

// Categorized exercise data for the Workout Routine feature
const char* CARDIO_EXERCISES[] = {"Running", "Cycling", "Fast Walking"};
const char* STRENGTH_EXERCISES[] = {"Push-ups", "Squats", "Leg Press", "Leg Curl", "Pull-ups", "Chin-ups"};
const char* CORE_EXERCISES[] = {"Plank", "Crunches"};
const int NUM_CARDIO = sizeof(CARDIO_EXERCISES)/sizeof(char*);
const int NUM_STRENGTH = sizeof(STRENGTH_EXERCISES)/sizeof(char*);
const int NUM_CORE = sizeof(CORE_EXERCISES)/sizeof(char*);

// Menu items for the Health & Knowledge section stored in an array
const char* HEALTH_MENU_ITEMS[] = {
    "A. Enter Physical Condition", "B. View Physical Condition",
    "C. Set Workout Routine", "D. View Workout Routine",
    "E. Set Diet Plan", "F. View Diet Plan"
};
const int NUM_HEALTH_MENU_ITEMS = sizeof(HEALTH_MENU_ITEMS) / sizeof(char*);


// --- Utility and Feature Functions ---

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief A shared function to display the Milliways member list.
 */
void display_member_list() {
    printf("\nMilliways Member List:\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("  - %s (%s)\n", milliways_members[i][0], milliways_members[i][1]);
    }
}

/**
 * @brief Parses a comma-separated string of scores and populates a float array.
 */
void parse_and_store_scores(char* input_str, float* scores) {
    char* token = strtok(input_str, ",");
    int i = 0;
    while (token != NULL && i < NUM_FITNESS_TESTS) {
        scores[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }
}

/**
 * @brief Full implementation for entering fitness data.
 */
void setHealth() {
    clear_screen();
    printf("========================================\n");
    printf("      A. Enter Physical Condition\n");
    printf("========================================\n");
    printf("Enter the 7 test results for each member as a comma-separated list.\n");
    printf("e.g., 5.5,12.3,1.2,2.0,2.5,8.0,0.75\n\n");

    char input_buffer[256];
    for (int i = 0; i < NUM_MEMBERS; i++) {
        strcpy(g_health_scores[i].nickname, milliways_members[i][1]);
        printf("Enter scores for %s (%s):\n> ", milliways_members[i][0], milliways_members[i][1]);
        fgets(input_buffer, sizeof(input_buffer), stdin);
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        parse_and_store_scores(input_buffer, g_health_scores[i].scores);
    }
    
    g_fitness_data_entered = 1;
    printf("\nAll fitness data has been successfully recorded.\n");
}

/**
 * @brief Full implementation for viewing fitness data.
 */
void getHealth() {
    clear_screen();
    printf("========================================\n");
    printf("      B. View Physical Condition\n");
    printf("========================================\n");

    if (!g_fitness_data_entered) {
        printf("No fitness data has been entered yet. Please use option 'A' first.\n");
        return;
    }
    display_member_list();
    
    printf("\nEnter nickname to view their full report: ");
    char nickname_buf[50];
    fgets(nickname_buf, sizeof(nickname_buf), stdin);
    nickname_buf[strcspn(nickname_buf, "\n")] = 0;

    int member_idx = -1;
    for(int i=0; i<NUM_MEMBERS; i++)
        if(strcmp(nickname_buf, g_health_scores[i].nickname) == 0) member_idx = i;

    if(member_idx != -1) {
        printf("\n--- Detailed Report ---\n");
        printf("Member Name: %s\n", milliways_members[member_idx][0]);
        printf("Nickname:    %s\n", g_health_scores[member_idx].nickname);
        printf("--------------------------\n");
        for(int j=0; j<NUM_FITNESS_TESTS; j++)
            printf("  - %-30s: %.2f\n", FITNESS_TEST_NAMES[j], g_health_scores[member_idx].scores[j]);
    } else {
        printf("Member '%s' not found.\n", nickname_buf);
    }
}

// --- Workout Routine Functions ---
int is_exercise_type(const char* exercise, const char** type_list, int list_size) {
    for (int i = 0; i < list_size; i++) if (strcmp(exercise, type_list[i]) == 0) return 1;
    return 0;
}

void setExerciseRoutine() {
    clear_screen();
    printf("========================================\n");
    printf("      C. Set Basic Workout Routine\n");
    printf("========================================\n");
    const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    for (int i = 0; i < NUM_MEMBERS; i++) {
        int core_used_this_week = 0;
        strcpy(g_member_routines[i].nickname, milliways_members[i][1]);
        printf("\n--- Setting routine for %s (%s) ---\n", milliways_members[i][0], milliways_members[i][1]);
        for (int day = 0; day < DAYS_OF_WEEK; day++) {
            int is_day_valid = 0;
            do {
                g_member_routines[i].weekly_plan[day].num_exercises = 0;
                printf("\n-- %s's Routine -- (type 'done' to finish the day)\n", days[day]);
                while (g_member_routines[i].weekly_plan[day].num_exercises < MAX_EXERCISES_PER_DAY) {
                    char input_buf[50];
                    printf("> ");
                    fgets(input_buf, sizeof(input_buf), stdin);
                    input_buf[strcspn(input_buf, "\n")] = 0;
                    if (strcmp(input_buf, "done") == 0) break;
                    strcpy(g_member_routines[i].weekly_plan[day].exercises[g_member_routines[i].weekly_plan[day].num_exercises], input_buf);
                    g_member_routines[i].weekly_plan[day].num_exercises++;
                }
                
                int has_cardio = 0, has_strength_core = 0, day_has_core = 0;
                for (int k = 0; k < g_member_routines[i].weekly_plan[day].num_exercises; k++) {
                    char* ex = g_member_routines[i].weekly_plan[day].exercises[k];
                    if (is_exercise_type(ex, CARDIO_EXERCISES, NUM_CARDIO)) has_cardio = 1;
                    if (is_exercise_type(ex, STRENGTH_EXERCISES, NUM_STRENGTH)) has_strength_core = 1;
                    if (is_exercise_type(ex, CORE_EXERCISES, NUM_CORE)) { has_strength_core = 1; day_has_core = 1; }
                }
                
                if (has_cardio && has_strength_core && (!day_has_core || !core_used_this_week)) {
                    is_day_valid = 1;
                    if (day_has_core) core_used_this_week = 1;
                } else {
                    printf("\n** Invalid Plan! Rules: 1+ Cardio, 1+ Strength/Core, Core once per week. Please re-enter.**\n");
                }
            } while (!is_day_valid);
        }
    }
    g_routines_set = 1;
    printf("\nAll member routines have been successfully set.\n");
}

void getExerciseRoutine() {
    clear_screen();
    printf("========================================\n");
    printf("      D. View Basic Workout Routine\n");
    printf("========================================\n");
    if (!g_routines_set) { printf("No workout routines have been set yet.\n"); return; }
    display_member_list();
    printf("\nEnter nickname to view their routine: ");
    char nickname_buf[50];
    fgets(nickname_buf, sizeof(nickname_buf), stdin);
    nickname_buf[strcspn(nickname_buf, "\n")] = 0;

    int member_idx = -1;
    for(int i=0; i<NUM_MEMBERS; i++) if(strcmp(nickname_buf, g_member_routines[i].nickname) == 0) member_idx = i;

    if (member_idx != -1) {
        printf("\n--- Weekly Workout Routine for %s (%s) ---\n", milliways_members[member_idx][0], g_member_routines[member_idx].nickname);
        const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        for(int day=0; day < DAYS_OF_WEEK; day++) {
            printf("  [%s]\n", days[day]);
            for(int k=0; k < g_member_routines[member_idx].weekly_plan[day].num_exercises; k++) {
                printf("    - %s\n", g_member_routines[member_idx].weekly_plan[day].exercises[k]);
            }
        }
    } else {
        printf("Member '%s' not found.\n", nickname_buf);
    }
}

// New placeholder functions for the Diet Plan feature
void setDietPlan() { printf("\n(This would open the 'Set Diet Plan' screen.)\n"); }
void viewDietPlan() { printf("\n(This would open the 'View Diet Plan' screen.)\n"); }


// --- Menu System ---

void show_physical_strength_menu() {
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < NUM_HEALTH_MENU_ITEMS; i++) printf("   %s\n", HEALTH_MENU_ITEMS[i]);
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        char choice = toupper(choice_buf[0]);

        if (choice == '0') break;

        switch(choice) {
            case 'A': setHealth(); break;
            case 'B': getHealth(); break;
            case 'C': setExerciseRoutine(); break;
            case 'D': getExerciseRoutine(); break;
            case 'E': setDietPlan(); break;
            case 'F': viewDietPlan(); break;
            default: printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

void run_training_system() { show_physical_strength_menu(); }


int main(void) {
    while (1) {
        clear_screen();
        printf("========================================\n");
        printf("        MAGRATHEA Main Menu\n");
        printf("========================================\n");
        printf("   II. Training\n");
        printf("   0. Exit Program\n");
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
