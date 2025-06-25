/**
 * @file stage2.c
 * @brief A module for Self-Management and Teamwork training.
 *
 * This version adds Arthur's "Easter Egg," a hidden feature for managing trauma.
 * It demonstrates advanced string manipulation, bitwise operations for character-to-binary
 * conversion, and a small puzzle for the user to solve. The bonus scrambling
 * logic is fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---

#define NUM_MEMBERS 4

// Structure to hold the Easter Egg's data
typedef struct {
    const char* keyword;
    const char* message;
} EasterEgg;

// --- Global Data and Forward Declarations ---

const EasterEgg g_arthurs_egg = {
    "specter",
    "I confess. After graduating from university, I was blinded by the arrogance of starting a startup and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."
};

// Forward declaration for the menu function
void show_self_mgmt_menu(); 

// --- Utility Functions ---

/**
 * @brief Clears the console screen in a cross-platform way.
 * THIS FUNCTION WAS ADDED TO FIX THE LINKER ERROR.
 */
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// --- Easter Egg Core Logic ---

/**
 * @brief Converts a single character to its 8-bit binary string representation.
 * This function uses bitwise operators as required by the constraints.
 * @param c The character to convert.
 * @param binary_str_buffer A buffer (at least 9 chars long) to store the result.
 */
void charToBinaryString(char c, char* binary_str_buffer) {
    binary_str_buffer[8] = '\0'; // Null-terminate the string
    for (int i = 7; i >= 0; i--) {
        // Use bitwise right shift and AND to check each bit
        int bit = (c >> i) & 1;
        binary_str_buffer[7 - i] = bit + '0'; // Convert int 0/1 to char '0'/'1'
    }
}

/**
 * @brief (Bonus) Scrambles a keyword using the specified method.
 * 1. Reverses the word.
 * 2. Collects odd-indexed characters.
 * 3. Collects even-indexed characters.
 * @param input The original keyword (e.g., "specter").
 * @param output A buffer to store the scrambled result (e.g., "ecprtes").
 */
void scramble_keyword(const char* input, char* output) {
    int len = strlen(input);
    char reversed[len + 1];

    // 1. Reverse the word
    for(int i = 0; i < len; i++) {
        reversed[i] = input[len - 1 - i];
    }
    reversed[len] = '\0';

    int out_idx = 0;
    // 2. Collect odd-indexed letters (e.g., from "retceps": e, c, p)
    for(int i = 1; i < len; i += 2) {
        output[out_idx++] = reversed[i];
    }
    // 3. Collect even-indexed letters (e.g., from "retceps": r, t, e, s)
    for(int i = 0; i < len; i += 2) {
        output[out_idx++] = reversed[i];
    }
    output[out_idx] = '\0';
}

/**
 * @brief Verifies if the user correctly decoded the binary strings.
 * @param correct_chars The string of characters the user should have typed.
 * @return 1 if correct, 0 otherwise.
 */
int isRightChar(const char* correct_chars) {
    char user_input[50];
    printf("\nEnter the characters corresponding to the binary values above:\n> ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = 0;

    if (strcmp(user_input, correct_chars) == 0) {
        printf("Correctly decoded!\n");
        return 1;
    } else {
        printf("That's not right. Please look carefully and try again.\n");
        return 0;
    }
}

/**
 * @brief Compares the user's final guess with the Easter egg keyword.
 * @return 1 if it matches, 0 otherwise.
 */
int isEasterEgg() {
    char user_input[50];
    printf("\nNow, unscramble those letters to form the original keyword:\n> ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = 0;
    
    return strcmp(user_input, g_arthurs_egg.keyword) == 0;
}


/**
 * @brief The main function that controls the Easter Egg puzzle flow.
 */
void find_easter_egg() {
    clear_screen();
    printf("========================================\n");
    printf("        <<Arthur's Easter Egg>>\n");
    printf("========================================\n");
    
    // Bonus: Create the scrambled word to be displayed
    char scrambled_word[20];
    scramble_keyword(g_arthurs_egg.keyword, scrambled_word);

    printf("To unlock the message, solve the puzzle.\n");
    printf("Decode the binary, then find the original keyword.\n\n");
    
    // Convert each character of the scrambled word to binary and display it
    char binary_buffer[9];
    for(int i=0; i < strlen(scrambled_word); i++) {
        charToBinaryString(scrambled_word[i], binary_buffer);
        printf("  %s\n", binary_buffer);
    }

    // Loop until the user decodes the characters correctly
    while (!isRightChar(scrambled_word)) {
        // The loop continues as long as isRightChar returns false (0)
    }

    // Check the final unscrambled word
    if (isEasterEgg()) {
        printf("\n##Easter Egg Discovered!$$\n");
        printf("%s\n", g_arthurs_egg.message);
    } else {
        printf("\nThat's not the right keyword. Returning to the menu.\n");
    }

    printf("\nPress Enter to continue...");
    getchar();
}


// --- Menu System Integration ---

void show_self_mgmt_menu() {
    char choice_buf[50];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 2. Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   A. Mentoring\n");
        printf("   B. Trauma Management\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice (or type 'Arthur' to find a secret): ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        choice_buf[strcspn(choice_buf, "\n")] = 0;

        // Check for the Easter Egg trigger word
        if (strcmp(choice_buf, "Arthur") == 0) {
            find_easter_egg();
            continue; // Go back to the menu display after the easter egg
        }

        char choice = toupper(choice_buf[0]);
        if (choice == '0') break;

        switch(choice) {
            case 'A':
                printf("\n'Mentoring' feature from a previous problem.\n");
                break;
            case 'B':
                printf("\n'Trauma Management' feature from a previous problem.\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Main Entry Point ---

int main(void) {
    // Seed the random number generator once at the start of the program
    srand(time(NULL));

    // For this standalone example, we'll just call the main feature's menu.
    show_self_mgmt_menu();

    return 0;
}
