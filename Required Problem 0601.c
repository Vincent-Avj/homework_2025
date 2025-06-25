/**
 * @file main.c
 * @brief A self-contained version of the "Learn Basic Dance Steps" quiz for online compilers.
 *
 * This program combines all necessary functions from the multi-file project
 * into a single file. It features a memory quiz where users try to recall
 * the correct order of dance steps after a timed hint. The program includes
 * a fallback mechanism to use hardcoded data if the required input file is
 * not found, ensuring it can run in any environment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// For cross-platform sleep functionality
#ifdef _WIN32
#include <windows.h>
#define sleep_seconds(s) Sleep((s) * 1000)
#else
#include <unistd.h>
#define sleep_seconds(s) sleep(s)
#endif


// --- Constants and Data Structures ---
#define MAX_DANCES 10
#define MAX_STEPS_PER_DANCE 10
#define NUM_MEMBERS 4

// Structure to hold one complete dance
typedef struct {
    char korean_name[100];
    char english_name[100];
    char** steps; // Dynamic array of strings
    int step_count;
} Dance;

// Structure to hold a member's score
typedef struct {
    char nickname[50];
    int score;
} MemberScore;


// --- Global Data Storage ---
Dance* g_dance_db = NULL;
int g_dance_count = 0;
MemberScore g_member_scores[NUM_MEMBERS];
int g_scores_initialized = 0;

const char* g_milliways_nicknames[NUM_MEMBERS] = {"Ariel", "Simba", "Belle", "Aladdin"};


// --- Forward Declarations for All Functions ---
void clear_screen();
void cleanup_dance_data();
void show_main_menu();
void show_training_menu();
void show_dance_menu();
void learnDanceStep();


// --- Main Entry Point ---
int main() {
    // Register the cleanup function to be called on normal program termination
    atexit(cleanup_dance_data);
    
    // Seed the random number generator once at the start
    srand(time(NULL));

    printf("Welcome to the Magrathea Management System.\n");
    printf("Press Enter to start...");
    getchar();
    
    show_main_menu();
    
    printf("\nExiting program. Goodbye!\n");
    return 0;
}


// --- Menu System Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_dance_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Dance Training\n");
        printf("----------------------------------------\n");
        printf("   A. Learn Basic Dance Steps\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == 'A' || choice == 'a') {
            learnDanceStep();
        }
    } while (choice != '0');
}

void show_training_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Training\n");
        printf("----------------------------------------\n");
        printf("   5. Dance Training\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '5') {
            show_dance_menu();
        }
    } while (choice != '0');
}

void show_main_menu() {
    char choice;
    do {
        clear_screen();
        printf("========================================\n");
        printf("      Milliways Management System\n");
        printf("========================================\n");
        printf("   II. Training\n");
        printf("   0. Exit Program\n");
        printf("========================================\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == '2') {
             show_training_menu();
        }
    } while (choice != '0' && toupper(choice) != 'Q');
}


// --- Feature Logic Functions ---

/**
 * @brief Shuffles an array of strings using the Fisher-Yates algorithm.
 */
void shuffle_string_array(char** array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

/**
 * @brief Loads dance data from a file, with a hardcoded fallback for online compilers.
 */
void load_dance_data() {
    if (g_dance_db != NULL) return; // Already loaded

    FILE* file = fopen("dance_step.txt", "r");
    if (!file) {
        // --- Fallback for Online Compilers ---
        printf("Notice: 'dance_step.txt' not found. Using hardcoded data as a fallback.\n");
        
        g_dance_db = (Dance*)malloc(6 * sizeof(Dance));
        if(!g_dance_db) return;
        g_dance_count = 6;
        
        // Data 1
        strcpy(g_dance_db[0].korean_name, "웨이브"); strcpy(g_dance_db[0].english_name, "Wave");
        g_dance_db[0].steps = (char**)malloc(3 * sizeof(char*));
        g_dance_db[0].steps[0] = strdup("Arm Wave"); g_dance_db[0].steps[1] = strdup("Body Wave"); g_dance_db[0].steps[2] = strdup("Pop");
        g_dance_db[0].step_count = 3;
        // Data 2
        strcpy(g_dance_db[1].korean_name, "기본 스텝"); strcpy(g_dance_db[1].english_name, "Basic Step");
        g_dance_db[1].steps = (char**)malloc(4 * sizeof(char*));
        g_dance_db[1].steps[0] = strdup("Two-Step"); g_dance_db[1].steps[1] = strdup("Grapevine"); g_dance_db[1].steps[2] = strdup("Box Step"); g_dance_db[1].steps[3] = strdup("Jazz Square");
        g_dance_db[1].step_count = 4;
        // ... (Additional hardcoded data) ...
        strcpy(g_dance_db[2].korean_name, "슬라이드"); strcpy(g_dance_db[2].english_name, "Slide");
        g_dance_db[2].steps = (char**)malloc(3 * sizeof(char*));
        g_dance_db[2].steps[0] = strdup("Moonwalk"); g_dance_db[2].steps[1] = strdup("Side Glide"); g_dance_db[2].steps[2] = strdup("Circle Glide");
        g_dance_db[2].step_count = 3;
        strcpy(g_dance_db[3].korean_name, "팝핑"); strcpy(g_dance_db[3].english_name, "Popping");
        g_dance_db[3].steps = (char**)malloc(4 * sizeof(char*));
        g_dance_db[3].steps[0] = strdup("Hit"); g_dance_db[3].steps[1] = strdup("Dime Stop"); g_dance_db[3].steps[2] = strdup("Robot"); g_dance_db[3].steps[3] = strdup("Waving");
        g_dance_db[3].step_count = 4;
        strcpy(g_dance_db[4].korean_name, "락킹"); strcpy(g_dance_db[4].english_name, "Locking");
        g_dance_db[4].steps = (char**)malloc(5 * sizeof(char*));
        g_dance_db[4].steps[0] = strdup("Lock"); g_dance_db[4].steps[1] = strdup("Point"); g_dance_db[4].steps[2] = strdup("Pacing"); g_dance_db[4].steps[3] = strdup("Wrist Roll"); g_dance_db[4].steps[4] = strdup("Clap");
        g_dance_db[4].step_count = 5;
        strcpy(g_dance_db[5].korean_name, "턴"); strcpy(g_dance_db[5].english_name, "Turn");
        g_dance_db[5].steps = (char**)malloc(3 * sizeof(char*));
        g_dance_db[5].steps[0] = strdup("Pirouette"); g_dance_db[5].steps[1] = strdup("Chainé Turn"); g_dance_db[5].steps[2] = strdup("Fouetté Turn");
        g_dance_db[5].step_count = 3;
        return;
    }
    
    // --- Original File Loading Logic ---
    int lines = 0;
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), file)) lines++;
    rewind(file);

    g_dance_db = (Dance*)malloc(lines * sizeof(Dance));
    if(!g_dance_db) { fclose(file); return; }

    while(g_dance_count < lines && fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        Dance* p_dance = &g_dance_db[g_dance_count];
        p_dance->step_count = 0;

        char* kr_name = strtok(buffer, ";");
        char* en_name = strtok(NULL, ";");
        char* steps_str = strtok(NULL, "");
        if(!kr_name || !en_name || !steps_str) continue;
        
        strcpy(p_dance->korean_name, kr_name);
        strcpy(p_dance->english_name, en_name);
        
        p_dance->steps = (char**)malloc(MAX_STEPS_PER_DANCE * sizeof(char*));
        char* step_token = strtok(steps_str, ",");
        while(step_token != NULL && p_dance->step_count < MAX_STEPS_PER_DANCE) {
            p_dance->steps[p_dance->step_count] = strdup(step_token);
            p_dance->step_count++;
            step_token = strtok(NULL, ",");
        }
        g_dance_count++;
    }
    fclose(file);
}

/**
 * @brief (Bonus) Displays the answer key for 10 seconds, then clears.
 */
void displayHint(const Dance* p_dance) {
    printf("\n--- Memorize This! Disappearing in 10 seconds... ---\n");
    printf("Dance: %s (%s)\n", p_dance->korean_name, p_dance->english_name);
    printf("Correct Step Order:\n");
    for(int i=0; i<p_dance->step_count; i++) {
        printf("  %d. %s\n", i+1, p_dance->steps[i]);
    }
    sleep_seconds(10);
    clear_screen();
}

/**
 * @brief Evaluates the user's answer against the correct steps and returns a score.
 */
int evalScore(const Dance* p_dance, char user_steps[][50], int user_step_count) {
    if(user_step_count != p_dance->step_count) return 0;

    int perfect_match = 1;
    for(int i=0; i<user_step_count; i++){
        if(strcmp(p_dance->steps[i], user_steps[i]) != 0) {
            perfect_match = 0;
            break;
        }
    }
    if(perfect_match) return 100;
    
    int correct_step_found[MAX_STEPS_PER_DANCE] = {0};
    int all_words_match = 1;
    for(int i=0; i<user_step_count; i++) {
        int found_this_step = 0;
        for(int j=0; j<p_dance->step_count; j++) {
            if(!correct_step_found[j] && strcmp(user_steps[i], p_dance->steps[j]) == 0) {
                correct_step_found[j] = 1;
                found_this_step = 1;
                break;
            }
        }
        if(!found_this_step) {
            all_words_match = 0;
            break;
        }
    }
    if(all_words_match) return 50;

    for(int i=0; i<user_step_count; i++){
        for(int j=0; j<p_dance->step_count; j++){
            if(strcmp(user_steps[i], p_dance->steps[j]) == 0) return 20;
        }
    }

    return 0;
}

/**
 * @brief The main function for the dance step learning feature.
 */
void learnDanceStep() {
    clear_screen();
    printf("========================================\n");
    printf("        Learn Basic Dance Steps\n");
    printf("========================================\n");
    
    load_dance_data();
    if (g_dance_count == 0) { printf("Failed to load dance data.\n"); return; }
    if (!g_scores_initialized) {
        for(int i=0; i<NUM_MEMBERS; i++) strcpy(g_member_scores[i].nickname, g_milliways_nicknames[i]);
        g_scores_initialized = 1;
    }

    // Login
    char nickname[50];
    int member_idx = -1, attempts=0;
    while(attempts < 3) {
        printf("Enter your nickname to begin (Attempt %d/3): ", attempts+1);
        fgets(nickname, sizeof(nickname), stdin);
        nickname[strcspn(nickname, "\n")] = 0;
        for(int i=0; i<NUM_MEMBERS; i++){
            if(strcmp(nickname, g_milliways_nicknames[i]) == 0) member_idx = i;
        }
        if (member_idx != -1) break;
        printf("Nickname not found.\n");
        attempts++;
    }
    if(member_idx == -1) { printf("Too many failed attempts.\n"); return; }

    // Quiz Setup
    int q_idx = rand() % g_dance_count;
    Dance* p_quiz_dance = &g_dance_db[q_idx];

    // Bonus: Show hint screen
    displayHint(p_quiz_dance);
    
    // Present Puzzle
    printf("--- Let's Practice! ---\n");
    printf("The dance is: %s (%s)\n", p_quiz_dance->korean_name, p_quiz_dance->english_name);
    printf("Here are the steps in a random order:\n");
    
    char** shuffled_steps = (char**)malloc(p_quiz_dance->step_count * sizeof(char*));
    for(int i=0; i<p_quiz_dance->step_count; i++) shuffled_steps[i] = p_quiz_dance->steps[i];
    shuffle_string_array(shuffled_steps, p_quiz_dance->step_count);
    for(int i=0; i<p_quiz_dance->step_count; i++) printf("  - %s\n", shuffled_steps[i]);
    free(shuffled_steps);
    
    // Get user input
    printf("\nPlease enter the %d steps in the correct order, one per line:\n", p_quiz_dance->step_count);
    char user_steps[MAX_STEPS_PER_DANCE][50];
    for(int i=0; i<p_quiz_dance->step_count; i++){
        printf("Step %d: ", i+1);
        fgets(user_steps[i], 50, stdin);
        user_steps[i][strcspn(user_steps[i], "\n")] = 0;
    }
    
    // Evaluate and display results
    int score = evalScore(p_quiz_dance, user_steps, p_quiz_dance->step_count);
    g_member_scores[member_idx].score = score;
    
    printf("\n--- Results for %s ---\n", g_milliways_nicknames[member_idx]);
    printf("Correct Order: ");
    for(int i=0; i<p_quiz_dance->step_count; i++) printf("%s%s", p_quiz_dance->steps[i], (i==p_quiz_dance->step_count-1)?"":", ");
    printf("\nYour Input:    ");
    for(int i=0; i<p_quiz_dance->step_count; i++) printf("%s%s", user_steps[i], (i==p_quiz_dance->step_count-1)?"":", ");
    printf("\n\nYour Score: %d / 100\n", score);
    
    printf("\nPress Enter to return to menu...");
    getchar();
}

/**
 * @brief Frees all dynamically allocated memory at program exit.
 */
void cleanup_dance_data() {
    if(g_dance_db != NULL) {
        for(int i=0; i < g_dance_count; i++) {
            for(int j=0; j < g_dance_db[i].step_count; j++) {
                free(g_dance_db[i].steps[j]);
            }
            free(g_dance_db[i].steps);
        }
        free(g_dance_db);
    }
}
