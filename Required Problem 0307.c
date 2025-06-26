/**
 * @file main.c
 * @brief A self-contained version of the "Emotion Diary" feature for online compilers.
 *
 * This program allows users to log their feelings through a guided questionnaire
 * with specific input validation. It includes a bonus feature to update skipped
 * answers. All string operations are performed with custom-built functions.
 */

#include <stdio.h>
#include <stdlib.h> // For exit(), atoi(), qsort()
#include <time.h>   // For time() and strftime()
#include <ctype.h>  // For toupper() and isspace()

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define NUM_QUESTIONS 7
#define ANSWER_MIN_LEN 20
#define ANSWER_MAX_LEN 40

// Structure to hold one complete daily reflection entry
typedef struct {
    char member_nickname[50];
    char date[11]; // "YYYY-MM-DD"
    char answers[NUM_QUESTIONS][256];
    int entry_exists; // Flag to check if data has been entered
} EmotionDiaryEntry;

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

const char* g_emotion_questions[NUM_QUESTIONS] = {
    "What happened recently? How did you feel about it?",
    "How has your mood been for the past few days?",
    "What is your current mood? Are you feeling joy, sadness, anger, anxiety, etc.?",
    "In what situations do you feel your emotions more strongly?",
    "Have there been any recent emotional changes or stressors?",
    "Are there any physical symptoms that express your emotions? (e.g., tears, laughter, a rapidly beating heart, etc.)?",
    "What thoughts or strategies do you have for dealing with your emotions?"
};

EmotionDiaryEntry g_diary_log[NUM_MEMBERS];


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_self_mgmt_menu();
void logEmotionDiary();
int inputNickName();
void writeEmotionDiary(int member_idx);
void printEmotionDiary(const char* nickname);
void updateTBDs(int entry_idx);


// --- Custom String Helper Functions (No <string.h>) ---
int my_strlen(const char* s) { int i=0; while(s[i]!='\0') i++; return i; }
void my_strcpy(char* dest, const char* src) { int i=0; while(src[i]!='\0'){ dest[i]=src[i]; i++; } dest[i]='\0'; }
int my_strcmp(const char* s1, const char* s2) { while(*s1 && (*s1==*s2)){s1++;s2++;} return *(unsigned char*)s1 - *(unsigned char*)s2; }
int my_atoi(const char* s) {
    int result = 0;
    int i = 0;
    while(s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    return result;
}


// --- Main Entry Point ---
int main() {
    for(int i=0; i<NUM_MEMBERS; i++) g_diary_log[i].entry_exists = 0;
    
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
        printf("   F. Emotional Diary\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'F') {
            logEmotionDiary();
        } else if (choice != '0') {
             printf("\n[This feature is not shown in this combined file.]\n");
             printf("Press Enter to continue...");
             getchar();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---
int strlen_no_spaces(const char* str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) if (!isspace((unsigned char)str[i])) len++;
    return len;
}

// Comparison function for qsort to sort QAPair structs by answer length
int compare_qa_pairs(const void* a, const void* b) {
    QAPair* pairA = (QAPair*)a;
    QAPair* pairB = (QAPair*)b;
    // Sort in descending order
    return pairB->answer_len - pairA->answer_len;
}

int inputNickName() {
    char guess[50];
    for (int attempt = 1; attempt <= 2; attempt++) {
        printf("\n--- Member Login (Attempt %d/2) ---\n", attempt);
        printf("Hints: ");
        for (int i = 0; i < NUM_MEMBERS; i++) {
            const char* nick = g_milliways_members[i][1];
            printf("%c...%c  ", nick[0], nick[my_strlen(nick)-1]);
        }
        printf("\nEnter your full nickname: ");
        fgets(guess, sizeof(guess), stdin);
        guess[my_strlen(guess)-1] = '\0';
        
        for (int i = 0; i < NUM_MEMBERS; i++) {
            if (my_strcmp(guess, g_milliways_members[i][1]) == 0) {
                printf("Welcome, %s!\n", g_milliways_members[i][0]);
                return i;
            }
        }
        printf("Nickname not found. Please try again.\n");
    }
    return -1;
}

void updateTBDs(int entry_idx) {
    EmotionDiaryEntry* p_entry = &g_diary_log[entry_idx];
    int tbd_indices[NUM_QUESTIONS], tbd_count = 0;
    
    printf("\n--- Update Skipped Answers ---\n");
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (my_strcmp(p_entry->answers[i], "TBD") == 0) {
            printf("  %d. %s\n", tbd_count + 1, g_emotion_questions[i]);
            tbd_indices[tbd_count] = i;
            tbd_count++;
        }
    }
    if (tbd_count == 0) { printf("No answers were skipped in this entry.\n"); return; }

    char buffer[10];
    printf("Which entry would you like to update? (Enter number, or 0 to cancel): ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = my_atoi(buffer);

    if (choice > 0 && choice <= tbd_count) {
        int q_idx = tbd_indices[choice - 1];
        char new_answer[256];
        do {
            printf("\nEnter new answer for:\n\"%s\"\n> ", g_emotion_questions[q_idx]);
            fgets(new_answer, sizeof(new_answer), stdin);
            new_answer[my_strlen(new_answer)-1] = '\0';
            int len = my_strlen(new_answer);
            // Using a ternary operator as required
            printf("Length check: %s\n", (len >= ANSWER_MIN_LEN && len <= ANSWER_MAX_LEN) ? "OK" : "INVALID");
            if (len >= ANSWER_MIN_LEN && len <= ANSWER_MAX_LEN) {
                my_strcpy(p_entry->answers[q_idx], new_answer);
                printf("--- Answer updated successfully! ---\n");
                break;
            } else {
                 printf("Error: Answer must be %d-%d characters long.\n", ANSWER_MIN_LEN, ANSWER_MAX_LEN);
            }
        } while(1);
    }
}

void writeEmotionDiary(int member_idx) {
    EmotionDiaryEntry* p_entry = &g_diary_log[member_idx];
    int tbd_count = 0;
    char buffer[512];
    
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(p_entry->date, sizeof(p_entry->date), "%Y-%m-%d", t);

    printf("\n--- Writing New Emotion Diary ---\n");
    printf("You can skip up to 3 questions by typing 'TBD'.\n");

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        do {
            printf("\n%s\n> ", g_emotion_questions[i]);
            fgets(buffer, sizeof(buffer), stdin);
            buffer[my_strlen(buffer)-1] = '\0';
            if (my_strcmp(buffer, "TBD") == 0) {
                if (tbd_count < 3) { tbd_count++; my_strcpy(p_entry->answers[i], "TBD"); break; } 
                else { printf("You have already skipped the maximum of 3 questions.\n"); }
            } else {
                int len = my_strlen(buffer);
                if (len >= ANSWER_MIN_LEN && len <= ANSWER_MAX_LEN) { my_strcpy(p_entry->answers[i], buffer); break; } 
                else { printf("Error: Answer must be %d-%d characters long.\n", ANSWER_MIN_LEN, ANSWER_MAX_LEN); }
            }
        } while (1);
    }
    p_entry->entry_exists = 1;
    printf("\n--- Diary entry saved! ---\n");

    printf("\nWould you like to view the entry you just created? (y/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if(buffer[0] == 'y' || buffer[0] == 'Y') {
        printEmotionDiary(p_entry->member_nickname);
    }
}

void printEmotionDiary(const char* nickname) {
    int entry_idx = -1;
    for (int i = 0; i < NUM_MEMBERS; i++) {
        // Find the correct diary entry based on nickname.
        // We also need to check g_diary_log[i].entry_exists in a real scenario
        // but since we link them by index, this is sufficient.
        if (my_strcmp(g_milliways_members[i][1], nickname) == 0) {
            entry_idx = i;
            break;
        }
    }
    
    if (entry_idx == -1 || !g_diary_log[entry_idx].entry_exists) {
        printf("\nNo diary entry found for %s.\n", nickname);
        return;
    }

    const EmotionDiaryEntry* p_entry = &g_diary_log[entry_idx];
    
    // --- FIX STARTS HERE ---
    // Create a temporary array of structs to hold Q&A pairs for sorting
    QAPair pairs[NUM_QUESTIONS];

    // Correctly populate the pairs array from the single `answers` array
    for(int i = 0; i < NUM_QUESTIONS; i++) {
        pairs[i].question = g_emotion_questions[i];
        pairs[i].answer = p_entry->answers[i];
        pairs[i].answer_len = strlen_no_spaces(p_entry->answers[i]);
    }
    // --- FIX ENDS HERE ---
    
    // Sort the pairs array based on answer length (descending)
    qsort(pairs, NUM_QUESTIONS, sizeof(QAPair), compare_qa_pairs);

    printf("\n--- Emotion Diary for %s ---\n", p_entry->member_nickname);
    printf("Date: %s\n", p_entry->date);
    printf("--------------------------------\n");
    
    // Print the sorted Q&A pairs, skipping "TBD" entries
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (my_strcmp(pairs[i].answer, "TBD") != 0) {
            printf("\nQ: %s\n", pairs[i].question);
            printf("A: %s\n", pairs[i].answer);
        }
    }
    
    char choice_buf[10];
    printf("\n--------------------------------\n");
    printf("\nWould you like to update any skipped (TBD) answers? (y/n): ");
    fgets(choice_buf, sizeof(choice_buf), stdin);
    if(choice_buf[0] == 'y' || choice_buf[0] == 'Y') {
        updateTBDs(entry_idx);
    }
}

void logEmotionDiary() {
    clear_screen();
    printf("========================================\n");
    printf("         F. Emotional Diary\n");
    printf("========================================\n");

    int member_idx = inputNickName();

    if (member_idx == -1) {
        printf("\nLogin failed. Too many incorrect attempts. Terminating program.\n");
        exit(1);
    }

    const char* member_nickname = g_milliways_members[member_idx][1];
    my_strcpy(g_diary_log[member_idx].member_nickname, member_nickname);
    
    char choice_buf[10];
    while(1) {
        printf("\n--- Welcome, %s ---\n", member_nickname);
        printf("  1. Write new diary entry\n");
        printf("  2. View my diary entry\n");
        printf("  0. Back to previous menu\n");
        printf("Choice: ");
        fgets(choice_buf, sizeof(choice_buf), stdin);

        if (choice_buf[0] == '0') break;
        
        if (choice_buf[0] == '1') {
            writeEmotionDiary(member_idx);
        } else if (choice_buf[0] == '2') {
            printEmotionDiary(member_nickname);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
        clear_screen();
    }
}
