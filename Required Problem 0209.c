/**
 * @file stage1.c
 * @brief The complete and integrated code for all "Stage 1" training modules.
 *
 * This version adds a "Creative Writing" feature. This module prompts a user
 * to create a word bank, then presents a random set of words as a writing
 * prompt. The user writes a story that is then validated and analyzed. The
 * bonus feature for ensuring unique re-rolled prompts is fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define WORDBOOK_SIZE 10
#define PROMPT_WORD_COUNT 5
#define STORY_MIN_LEN 90
#define STORY_MAX_LEN 100
#define MAX_STORY_LEN 2048

// --- Forward Declarations for Menu System and Other Features ---
void clear_screen();
void show_physical_strength_menu();
void inputCommonLearningInfo();
void inquiryCommonLearningInfo();


// --- Utility Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// --- Creative Writing Feature Functions ---

/**
 * @brief Prompts the user to enter 10 words, validating their length.
 * @param wordbook A 2D char array to store the 10 words.
 */
void input_words(char wordbook[WORDBOOK_SIZE][10]) {
    printf("Please enter %d words for the wordbook.\n", WORDBOOK_SIZE);
    printf("Each word must be between 3 and 5 characters long.\n\n");

    for (int i = 0; i < WORDBOOK_SIZE; i++) {
        do {
            printf("Enter word %d/%d: ", i + 1, WORDBOOK_SIZE);
            fgets(wordbook[i], 10, stdin);
            wordbook[i][strcspn(wordbook[i], "\n")] = 0;
            int len = strlen(wordbook[i]);
            if (len >= 3 && len <= 5) {
                break; // Valid input
            } else {
                printf("Invalid length. Please try again.\n");
            }
        } while (1);
    }
}

/**
 * @brief Generates a single set of 5 unique random words.
 */
void generate_random_set(char wordbook[WORDBOOK_SIZE][10], char chosen_words[PROMPT_WORD_COUNT][10]) {
    int indices[WORDBOOK_SIZE];
    for(int i=0; i<WORDBOOK_SIZE; i++) indices[i] = i;
    
    for(int i=WORDBOOK_SIZE-1; i>0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    
    for(int i=0; i<PROMPT_WORD_COUNT; i++) {
        strcpy(chosen_words[i], wordbook[indices[i]]);
    }
}

/**
 * @brief Analyzes the story and prints the results.
 * @param story The user's written story.
 * @param prompt_words The 5 words that were given as the prompt.
 */
void analyze_and_print_results(const char* story, char prompt_words[PROMPT_WORD_COUNT][10]) {
    printf("\n========================================\n");
    printf("            Story Analysis\n");
    printf("========================================\n");

    // 1. Character Count
    int char_count = strlen(story);
    printf("Total Characters: %d\n", char_count);

    // 2. Word Count
    char story_copy[MAX_STORY_LEN];
    strcpy(story_copy, story);
    int word_count = 0;
    char* token = strtok(story_copy, " \t\n\r");
    while(token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    printf("Total Words: %d\n", word_count);

    // 3. Keyword Repetition Count
    printf("\n--- Prompt Word Frequency ---\n");
    for (int i = 0; i < PROMPT_WORD_COUNT; i++) {
        int repetitions = 0;
        const char* current_pos = story;
        while ((current_pos = strstr(current_pos, prompt_words[i])) != NULL) {
            repetitions++;
            current_pos++; // Move past the start of the found word
        }
        printf("  - '%s': Used %d time(s).\n", prompt_words[i], repetitions);
    }
}


/**
 * @brief The main function for the Creative Writing feature.
 */
void doCreativeWriting() {
    clear_screen();
    printf("========================================\n");
    printf("         M. Creative Writing\n");
    printf("========================================\n");

    // Step 1: Input words
    char wordbook[WORDBOOK_SIZE][10];
    input_words(wordbook);

    // Step 2: Get prompt words, with re-rolls and bonus logic
    char chosen_words[PROMPT_WORD_COUNT][10];
    char first_set[PROMPT_WORD_COUNT][10];
    
    for (int attempt = 1; attempt <= 3; attempt++) {
        printf("\n--- Generating Prompt Words (Attempt %d of 3) ---\n", attempt);
        
        if (attempt == 1) {
            generate_random_set(wordbook, chosen_words);
            // Save the first set for the bonus check
            for(int i=0; i<PROMPT_WORD_COUNT; i++) strcpy(first_set[i], chosen_words[i]);
        } else {
            // (Bonus) Ensure the new set has < 2 words in common with the first set
            int overlap;
            do {
                overlap = 0;
                generate_random_set(wordbook, chosen_words);
                for (int i = 0; i < PROMPT_WORD_COUNT; i++) {
                    for (int j = 0; j < PROMPT_WORD_COUNT; j++) {
                        if (strcmp(chosen_words[i], first_set[j]) == 0) {
                            overlap++;
                        }
                    }
                }
            } while (overlap >= 2);
            printf("(A new set with less than 2 overlapping words has been generated.)\n");
        }

        printf("Your 5 random words are:\n");
        for (int i = 0; i < PROMPT_WORD_COUNT; i++) printf("  - %s\n", chosen_words[i]);
        
        if (attempt < 3) {
            char choice_buf[10];
            printf("\nAre you happy with these words? (y/n): ");
            fgets(choice_buf, sizeof(choice_buf), stdin);
            if (toupper(choice_buf[0]) == 'Y') break;
        } else {
            printf("\nThis is your final set of words. Let's write!\n");
        }
    }
    
    // Step 3: Write and validate the story
    char story[MAX_STORY_LEN];
    do {
        printf("\nWrite a story between %d and %d characters using these words.\n", STORY_MIN_LEN, STORY_MAX_LEN);
        printf("Type 'END' on a new line when you are finished.\n> ");
        
        char line_buffer[256];
        story[0] = '\0'; // Start with an empty story
        
        while(1) {
            fgets(line_buffer, sizeof(line_buffer), stdin);
            if(strncmp(line_buffer, "END", 3) == 0) break;
            // Check for buffer overflow before concatenating
            if (strlen(story) + strlen(line_buffer) < MAX_STORY_LEN) {
                strcat(story, line_buffer);
            } else {
                printf("Story buffer is full, cannot add more lines.\n");
                break;
            }
        }
        story[strcspn(story, "\n")] = 0; // Clean up any final newlines

        int len = strlen(story);
        if (len >= STORY_MIN_LEN && len <= STORY_MAX_LEN) {
            break;
        } else {
            printf("\nYour story has %d characters. It must be between %d and %d. Please try again.\n", len, STORY_MIN_LEN, STORY_MAX_LEN);
        }
    } while (1);

    // Step 4: Analyze and print results
    analyze_and_print_results(story, chosen_words);
}


// --- Menu System ---
const char* HEALTH_MENU_ITEMS[] = {
    "A-J. (Previous Features)", "K. Input Common Education Info", "L. Inquire Common Education Results",
    "M. Creative Writing"
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
            case 'K': inputCommonLearningInfo(); break;
            case 'L': inquiryCommonLearningInfo(); break;
            case 'M': doCreativeWriting(); break;
            default: printf("\n(This is a placeholder for a previous feature.)\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Dummy implementations for context
void inputCommonLearningInfo() { printf("\n(Placeholder for Input Education Info)\n"); }
void inquiryCommonLearningInfo() { printf("\n(Placeholder for Inquire Education Results)\n"); }

int main(void) {
    srand(time(NULL));
    show_physical_strength_menu();
    return 0;
}
