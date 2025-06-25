/**
 * @file main.c
 * @brief A self-contained version of the "Korean Jamo Learning" quiz for online compilers.
 *
 * This program combines all necessary functions from the multi-file project
 * into a single file. It allows a user to select a Milliways member and
 * take a quiz on Korean consonants and vowels, with results tracked for
 * multiple attempts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// For cross-platform sleep functionality
#ifdef _WIN32
#include <windows.h>
#define delay_seconds(s) Sleep((s) * 1000)
#else
#include <unistd.h>
#define delay_seconds(s) sleep(s)
#endif


// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define NUM_JAMO 24 // 14 consonants + 10 vowels
#define QUIZ_QUESTION_COUNT 5
#define MAX_ATTEMPTS 3
#define MAX_RESULTS (NUM_MEMBERS * 2 * 2 * MAX_ATTEMPTS) // 4 members, 2 game types, 2 char types, 3 attempts each

// Structure for a single Hangul Jamo flashcard
typedef struct {
    char jamo[10];
    char english_alpha[10];
    char english_pron[20];
    char type; // 'C' for Consonant, 'V' for Vowel
} HangulJamo;

// Structure to store the result of a single quiz attempt
typedef struct {
    char nickname[50];
    char challenge_type[10]; // e.g., "KE-C" (Kor-Eng Consonants)
    int score;
    int attempt_num;
} GameResult;


// --- Global Data Storage ---
const HangulJamo g_jamo_bank[NUM_JAMO] = {
    // Consonants
    {"ㄱ", "g", "giyeok", 'C'}, {"ㄴ", "n", "nieun", 'C'},
    {"ㄷ", "d", "digeut", 'C'}, {"ㄹ", "r/l", "rieul", 'C'},
    {"ㅁ", "m", "mieum", 'C'}, {"ㅂ", "b", "bieup", 'C'},
    {"ㅅ", "s", "siot", 'C'}, {"ㅇ", "ng", "ieung", 'C'},
    {"ㅈ", "j", "jieut", 'C'}, {"ㅊ", "ch", "chieut", 'C'},
    {"ㅋ", "k", "kieuk", 'C'}, {"ㅌ", "t", "tieut", 'C'},
    {"ㅍ", "p", "pieup", 'C'}, {"ㅎ", "h", "hieut", 'C'},
    // Vowels
    {"ㅏ", "a", "a", 'V'}, {"ㅑ", "ya", "ya", 'V'},
    {"ㅓ", "eo", "eo", 'V'}, {"ㅕ", "yeo", "yeo", 'V'},
    {"ㅗ", "o", "o", 'V'}, {"ㅛ", "yo", "yo", 'V'},
    {"ㅜ", "u", "u", 'V'}, {"ㅠ", "yu", "yu", 'V'},
    {"ㅡ", "eu", "eu", 'V'}, {"ㅣ", "i", "i", 'V'}
};

const char* g_milliways_nicknames[NUM_MEMBERS] = {"Ariel", "Simba", "Belle", "Aladdin"};

GameResult g_results[MAX_RESULTS];
int g_results_count = 0;


// --- Forward Declarations ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_lang_menu();
void learnHangul();


// --- Main Entry Point ---
int main() {
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
    } while (choice != '0' && choice != 'Q' && choice != 'q');
}

void show_training_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Training\n");
        printf("----------------------------------------\n");
        printf("   3. Language & Pronunciation\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '3') {
            show_lang_menu();
        }
    } while (choice != '0');
}

void show_lang_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Language & Pronunciation\n");
        printf("----------------------------------------\n");
        printf("   B. Hangul Jamo Learning\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == 'B' || choice == 'b') {
            learnHangul();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

/**
 * @brief The main game loop function, generalized for any quiz type.
 * @param p_trainee_nickname Pointer to the trainee's nickname.
 * @param game_mode 1 for Kor->Eng, 2 for Eng->Kor.
 */
void playGame(const char* p_trainee_nickname, int game_mode) {
    char type_choice;
    char challenge_type_str[10];

    // Bonus: Ask user to choose Consonants or Vowels
    printf("\nWhich set would you like to practice?\n");
    printf("  C. Consonants\n");
    printf("  V. Vowels\n");
    printf("Choice: ");
    scanf(" %c", &type_choice);
    while(getchar() != '\n');
    type_choice = toupper(type_choice);

    if (type_choice != 'C' && type_choice != 'V') {
        printf("Invalid choice. Returning to menu.\n");
        return;
    }
    
    // Determine challenge type string for results, e.g., "KE-C"
    sprintf(challenge_type_str, "%s-%c", (game_mode == 1 ? "KE" : "EK"), type_choice);

    // Check attempt count
    int attempts = 0;
    for (int i = 0; i < g_results_count; i++) {
        if (strcmp(g_results[i].nickname, p_trainee_nickname) == 0 &&
            strcmp(g_results[i].challenge_type, challenge_type_str) == 0) {
            attempts++;
        }
    }
    if (attempts >= MAX_ATTEMPTS) {
        printf("You have already completed the maximum of %d attempts for this quiz.\n", MAX_ATTEMPTS);
        return;
    }

    printf("\nGet ready! The quiz will begin shortly...\n");
    
    // Filter and select questions
    int question_indices[NUM_JAMO];
    int available_q_count = 0;
    for(int i=0; i<NUM_JAMO; i++) {
        if(g_jamo_bank[i].type == type_choice) {
            question_indices[available_q_count++] = i;
        }
    }
    // Shuffle the available questions
    for(int i=available_q_count-1; i>0; i--) { int j = rand()%(i+1); int tmp=question_indices[i]; question_indices[i]=question_indices[j]; question_indices[j]=tmp; }

    int score = 0;
    char user_answer[100];
    
    // Quiz Loop
    for (int i = 0; i < QUIZ_QUESTION_COUNT; i++) {
        int q_idx = question_indices[i];
        const HangulJamo* p_q = &g_jamo_bank[q_idx];
        printf("\n--- Question %d of %d ---\n", i + 1, QUIZ_QUESTION_COUNT);

        if (game_mode == 1) { // Kor -> Eng
            printf("What is the English letter and pronunciation for '%s'?\n", p_q->jamo);
            printf("(Format: alpha,pronunciation): ");
            fgets(user_answer, sizeof(user_answer), stdin);
            user_answer[strcspn(user_answer, "\n")] = 0;

            char* user_alpha = strtok(user_answer, ",");
            char* user_pron = strtok(NULL, "");
            
            if(user_alpha && user_pron && strcmp(user_alpha, p_q->english_alpha) == 0 && strcmp(user_pron, p_q->english_pron) == 0) {
                printf("Correct!\n");
                score += 10;
            } else {
                printf("Incorrect. Correct answer: %s,%s\n", p_q->english_alpha, p_q->english_pron);
            }
        } else { // Eng -> Kor
            printf("What is the Hangul Jamo for '%s' (%s)?\n", p_q->english_alpha, p_q->english_pron);
            printf("Your answer: ");
            fgets(user_answer, sizeof(user_answer), stdin);
            user_answer[strcspn(user_answer, "\n")] = 0;

            if (strcmp(user_answer, p_q->jamo) == 0) {
                printf("Correct!\n");
                score += 10;
            } else {
                printf("Incorrect. Correct answer: %s\n", p_q->jamo);
            }
        }
    }

    // Save result
    if (g_results_count < MAX_RESULTS) {
        strcpy(g_results[g_results_count].nickname, p_trainee_nickname);
        strcpy(g_results[g_results_count].challenge_type, challenge_type_str);
        g_results[g_results_count].score = score;
        g_results[g_results_count].attempt_num = attempts + 1;
        g_results_count++;
    }

    printf("\n--- Quiz Finished! ---\n");
    printf("Your score for this attempt: %d / 50\n", score);
}

/**
 * @brief Displays the aggregated results for the selected trainee.
 */
void showResult(const char* p_trainee_nickname) {
    printf("\n--- Results for %s ---\n", p_trainee_nickname);
    
    const char* types[] = {"KE-C", "KE-V", "EK-C", "EK-V"};
    const char* type_names[] = {
        "Hangul -> Eng (Consonants)", "Hangul -> Eng (Vowels)",
        "Eng -> Hangul (Consonants)", "Eng -> Hangul (Vowels)"
    };
    
    // Loop through the 4 possible challenge types to display them in sorted order
    for(int i=0; i<4; i++){
        int total_score = 0;
        int num_attempts = 0;
        for(int j=0; j<g_results_count; j++){
            if(strcmp(g_results[j].nickname, p_trainee_nickname) == 0 &&
               strcmp(g_results[j].challenge_type, types[i]) == 0) {
                total_score += g_results[j].score;
                num_attempts++;
            }
        }
        
        printf("\nCategory: %s\n", type_names[i]);
        if(num_attempts > 0) {
            double avg_score = (double)total_score / num_attempts;
            printf("  - Attempts: %d/%d\n", num_attempts, MAX_ATTEMPTS);
            printf("  - Average Score: %.1f / 50\n", avg_score);
        } else {
            printf("  - No attempts made yet.\n");
        }
    }
}


/**
 * @brief The main function for the Hangul Learning feature.
 */
void learnHangul() {
    clear_screen();
    printf("========================================\n");
    printf("        Hangul Jamo Learning\n");
    printf("========================================\n");
    
    printf("Select a trainee to start:\n");
    for(int i=0; i<NUM_MEMBERS; i++) printf("  %d. %s\n", i+1, g_milliways_nicknames[i]);
    
    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if(choice < 1 || choice > NUM_MEMBERS) {
        printf("Invalid selection. Returning.\n");
        return;
    }
    const char* p_selected_trainee = g_milliways_nicknames[choice-1];

    while(1) {
        printf("\n--- Welcome, %s! ---\n", p_selected_trainee);
        printf("  1. Hangul to English Quiz\n");
        printf("  2. English to Hangul Quiz\n");
        printf("  3. View My Results\n");
        printf("  0. Back to Main Menu\n");
        printf("Choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        if(buffer[0] == '0') break;

        switch(buffer[0]) {
            case '1': playGame(p_selected_trainee, 1); break;
            case '2': playGame(p_selected_trainee, 2); break;
            case '3': showResult(p_selected_trainee); break;
            default: printf("Invalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
        clear_screen();
    }
}
