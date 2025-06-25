/**
 * @file main.c
 * @brief A self-contained version of the "Korean Grammar Learning" quiz for online compilers.
 *
 * This program combines all necessary functions from the multi-file project
 * into a single file. It allows a user to log in, take a sentence composition
 * quiz, and view their ranked score. It uses Quick Sort as required by the bonus.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define NUM_SENTENCES 10
#define GRAMMAR_QUIZ_COUNT 5

// For the Korean Grammar Game
typedef struct {
    const char* english;
    const char* korean;
} EngKorSentence;

typedef struct {
    char nickname[50];
    int score;
    int has_played; // Flag to check if a score exists
} GrammarScore;

// --- Global Data Storage ---
const EngKorSentence g_sentence_bank[NUM_SENTENCES] = {
    {"I go to school every day.", "저는 매일 학교에 갑니다"},
    {"This apple is very delicious.", "이 사과는 정말 아주 맛있어요"},
    {"My younger sibling is reading a book.", "제 동생은 지금 책을 읽고 있어요"},
    {"Yesterday, I met a friend in Seoul.", "어제 저는 서울에서 친구를 만났어요"},
    {"What movie do you want to see tomorrow?", "내일 어떤 영화를 보고 싶으세요"},
    {"I plan to travel to Jeju Island this summer.", "저는 이번 여름에 제주도로 여행을 갈 계획입니다"},
    {"Learning Korean is fun but difficult.", "한국어 배우는 것은 재미있지만 조금 어려워요"},
    {"Please recommend a delicious Korean restaurant.", "맛있는 한국 식당을 하나 추천해 주세요"},
    {"The weather is nice, so I feel good.", "오늘 날씨가 좋아서 기분이 정말 좋아요"},
    {"I want to become a great singer.", "저는 훌륭한 가수가 되고 싶습니다"}
};

const char* g_milliways_nicknames[NUM_MEMBERS] = {"Ariel", "Simba", "Belle", "Aladdin"};

GrammarScore g_grammar_scores[NUM_MEMBERS];
int g_grammar_data_initialized = 0;


// --- Forward Declarations for all functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_lang_menu();
void learnKoreanGrammar();
void composeSentences(GrammarScore* p_user_score);
void printScore(const GrammarScore* p_user_score);
void quickSort(char** arr, int low, int high);


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
    } while (choice != '0' && toupper(choice) != 'Q');
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
        printf("   C. Learning Korean Grammar\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == 'C' || choice == 'c') {
            learnKoreanGrammar();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

// (Bonus) Quick Sort Implementation
void swap_pointers(char** a, char** b) {
    char* temp = *a;
    *a = *b;
    *b = temp;
}

int partition(char** arr, int low, int high) {
    char* pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j], pivot) < 0) {
            i++;
            swap_pointers(&arr[i], &arr[j]);
        }
    }
    swap_pointers(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(char** arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void initialize_grammar_data() {
    if (g_grammar_data_initialized) return;
    for (int i = 0; i < NUM_MEMBERS; i++) {
        strcpy(g_grammar_scores[i].nickname, g_milliways_nicknames[i]);
        g_grammar_scores[i].score = 0;
        g_grammar_scores[i].has_played = 0;
    }
    g_grammar_data_initialized = 1;
}

void display_sort_comparison() {
    printf("\n--- Comparison of Sorting Algorithms ---\n");
    printf("\n[Selection Sort]\n");
    printf(" - Principle: Repeatedly find the minimum element from the unsorted part and put it at the beginning.\n");
    printf(" - Time Complexity: O(n^2) in all cases (worst, average, best).\n");
    printf(" - Feature: Simple to understand, but inefficient for large datasets.\n");

    printf("\n[Quick Sort]\n");
    printf(" - Principle: A 'divide and conquer' algorithm. It picks a 'pivot' element and partitions the array around the pivot.\n");
    printf(" - Time Complexity: O(n log n) on average, but O(n^2) in the worst case.\n");
    printf(" - Feature: Extremely fast in practice, often preferred for large datasets.\n");
}

void composeSentences(GrammarScore* p_user_score) {
    printf("\n--- Quiz: Compose Korean Sentences ---\n");
    
    int q_indices[NUM_SENTENCES];
    for(int i=0; i<NUM_SENTENCES; i++) q_indices[i] = i;
    for(int i=NUM_SENTENCES-1; i>0; i--) { int j = rand()%(i+1); int tmp=q_indices[i]; q_indices[i]=q_indices[j]; q_indices[j]=tmp; }

    int current_score = 0;
    for(int i=0; i<GRAMMAR_QUIZ_COUNT; i++) {
        int q_idx = q_indices[i];
        const EngKorSentence* p_q = &g_sentence_bank[q_idx];
        
        printf("\n--- Question %d of %d ---\n", i + 1, GRAMMAR_QUIZ_COUNT);
        printf("English: %s\n", p_q->english);

        char sentence_copy[256];
        strcpy(sentence_copy, p_q->korean);
        char* word_tokens[20];
        int word_count = 0;
        char* token = strtok(sentence_copy, " ");
        while(token != NULL) {
            word_tokens[word_count++] = token;
            token = strtok(NULL, " ");
        }

        quickSort(word_tokens, 0, word_count - 1);
        
        printf("Korean words (sorted): ");
        for(int j=0; j<word_count; j++) printf("%s ", word_tokens[j]);
        printf("\n");

        char user_answer[256];
        printf("Your sentence: ");
        fgets(user_answer, sizeof(user_answer), stdin);
        user_answer[strcspn(user_answer, "\n")] = 0;

        if(strcmp(user_answer, p_q->korean) == 0) {
            printf("Correct!\n");
            current_score += 20;
        } else {
            printf("Incorrect. The correct sentence was: %s\n", p_q->korean);
        }
    }
    
    p_user_score->score = current_score;
    p_user_score->has_played = 1;
    printf("\n--- Quiz Finished! Your score is %d / 100 ---\n", current_score);
}

void printScore(const GrammarScore* p_user_score) {
    if(!p_user_score->has_played) {
        printf("\nYou must complete the 'Compose Sentences' quiz first!\n");
        return;
    }
    
    printf("\n--- Your Score ---\n");
    printf("  %s: %d points\n", p_user_score->nickname, p_user_score->score);

    GrammarScore rankings[NUM_MEMBERS];
    memcpy(rankings, g_grammar_scores, sizeof(g_grammar_scores));
    
    for(int i=0; i<NUM_MEMBERS-1; i++){
        for(int j=0; j<NUM_MEMBERS-i-1; j++){
            if(rankings[j].score < rankings[j+1].score){
                GrammarScore temp = rankings[j];
                rankings[j] = rankings[j+1];
                rankings[j+1] = temp;
            }
        }
    }

    printf("\n--- All Member Rankings ---\n");
    for(int i=0; i<NUM_MEMBERS; i++){
        printf("  Rank %d: %-10s (%d points)\n", i+1, rankings[i].nickname, rankings[i].score);
    }
}

void learnKoreanGrammar() {
    initialize_grammar_data();
    clear_screen();
    printf("========================================\n");
    printf("       Learning Korean Grammar\n");
    printf("========================================\n");
    
    char nickname_buf[50];
    int login_attempts = 0;
    int member_idx = -1;

    while(login_attempts < 3) {
        printf("Enter your nickname to begin (Attempt %d/3): ", login_attempts + 1);
        fgets(nickname_buf, sizeof(nickname_buf), stdin);
        nickname_buf[strcspn(nickname_buf, "\n")] = 0;

        for(int i=0; i<NUM_MEMBERS; i++) {
            if(strcmp(nickname_buf, g_milliways_nicknames[i]) == 0) {
                member_idx = i;
                break;
            }
        }
        if(member_idx != -1) break;
        printf("Nickname not found.\n");
        login_attempts++;
    }

    if(member_idx == -1) {
        printf("Too many failed login attempts. Returning to menu.\n");
        return;
    }
    
    GrammarScore* p_current_user_score = &g_grammar_scores[member_idx];
    
    char buffer[10];
    while(1) {
        printf("\n--- Welcome, %s! ---\n", p_current_user_score->nickname);
        printf("  1. Compose Korean Sentences\n");
        printf("  2. Check Scores\n");
        printf("  3. Compare Sorting Algorithms (Bonus Info)\n");
        printf("  0. Back\n");
        printf("Choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        if (buffer[0] == '0') break;

        switch(buffer[0]) {
            case '1': composeSentences(p_current_user_score); break;
            case '2': printScore(p_current_user_score); break;
            case '3': display_sort_comparison(); break;
            default: printf("Invalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}
