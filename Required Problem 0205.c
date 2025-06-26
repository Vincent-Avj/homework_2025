/**
 * @file main.c
 * @brief A self-contained version of the "Meal Log" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. The core
 * of this feature is a Circular Queue used to log daily meals with strict
 * validation rules, fulfilling the bonus requirement.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>   // FIX: Added to declare time() for srand()

// --- Constants and Data Structures ---

#define MAX_MEALS 7             // The capacity of our queue
#define DAILY_CALORIE_LIMIT 1500

// A structure to hold a single meal entry
typedef struct {
    char food_name[100];
    int calories;
} Meal;

// (Bonus) The Circular Queue data structure implemented with an array
typedef struct {
    Meal items[MAX_MEALS];
    int front;
    int rear;
    int size;
} CircularQueue;


// --- Forward Declarations for Menu System ---
void clear_screen();
void show_physical_strength_menu();


// --- Utility Functions ---
/**
 * @brief Clears the console screen in a cross-platform way.
 * FIX: This function was added to resolve the "undefined reference" linker error.
 */
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// --- (Bonus) Circular Queue Operations ---

/**
 * @brief Initializes a new, empty queue.
 * @param q A pointer to the queue to initialize.
 */
void initQueue(CircularQueue* q) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
}

/**
 * @brief Checks if the queue is full.
 * @param q A pointer to the queue.
 * @return 1 if full, 0 otherwise.
 */
int isFull(const CircularQueue* q) {
    return q->size == MAX_MEALS;
}

/**
 * @brief Checks if the queue is empty.
 * @param q A pointer to the queue.
 * @return 1 if empty, 0 otherwise.
 */
int isEmpty(const CircularQueue* q) {
    return q->size == 0;
}

/**
 * @brief Adds a new meal to the rear of the queue.
 * @param q A pointer to the queue.
 * @param item The Meal to add.
 */
void enqueue(CircularQueue* q, Meal item) {
    // Bonus Evaluation: Check for a full state
    if (isFull(q)) {
        printf("[SYSTEM] The meal log is full for today (max 7 meals).\n");
        return;
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    // Increment rear with wrap-around using the modulo operator
    q->rear = (q->rear + 1) % MAX_MEALS;
    q->items[q->rear] = item;
    q->size++;
    printf("-> Meal logged: %s (%d kcal)\n", item.food_name, item.calories);
}

/**
 * @brief Displays the current contents of the meal log queue.
 * @param q A pointer to the queue.
 */
void displayQueue(const CircularQueue* q) {
    printf("\n--- Today's Meal Log (%d/%d) ---\n", q->size, MAX_MEALS);
    // Bonus Evaluation: Check for an empty state
    if (isEmpty(q)) {
        printf("No meals logged yet.\n");
        return;
    }
    
    // Loop from the front for 'size' items, handling wrap-around
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->size;
        printf("  %d. %s (%d kcal)\n", i + 1, q->items[index].food_name, q->items[index].calories);
    }
    printf("--------------------------------\n");
}


// --- Main Feature: Meal Log Function ---

/**
 * @brief Manages the process of logging daily meals and calories.
 * @return The total calories consumed during the session.
 */
int setMyDailyFood() {
    CircularQueue meal_log;
    initQueue(&meal_log);

    int total_calories = 0;
    char buffer[100];

    clear_screen();
    printf("========================================\n");
    printf("        G. Daily Meal Log\n");
    printf("========================================\n");
    printf("Log up to %d meals. Total must not exceed %d kcal.\n", MAX_MEALS, DAILY_CALORIE_LIMIT);
    printf("Allowed calories per meal: 100, 150, 200, 250, 300\n");

    // Loop until the log is full or the user quits
    while (!isFull(&meal_log)) {
        Meal current_meal;
        int temp_calories;

        printf("\nEnter food name (or type 'done' to finish for the day): ");
        fgets(current_meal.food_name, sizeof(current_meal.food_name), stdin);
        current_meal.food_name[strcspn(current_meal.food_name, "\n")] = 0;

        if (strcmp(current_meal.food_name, "done") == 0) {
            break; // Exit loop if user is finished
        }

        // --- Get and Validate Calorie Input ---
        while (1) {
            printf("Enter calories for %s: ", current_meal.food_name);
            fgets(buffer, sizeof(buffer), stdin);
            temp_calories = atoi(buffer);

            // Validation 1: Check if the calorie value is one of the allowed options
            if (temp_calories == 100 || temp_calories == 150 || temp_calories == 200 ||
                temp_calories == 250 || temp_calories == 300) {
                // Validation 2: Check if adding this would exceed the daily total
                if (total_calories + temp_calories > DAILY_CALORIE_LIMIT) {
                    printf("\nError: Adding this meal would exceed the %d kcal daily limit.\n", DAILY_CALORIE_LIMIT);
                    goto end_of_input;
                }
                current_meal.calories = temp_calories;
                break; // Valid input, exit calorie loop
            } else {
                printf("Invalid calorie amount. Please choose from 100, 150, 200, 250, or 300.\n");
            }
        }

        // --- Add to Log ---
        enqueue(&meal_log, current_meal);
        total_calories += current_meal.calories;

        displayQueue(&meal_log);
        printf("Current Total: %d / %d kcal\n", total_calories, DAILY_CALORIE_LIMIT);
    }

end_of_input:
    printf("\n========================================\n");
    printf("         End of Day Summary\n");
    printf("========================================\n");
    displayQueue(&meal_log);
    printf("Final Total Calories Consumed: %d kcal\n", total_calories);
    
    return total_calories;
}


// --- Menu System ---
const char* HEALTH_MENU_ITEMS[] = {
    "A. Enter Physical Condition", "B. View Physical Condition",
    "C. Set Workout Routine", "D. View Workout Routine",
    "E. Set Diet Plan", "F. View Diet Plan", "G. Meal Log"
};
const int NUM_HEALTH_MENU_ITEMS = sizeof(HEALTH_MENU_ITEMS) / sizeof(char*);

// Dummy functions for other menu items
void setHealth() { printf("\n(Placeholder for Enter Physical Condition)\n"); }
void getHealth() { printf("\n(Placeholder for View Physical Condition)\n"); }
void setExerciseRoutine() { printf("\n(Placeholder for Set Workout Routine)\n"); }
void getExerciseRoutine() { printf("\n(Placeholder for View Workout Routine)\n"); }
void setDietPlan() { printf("\n(Placeholder for Set Diet Plan)\n"); }
void viewDietPlan() { printf("\n(Placeholder for View Diet Plan)\n"); }


void show_physical_strength_menu() {
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < NUM_HEALTH_MENU_ITEMS; i++) {
            printf("   %s\n", HEALTH_MENU_ITEMS[i]);
        }
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
            case 'G': setMyDailyFood(); break; // New feature
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

void run_training_system() {
    show_physical_strength_menu();
}

int main(void) {
    srand(time(NULL)); // Seed random number generator
    run_training_system(); // Directly go to the relevant menu for this problem
    return 0;
}
