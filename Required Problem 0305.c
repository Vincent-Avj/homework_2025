/**
 * @file main.c
 * @brief A self-contained version of the "Self-Reflection Journal" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. It allows
 * users to select a Milliways member and fill out a daily reflection journal.
 * The program validates input length and displays the final entry sorted by
 * answer length, as required. The bonus feature of using a 2D array for
 * the reflection questions is fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define NUM_QUESTIONS 3
#define ANSWER_MIN_LEN 20
#define ANSWER_MAX_LEN 50

// Structure to hold one complete daily reflection entry
typedef struct {
    char member_nickname[50];
    char date[11]; // "YYYY-MM-DD"
    char achievement[256];
    char difficulty_overcome[256];
    char tomorrow_plan[256];
    int entry_exists;
} DailyReflection;

// A helper struct for sorting the question-answer pairs
typedef struct {
    const char* question;
    const char* answer;
    int answer_len;
} QAPair;


// --- Global Data Storage ---
const char* g_milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};

// (Bonus) The 3 reflection questions stored in a 2D array
const char* g_reflection_questions[NUM_QUESTIONS] = {
    "What did I achieve today?",
    "What difficulty did I overcome?",
    "What plan will I make for tomorrow?"
};

// Global array to store the latest reflection for each member
DailyReflection g_reflections[NUM_MEMBERS];


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_self_mgmt_menu();
void doSelfReflection();
void inputReflection(DailyReflection* p_entry);
void displayReflection(const DailyReflection* p_entry);


// --- Main Entry Point ---
int main() {
    // Initialize the reflections so we know no entries exist yet
    for(int i=0; i<NUM_MEMBERS; i++) g_reflections[i].entry_exists = 0;
    
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
    } while (choice != '0' && toupper(choice) != 'Q');
}

void show_training_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Training\n");
        printf("----------------------------------------\n");
        printf("   2. Self-Management & Teamwork\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '2') {
            show_self_mgmt_menu();
        }
    } while (choice != '0');
}

void show_self_mgmt_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   C. Teamwork Mission\n");
        printf("   D. Self-Reflection\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'C') {
             printf("\n[Teamwork Mission feature not shown in this combined file.]\n");
             printf("Press Enter to continue...");
             getchar();
        } else if (toupper(choice) == 'D') {
            doSelfReflection();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

/**
 * @brief Gets the current system date and formats it as "YYYY-MM-DD".
 */
void get_current_date(char* buffer) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, 11, "%Y-%m-%d", t);
}

/**
 * @brief Counts the number of non-space characters in a string.
 */
int strlen_no_spaces(const char* str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            len++;
        }
    }
    return len;
}

/**
 * @brief Comparison function for qsort to sort QAPair structs by answer length.
 */
int compare_qa_pairs(const void* a, const void* b) {
    QAPair* pairA = (QAPair*)a;
    QAPair* pairB = (QAPair*)b;
    // Sort in descending order
    return pairB->answer_len - pairA->answer_len;
}

/**
 * @brief Receives user input for the three reflection questions.
 */
void inputReflection(DailyReflection* p_entry) {
    char buffer[512];
    
    // Automatically set the date
    get_current_date(p_entry->date);

    printf("\nAnswer the following three questions. Each answer must be %d-%d characters (excluding spaces).\n", ANSWER_MIN_LEN, ANSWER_MAX_LEN);

    char* destinations[] = {p_entry->achievement, p_entry->difficulty_overcome, p_entry->tomorrow_plan};

    // Loop through the global questions array (Bonus)
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        do {
            printf("\n%s\n> ", g_reflection_questions[i]);
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            int len = strlen_no_spaces(buffer);
            
            // Using a ternary operator as required by the constraints
            printf("Length check (excl. spaces): %d characters. Status: %s\n", len,
                   (len >= ANSWER_MIN_LEN && len <= ANSWER_MAX_LEN) ? "OK" : "INVALID");
                   
            if (len >= ANSWER_MIN_LEN && len <= ANSWER_MAX_LEN) {
                strcpy(destinations[i], buffer);
                break; // Valid answer, move to next question
            } else {
                printf("Answer length is not within the required range. Please try again.\n");
            }
        } while (1);
    }
    p_entry->entry_exists = 1; // Mark that this entry is now valid
}

/**
 * @brief Displays a completed reflection, sorted by answer length.
 */
void displayReflection(const DailyReflection* p_entry) {
    if (!p_entry->entry_exists) {
        printf("No reflection has been recorded for this member yet.\n");
        return;
    }
    
    // Create a temporary array of structs to hold Q&A pairs for sorting
    QAPair pairs[NUM_QUESTIONS];
    const char* answers[] = {p_entry->achievement, p_entry->difficulty_overcome, p_entry->tomorrow_plan};

    for(int i = 0; i < NUM_QUESTIONS; i++) {
        pairs[i].question = g_reflection_questions[i];
        pairs[i].answer = answers[i];
        pairs[i].answer_len = strlen_no_spaces(answers[i]);
    }

    // Sort the pairs array based on answer length (descending)
    qsort(pairs, NUM_QUESTIONS, sizeof(QAPair), compare_qa_pairs);
    
    printf("\n========================================\n");
    printf("        Reflection for %s\n", p_entry->member_nickname);
    printf("Date: %s\n", p_entry->date);
    printf("----------------------------------------\n");

    // Print the sorted Q&A pairs
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        printf("\nQ: %s\n", pairs[i].question);
        printf("A: %s\n", pairs[i].answer);
    }
    printf("========================================\n");
}


/**
 * @brief The main function for the Self-Reflection feature.
 */
void doSelfReflection() {
    clear_screen();
    printf("========================================\n");
    printf("         D. Self-Reflection Journal\n");
    printf("========================================\n");
    
    printf("Which member is completing their reflection?\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("  %d. %s (%s)\n", i + 1, g_milliways_members[i][0], g_milliways_members[i][1]);
    }
    
    char buffer[10];
    int choice = 0;
    do {
        printf("Select a member (1-%d): ", NUM_MEMBERS);
        fgets(buffer, sizeof(buffer), stdin);
        choice = atoi(buffer);
    } while (choice < 1 || choice > NUM_MEMBERS);
    
    int member_idx = choice - 1;
    DailyReflection* p_current_reflection = &g_reflections[member_idx];
    strcpy(p_current_reflection->member_nickname, g_milliways_members[member_idx][1]);
    
    // Get Input and then Display it
    inputReflection(p_current_reflection);
    displayReflection(p_current_reflection);

    printf("\nPress Enter to return to the menu...");
    getchar();
}
