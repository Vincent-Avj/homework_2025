/**
 * @file main.c
 * @brief A self-contained version of the "Meal Log" and "Calorie Management" features.
 *
 * This program combines all necessary functions into a single file. The core
 * of this feature is a Circular Queue used to log daily meals and a system
 * to "burn off" those meals with exercise, demonstrating the FIFO principle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>   // FIX 1: Added to declare time() for srand()

// --- Constants and Data Structures ---

#define MAX_MEALS 7
#define DAILY_CALORIE_LIMIT 1500

// A structure to hold a single meal entry
typedef struct {
    char food_name[100];
    int calories;
} Meal;

// The Circular Queue data structure implemented with an array
typedef struct {
    Meal items[MAX_MEALS];
    int front;
    int rear;
    int size;
} CircularQueue;

// A structure to hold a single exercise
typedef struct {
    const char* name;
    int calories_burned;
} Exercise;


// --- Global Data ---
const Exercise g_exercise_db[] = {
    {"30 minutes of fast walking or jogging", 200},
    {"Full-body strength training", 250},
    {"30 minutes of swimming or cycling", 300},
    {"30 minutes of stretching and yoga", 150},
    {"30 minutes of dance or aerobics", 250},
    {"Tennis, badminton, or dance", 300},
    {"Light walk", 100}
};
const int NUM_EXERCISES = sizeof(g_exercise_db) / sizeof(Exercise);

const char* HEALTH_MENU_ITEMS[] = {
    "A. Enter Physical Condition", "B. View Physical Condition",
    "C. Set Workout Routine", "D. View Workout Routine",
    "E. Set Diet Plan", "F. View Diet Plan", "G. Meal Log", "H. Calorie Management"
};
const int NUM_HEALTH_MENU_ITEMS = sizeof(HEALTH_MENU_ITEMS) / sizeof(char*);


// --- Forward Declarations ---
void clear_screen();
void show_physical_strength_menu();
int setMyDailyFood(CircularQueue* q);
void mngMyDailyCalorie(CircularQueue* q);


// --- Main Program Entry Point ---
int main(void) {
    srand(time(NULL)); // Seed random number generator
    show_physical_strength_menu(); // Directly go to the relevant menu for this problem
    printf("\nExiting Program.\n");
    return 0;
}


// --- Utility Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// --- Circular Queue Operations ---

void initQueue(CircularQueue* q) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
}

int isFull(const CircularQueue* q) { return q->size == MAX_MEALS; }
int isEmpty(const CircularQueue* q) { return q->size == 0; }

void enqueue(CircularQueue* q, Meal item) {
    if (isFull(q)) {
        printf("[SYSTEM] The meal log is full for today (max %d meals).\n", MAX_MEALS);
        return;
    }
    if (isEmpty(q)) q->front = 0;
    q->rear = (q->rear + 1) % MAX_MEALS;
    q->items[q->rear] = item;
    q->size++;
    printf("-> Meal logged: %s (%d kcal)\n", item.food_name, item.calories);
}

int dequeue(CircularQueue* q, Meal* p_item) {
    if (isEmpty(q)) return 0;
    *p_item = q->items[q->front];
    q->size--;
    if (isEmpty(q)) {
        initQueue(q);
    } else {
        q->front = (q->front + 1) % MAX_MEALS;
    }
    return 1;
}

int peek(const CircularQueue* q, Meal* p_item) {
    if (isEmpty(q)) return 0;
    *p_item = q->items[q->front];
    return 1;
}

void displayQueue(const CircularQueue* q) {
    printf("\n--- Current Meal Log (%d/%d) ---\n", q->size, MAX_MEALS);
    if (isEmpty(q)) {
        printf("The meal log is empty.\n");
        return;
    }
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_MEALS;
        printf("  %d. %s (%d kcal)\n", i + 1, q->items[index].food_name, q->items[index].calories);
    }
    printf("--------------------------------\n");
}


// --- Main Feature Functions ---

int setMyDailyFood(CircularQueue* meal_log) {
    initQueue(meal_log); // Start a fresh log for the day
    int total_calories = 0;
    char buffer[100];
    clear_screen();
    printf("========================================\n");
    printf("        G. Daily Meal Log\n");
    printf("========================================\n");
    printf("Log up to %d meals. Total must not exceed %d kcal.\n", MAX_MEALS, DAILY_CALORIE_LIMIT);
    
    while (!isFull(meal_log)) {
        Meal current_meal;
        printf("\nEnter food name (or 'done' to finish): ");
        fgets(current_meal.food_name, sizeof(current_meal.food_name), stdin);
        current_meal.food_name[strcspn(current_meal.food_name, "\n")] = 0;
        if (strcmp(current_meal.food_name, "done") == 0) break;
        
        while (1) {
            printf("Enter calories for %s (100, 150, 200, 250, 300): ", current_meal.food_name);
            fgets(buffer, sizeof(buffer), stdin);
            current_meal.calories = atoi(buffer);
            if (current_meal.calories == 100 || current_meal.calories == 150 || current_meal.calories == 200 || current_meal.calories == 250 || current_meal.calories == 300) {
                if (total_calories + current_meal.calories > DAILY_CALORIE_LIMIT) {
                    printf("\nError: Exceeds daily limit of %d kcal.\n", DAILY_CALORIE_LIMIT);
                    goto end_of_input;
                }
                break;
            } else {
                printf("Invalid calorie amount.\n");
            }
        }
        enqueue(meal_log, current_meal);
        total_calories += current_meal.calories;
        displayQueue(meal_log);
        printf("Current Total: %d / %d kcal\n", total_calories, DAILY_CALORIE_LIMIT);
    }

end_of_input:
    printf("\n--- End of Day Summary ---\n");
    printf("Final Total Calories Logged: %d kcal\n", total_calories);
    return total_calories;
}

void mngMyDailyCalorie(CircularQueue* meal_log) {
    clear_screen();
    printf("========================================\n");
    printf("      H. Calorie Management\n");
    printf("========================================\n");
    
    if (isEmpty(meal_log)) {
        printf("Your meal log is empty. Nothing to burn off!\n");
        return;
    }

    Meal target_meal;
    peek(meal_log, &target_meal);
    
    printf("Next meal to burn off: %s (%d kcal)\n", target_meal.food_name, target_meal.calories);
    printf("\nChoose an exercise you performed:\n");
    for(int i=0; i<NUM_EXERCISES; i++) {
        printf("  %d. %s (%d kcal)\n", i+1, g_exercise_db[i].name, g_exercise_db[i].calories_burned);
    }
    
    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    
    if (choice < 1 || choice > NUM_EXERCISES) {
        printf("Invalid choice.\n");
        return;
    }
    
    const Exercise* chosen_exercise = &g_exercise_db[choice-1];
    printf("\nYou performed: %s\n", chosen_exercise->name);
    
    if (chosen_exercise->calories_burned >= target_meal.calories) {
        printf("Success! You burned enough calories to offset the meal.\n");
        Meal burned_meal;
        dequeue(meal_log, &burned_meal);
        printf("Removed '%s' from your log.\n", burned_meal.food_name);
    } else {
        int deficit = target_meal.calories - chosen_exercise->calories_burned;
        printf("Insufficient exercise. The meal was not burned off.\n");
        printf("You need to burn %d more calories.\n", deficit);
    }
    
    displayQueue(meal_log);
}


// --- Menu System ---

// Dummy functions for other menu items
void setHealth() { printf("\n(Placeholder for Enter Physical Condition)\n"); }
void getHealth() { printf("\n(Placeholder for View Physical Condition)\n"); }
void setExerciseRoutine() { printf("\n(Placeholder for Set Workout Routine)\n"); }
void getExerciseRoutine() { printf("\n(Placeholder for View Workout Routine)\n"); }
void setDietPlan() { printf("\n(Placeholder for Set Diet Plan)\n"); }
void viewDietPlan() { printf("\n(Placeholder for View Diet Plan)\n"); }

void show_physical_strength_menu() {
    CircularQueue shared_meal_log;
    initQueue(&shared_meal_log);

    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < NUM_HEALTH_MENU_ITEMS; i++) {
            printf("   %s\n", HEALTH_MENU_ITEMS[i]);
        }
        printf("   0. Back to Main Menu\n");
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
            case 'G': setMyDailyFood(&shared_meal_log); break;
            // FIX 2: Corrected the typo from shared_mil_log to shared_meal_log
            case 'H': mngMyDailyCalorie(&shared_meal_log); break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}
