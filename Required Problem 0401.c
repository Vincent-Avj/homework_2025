#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // For toupper()

// For cross-platform sleep
#ifdef _WIN32
#include <windows.h>
#define delay(seconds) Sleep((seconds) * 1000)
#else
#include <unistd.h>
#define delay(seconds) sleep(seconds)
#endif

// --- Constants and Data Structures ---
#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINEES 4
#define NUM_QUESTIONS 10
#define QUIZ_QUESTION_COUNT 5
#define PASS_SCORE 80
#define MAX_LEN 256

typedef struct {
    int id;
    const char* question;
    const char* answer;
} QuestionBank;

typedef struct {
    char name[MAX_LEN];
    char nickname[MAX_LEN];
    char nationality[MAX_LEN];
    int score;
    char pass_status; // 'N' = Not taken, 'P' = Pass, 'F' = Fail
} Trainee;

typedef struct {
    int question_id;
    char correctness; // 'O' for correct, 'X' for incorrect
} QuizAnswer;

// --- Global Read-Only Data ---
const QuestionBank KOREAN_Q_BANK[NUM_QUESTIONS] = {
    {1, "How do you say 'Hello' in Korean?", "안녕하세요"},
    {2, "How do you say 'Thank you' in Korean?", "감사합니다"},
    {3, "What is 'Water' in Korean?", "물"},
    {4, "What is 'I love you' in Korean?", "사랑해요"},
    {5, "How do you say 'Yes' in Korean?", "네"},
    {6, "How do you say 'No' in Korean?", "아니요"},
    {7, "What is 'Goodbye' (when staying) in Korean?", "안녕히 가세요"},
    {8, "What is 'Goodbye' (when leaving) in Korean?", "안녕히 계세요"},
    {9, "How do you say 'My name is...' in Korean?", "제 이름은... 입니다"},
    {10, "What is 'Please give me...' in Korean?", "주세요"}
};

// --- Function Prototypes ---
void clear_screen();
void display_main_menu();
void handle_training_menu(Trainee* trainees);
void testKoreanLang(Trainee* trainees);
Trainee* selectRandomTaker(Trainee* trainees, int current_hour);
void serveRandomQuiz(Trainee* taker, const QuestionBank* bank);
int evaluateAllAnswers(const char user_answers[][MAX_LEN], const int q_indices[], const QuestionBank* bank, QuizAnswer* results);

// --- Main Program ---
int main() {
    // Initialize trainee data at the start
    Trainee milliways_members[NUM_TRAINEES];
    Trainee* p_members = milliways_members; // Use a pointer as required

    // Populate the initial trainee data
    strcpy(p_members[0].name, "Jiyeon Park"); strcpy(p_members[0].nickname, "Ariel"); strcpy(p_members[0].nationality, "Korean");
    strcpy(p_members[1].name, "Ethan Smith"); strcpy(p_members[1].nickname, "Simba"); strcpy(p_members[1].nationality, "USA");
    strcpy(p_members[2].name, "Helena Silva"); strcpy(p_members[2].nickname, "Belle"); strcpy(p_members[2].nationality, "Brazil");
    strcpy(p_members[3].name, "Liam Wilson"); strcpy(p_members[3].nickname, "Aladdin"); strcpy(p_members[3].nationality, "Australia");

    for (int i = 0; i < NUM_TRAINEES; i++) {
        p_members[i].score = 0;
        p_members[i].pass_status = 'N'; // 'N' for Not Started
    }

    // Seed the random number generator once
    srand(time(NULL));

    char input_buffer[100];
    // Main menu loop
    while (1) {
        display_main_menu();
        printf("> Select a menu (or 0 to quit): ");
        fgets(input_buffer, sizeof(input_buffer), stdin);
        
        // Check for quit conditions
        if (input_buffer[0] == '0' || toupper(input_buffer[0]) == 'Q' || input_buffer[0] == '\n') {
            break;
        }

        int choice = atoi(input_buffer);
        if (choice == 2) {
            handle_training_menu(p_members);
        } else if (choice == 1 || choice == 3) {
            printf("\n(This feature is not yet implemented.)\n");
            printf("Press Enter to continue...");
            getchar();
        } else {
            printf("\nInvalid selection.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    }
    printf("Terminating Magrathea System.\n");
    return 0;
}

// --- Function Definitions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void display_main_menu() {
    clear_screen();
    printf("========================================\n");
    printf("        MAGRATHEA Main Menu\n");
    printf("========================================\n");
    printf("   I. Audition Management\n");
    printf("   II. Training\n");
    printf("   III. Debut\n");
    printf("----------------------------------------\n");
}

void handle_training_menu(Trainee* trainees) {
    clear_screen();
    printf("\n--- [II. Training] ---\n");
    printf("3. Language and Pronunciation Training\n");
    printf("--------------------------------------------\n");
    printf("A. Korean Quiz\n");
    printf("0. Return to Main Menu\n");
    printf("> Select an option: ");

    char choice_char = getchar();
    while (getchar() != '\n'); // Clear the rest of the input buffer

    if (choice_char == 'A' || choice_char == 'a') {
        testKoreanLang(trainees);
    }
    printf("\nPress Enter to return to the main menu...");
    getchar();
}

void testKoreanLang(Trainee* trainees) {
    clear_screen();
    time_t now = time(NULL);
    struct tm* p_tm = localtime(&now);
    int current_hour = p_tm->tm_hour;

    printf("\n--- Korean Language Comprehension Quiz ---\n");

    Trainee* p_taker = selectRandomTaker(trainees, current_hour);
    
    if (p_taker != NULL) {
        printf("Welcome, %s (%s), to the Korean quiz session!\n", p_taker->name, p_taker->nickname);
        serveRandomQuiz(p_taker, KOREAN_Q_BANK);
    } else {
        printf("No eligible (non-Korean) trainees available for the quiz at this time.\n");
    }
}

Trainee* selectRandomTaker(Trainee* trainees, int current_hour) {
    Trainee* potential_takers[NUM_TRAINEES];
    int count = 0;
    
    // Create a list of pointers to eligible trainees
    for (int i = 0; i < NUM_TRAINEES; i++) {
        if (strcmp(trainees[i].nationality, "Korean") != 0) {
            potential_takers[count++] = &trainees[i];
        }
    }

    if (count == 0) return NULL; // No one to test
    
    // Select a taker based on the current hour
    int taker_index = current_hour % count;
    return potential_takers[taker_index];
}

void serveRandomQuiz(Trainee* taker, const QuestionBank* bank) {
    int question_indices[NUM_QUESTIONS];
    char user_answers[QUIZ_QUESTION_COUNT][MAX_LEN];
    QuizAnswer session_results[QUIZ_QUESTION_COUNT];
    int total_score;

    // Create a shuffled list of question indices
    for (int i = 0; i < NUM_QUESTIONS; i++) question_indices[i] = i;
    for (int i = NUM_QUESTIONS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = question_indices[i];
        question_indices[i] = question_indices[j];
        question_indices[j] = temp;
    }

    printf("The quiz will begin in 5 seconds. Please prepare.\n");
    delay(5); // Using 5 seconds for a quicker demo

    printf("\n--- QUIZ START ---\n");
    for (int i = 0; i < QUIZ_QUESTION_COUNT; i++) {
        int q_index = question_indices[i];
        printf("\nQuestion %d of %d: %s\n", i + 1, QUIZ_QUESTION_COUNT, bank[q_index].question);
        printf("Your answer: ");
        fgets(user_answers[i], MAX_LEN, stdin);
        user_answers[i][strcspn(user_answers[i], "\n")] = 0; 
    }
    printf("\n--- QUIZ COMPLETE ---\n\n");
    printf("Grading your answers...\n");
    
    // (Bonus) Evaluate all answers after the quiz is finished
    total_score = evaluateAllAnswers(user_answers, question_indices, bank, session_results);

    // Update the trainee's record
    taker->score = total_score;
    taker->pass_status = (total_score >= PASS_SCORE) ? 'P' : 'F';

    // Display the final results
    printf("\n--- Results for %s ---\n", taker->name);
    printf("Answer Sheet:\n");
    for (int i = 0; i < QUIZ_QUESTION_COUNT; i++) {
        printf("  Question ID %2d: You were %s\n", session_results[i].question_id, 
               (session_results[i].correctness == 'O' ? "Correct (O)" : "Incorrect (X)"));
    }
    printf("---------------------------\n");
    printf("Total Score: %d / 100\n", taker->score);
    printf("Status: %s\n", (taker->pass_status == 'P') ? "Pass" : "Fail");
}

// (Bonus) This function is called only ONCE after all answers are collected.
int evaluateAllAnswers(const char user_answers[][MAX_LEN], const int q_indices[], const QuestionBank* bank, QuizAnswer* results) {
    int score = 0;
    for (int i = 0; i < QUIZ_QUESTION_COUNT; i++) {
        int current_q_index = q_indices[i];
        results[i].question_id = bank[current_q_index].id;

        // The single call to check the answer happens here
        if (strcmp(user_answers[i], bank[current_q_index].answer) == 0) {
            results[i].correctness = 'O';
            score += 20;
        } else {
            results[i].correctness = 'X';
        }
    }
    return score;
}
