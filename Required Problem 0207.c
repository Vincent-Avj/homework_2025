/**
 * @file stage1.c
 * @brief A state-aware, hierarchical menu system for the Magrathea project.
 *
 * This version adds a comprehensive "Workout Routine" module. It allows for
 * detailed logging of daily workouts and includes a powerful analysis suite
 * to calculate statistics like total calories per day, most frequent exercise,
 * and the most efficient time slot for calorie burning (Bonus Feature).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Constants and Data Structures ---

#define MAX_ROUTINES 50 // The maximum number of workout entries we can store

// Structure to hold a single workout routine entry
typedef struct {
    char date[11];      // "YYYY-MM-DD"
    char weather[50];
    char time_slot[20]; // "Before Meal", "After Meal", "Pre-Performance"
    char exercise_type[100];
    int duration_min;
    int calories_burned;
} WorkoutRoutine;


// --- Global State Variables ---
WorkoutRoutine g_workout_log[MAX_ROUTINES];
int g_workout_log_count = 0;


// --- Forward Declarations for Menu System and Other Features ---
void clear_screen();
void show_physical_strength_menu();
// Dummy placeholders for functions from previous problems
void setHealth() { printf("\n(Placeholder for Enter Physical Condition)\n"); }
void getHealth() { printf("\n(Placeholder for View Physical Condition)\n"); }
void setExerciseRoutine() { printf("\n(Placeholder for Set Workout Routine)\n"); }
void getExerciseRoutine() { printf("\n(Placeholder for View Workout Routine)\n"); }
void setDietPlan() { printf("\n(Placeholder for Set Diet Plan)\n"); }
void viewDietPlan() { printf("\n(Placeholder for View Diet Plan)\n"); }
void setMyDailyFood() { printf("\n(Placeholder for Meal Log)\n"); }
void mngMyDailyCalorie() { printf("\n(Placeholder for Calorie Management)\n"); }


// --- Utility Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// --- Workout Routine Feature Functions ---

/**
 * @brief Displays all workout routines currently stored in the log.
 */
void displayAllRoutines() {
    printf("\n--- Full Workout Log ---\n");
    if (g_workout_log_count == 0) {
        printf("No workout routines have been logged yet.\n");
        return;
    }
    printf("===================================================================================================\n");
    printf("| %-10s | %-15s | %-15s | %-25s | %-8s | %-8s |\n", "Date", "Weather", "Time Slot", "Exercise", "Duration", "Calories");
    printf("---------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < g_workout_log_count; i++) {
        WorkoutRoutine* r = &g_workout_log[i];
        printf("| %-10s | %-15s | %-15s | %-25s | %-8d | %-8d |\n",
            r->date, r->weather, r->time_slot, r->exercise_type, r->duration_min, r->calories_burned);
    }
    printf("===================================================================================================\n");
}

/**
 * @brief The main function for inputting new workout routine data.
 */
void setMyWorkoutRoutine() {
    char buffer[256];
    clear_screen();
    printf("========================================\n");
    printf("      I. Enter Workout Routine\n");
    printf("========================================\n");

    while (g_workout_log_count < MAX_ROUTINES) {
        printf("\nEntering data for new routine (Entry %d/%d). Type 'done' in date to exit.\n", g_workout_log_count + 1, MAX_ROUTINES);
        
        WorkoutRoutine* r = &g_workout_log[g_workout_log_count];

        printf("  - Date (YYYY-MM-DD): "); fgets(r->date, sizeof(r->date), stdin); r->date[strcspn(r->date, "\n")] = 0;
        if (strcmp(r->date, "done") == 0) break;

        printf("  - Weather (e.g., Sunny): "); fgets(r->weather, sizeof(r->weather), stdin); r->weather[strcspn(r->weather, "\n")] = 0;
        printf("  - Time Slot (Before Meal, After Meal, Pre-Performance): "); fgets(r->time_slot, sizeof(r->time_slot), stdin); r->time_slot[strcspn(r->time_slot, "\n")] = 0;
        printf("  - Exercise Type: "); fgets(r->exercise_type, sizeof(r->exercise_type), stdin); r->exercise_type[strcspn(r->exercise_type, "\n")] = 0;

        printf("  - Duration (minutes): "); fgets(buffer, sizeof(buffer), stdin); r->duration_min = atoi(buffer);
        printf("  - Calories Burned: "); fgets(buffer, sizeof(buffer), stdin); r->calories_burned = atoi(buffer);
        
        g_workout_log_count++;
        printf("--- Routine Logged Successfully! ---\n");
    }

    displayAllRoutines();
}

// --- Analysis Sub-Functions ---

void analyze_calories_per_day() {
    printf("\n--- Analysis: Total Calories Burned Per Day ---\n");
    if (g_workout_log_count == 0) { printf("No data to analyze.\n"); return; }
    
    char unique_dates[MAX_ROUTINES][11];
    int unique_count = 0;
    
    // Find all unique dates
    for(int i=0; i<g_workout_log_count; i++) {
        int found = 0;
        for(int j=0; j<unique_count; j++) {
            if(strcmp(g_workout_log[i].date, unique_dates[j]) == 0) {
                found = 1;
                break;
            }
        }
        if(!found) strcpy(unique_dates[unique_count++], g_workout_log[i].date);
    }

    // For each unique date, sum up the calories
    for(int i=0; i<unique_count; i++) {
        int total_calories = 0;
        for(int j=0; j<g_workout_log_count; j++) {
            if(strcmp(unique_dates[i], g_workout_log[j].date) == 0) {
                total_calories += g_workout_log[j].calories_burned;
            }
        }
        printf("  - Date: %s, Total Calories Burned: %d\n", unique_dates[i], total_calories);
    }
}

void analyze_most_frequent_exercise() {
    printf("\n--- Analysis: Most Frequent Exercise ---\n");
    if (g_workout_log_count == 0) { printf("No data to analyze.\n"); return; }
    
    char unique_exercises[MAX_ROUTINES][100];
    int counts[MAX_ROUTINES] = {0};
    int unique_count = 0;

    for (int i = 0; i < g_workout_log_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(g_workout_log[i].exercise_type, unique_exercises[j]) == 0) {
                counts[j]++; found = 1; break;
            }
        }
        if (!found) {
            strcpy(unique_exercises[unique_count], g_workout_log[i].exercise_type);
            counts[unique_count] = 1;
            unique_count++;
        }
    }

    int max_count = 0;
    char most_frequent[100] = "N/A";
    for (int i = 0; i < unique_count; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            strcpy(most_frequent, unique_exercises[i]);
        }
    }
    printf("Most Frequent Exercise: %s (%d times)\n", most_frequent, max_count);
}

// (Bonus) Analyze most efficient time slot
void analyze_most_efficient_timeslot() {
    printf("\n--- Analysis: Most Efficient Time Slot (Highest Avg Calorie Burn) ---\n");
    if (g_workout_log_count == 0) { printf("No data to analyze.\n"); return; }

    const char* slots[] = {"Before Meal", "After Meal", "Pre-Performance"};
    double total_cals[3] = {0.0, 0.0, 0.0};
    int counts[3] = {0, 0, 0};

    for (int i = 0; i < g_workout_log_count; i++) {
        if (strcmp(g_workout_log[i].time_slot, slots[0]) == 0) {
            total_cals[0] += g_workout_log[i].calories_burned; counts[0]++;
        } else if (strcmp(g_workout_log[i].time_slot, slots[1]) == 0) {
            total_cals[1] += g_workout_log[i].calories_burned; counts[1]++;
        } else if (strcmp(g_workout_log[i].time_slot, slots[2]) == 0) {
            total_cals[2] += g_workout_log[i].calories_burned; counts[2]++;
        }
    }
    
    double max_avg = -1.0;
    int best_slot_idx = -1;
    for (int i=0; i<3; i++) {
        if (counts[i] > 0) {
            double avg = total_cals[i] / counts[i];
            if(avg > max_avg) { max_avg = avg; best_slot_idx = i; }
        }
    }
    
    if (best_slot_idx != -1) {
        printf("The most efficient time slot is '%s' with an average burn of %.1f calories.\n", slots[best_slot_idx], max_avg);
    } else {
        printf("Not enough data to determine an efficient time slot.\n");
    }
}


/**
 * @brief The main analysis hub, providing a sub-menu for various reports.
 */
void statsMyWorkoutRoutine() {
    if (g_workout_log_count == 0) {
        printf("\nNo data to analyze. Please use option 'I' to enter routines first.\n");
        return;
    }

    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("      J. Analyze Workout Routine\n");
        printf("========================================\n");
        printf("   1. Total Calories Burned Per Day\n");
        printf("   2. Most Frequent Exercise\n");
        printf("   3. Most Efficient Time Slot (Bonus)\n");
        printf("   0. Back to previous menu\n");
        printf("----------------------------------------\n");
        printf("Select an analysis option: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        int choice = atoi(choice_buf);

        if (choice == 0) break;

        switch(choice) {
            case 1: analyze_calories_per_day(); break;
            case 2: analyze_most_frequent_exercise(); break;
            case 3: analyze_most_efficient_timeslot(); break;
            default: printf("Invalid option.\n"); break;
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Menu System ---
const char* HEALTH_MENU_ITEMS[] = {
    "A-F. (Previous Features)", "G. Meal Log", "H. Calorie Management",
    "I. Enter Workout Routine", "J. Analyze Workout Routine"
};
const int NUM_HEALTH_MENU_ITEMS = sizeof(HEALTH_MENU_ITEMS) / sizeof(char*);

void show_physical_strength_menu() {
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < NUM_HEALTH_MENU_ITEMS; i++) printf("   %s\n", HEALTH_MENU_ITEMS[i]);
        printf("   0. Back to Main Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        char choice = toupper(choice_buf[0]);

        if (choice == '0') break;

        switch(choice) {
            case 'G': setMyDailyFood(); break;
            case 'H': mngMyDailyCalorie(); break;
            case 'I': setMyWorkoutRoutine(); break;
            case 'J': statsMyWorkoutRoutine(); break;
            default: printf("\n(This is a placeholder for a previous feature.)\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


int main(void) {
    srand(time(NULL));
    show_physical_strength_menu();
    return 0;
}
