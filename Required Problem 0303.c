#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 

#define NUM_MAIN_MENU_ITEMS 3
#define MAX_NAME_LEN 50
#define KEYWORD_LEN 7

struct EasterEgg {
    const char* keyword;
    const char* message;
};

const struct EasterEgg arthur_trauma = {
    "specter",
    "I confess. After graduating from university, I was blinded by the arrogance of starting a startup and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."
};

void display_main_menu();
void handle_training_menu();
void find_easter_egg();
void char_to_binary(char c);
void scramble_keyword(const char* keyword, char* scrambled_buffer);
int isRightChar(char input, char correct_char);
int isEasterEgg(const char* user_word, const char* keyword);
int str_ci_cmp(const char *s1, const char *s2); // Case-insensitive string compare

int main() {
    char input_buffer[100];
    int choice;

    srand(time(NULL));

    while (1) {
        display_main_menu();
        printf("> Select a menu (or 0, q to quit): ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;

        if (strcmp(input_buffer, "\n") == 0 || strcmp(input_buffer, "0\n") == 0 ||
            str_ci_cmp(input_buffer, "q\n") == 0) {
            printf("Terminating Magrathea System.\n");
            break;
        }

        if (str_ci_cmp(input_buffer, "Arthur\n") == 0) {
            find_easter_egg();
            continue; 
        }

        choice = atoi(input_buffer);
        switch (choice) {
            case 1: printf("\n(Audition Management is not yet implemented.)\n\n"); break;
            case 2: handle_training_menu(); break;
            case 3: printf("\n(Debut is not yet implemented.)\n\n"); break;
            default: printf("\nInvalid selection. Please try again.\n\n");
        }
    }
    return 0;
}



void display_main_menu() {
    printf("####################################\n");
    printf("#        Magrathea Main Menu       #\n");
    printf("####################################\n");
    printf("I. Audition Management\n");
    printf("II. Training\n");
    printf("III. Debut\n");
    printf("------------------------------------\n");
}


void handle_training_menu() {
    printf("\n--- [II. Training] ---\n");
    printf("This stage focuses on Self-Management, Teamwork, and personal growth.\n");
    printf("Returning to main menu. (Hint: Try typing a name.)\n\n");

}



void find_easter_egg() {
    char scrambled_keyword[KEYWORD_LEN + 1];
    char user_chars[KEYWORD_LEN + 1];
    char final_word_input[MAX_NAME_LEN];
    int i;

    printf("\n<<Arthur's Easter Egg>>\n");
    printf("Decode the binary sequences to reveal the keyword.\n");

 
    scramble_keyword(arthur_trauma.keyword, scrambled_keyword);

    for (i = 0; i < KEYWORD_LEN; i++) {
        char correct_char = scrambled_keyword[i];
        char user_input_char;

        printf("\nBinary: ");
        char_to_binary(correct_char); 

        do {
            printf("\nEnter the corresponding character: ");
            scanf(" %c", &user_input_char);
            while(getchar() != '\n'); 

            if (!isRightChar(user_input_char, correct_char)) {
                printf("Incorrect. Please try again.\n");
            }
        } while (!isRightChar(user_input_char, correct_char));

        user_chars[i] = user_input_char; 
    }
    user_chars[i] = '\0';

    printf("\nExcellent. The characters you found are: ");
    for(i=0; i < KEYWORD_LEN; i++) printf("%c ", user_chars[i]);

    printf("\nNow, combine these letters to form the original keyword: ");
    fgets(final_word_input, sizeof(final_word_input), stdin);
    final_word_input[strcspn(final_word_input, "\n")] = 0;

    if (isEasterEgg(final_word_input, arthur_trauma.keyword)) {
        printf("\n##Easter Egg Discovered!$$\n");
        printf("%s\n\n", arthur_trauma.message);
    } else {
        printf("\nThat is not the correct keyword. The ghost remains elusive...\n\n");
    }
}


void char_to_binary(char c) {
    for (int i = 7; i >= 0; i--) {
      
        int bit = (c >> i) & 1;
        printf("%d", bit);
    }
}


void scramble_keyword(const char* keyword, char* scrambled_buffer) {
    char reversed[KEYWORD_LEN + 1];
    int len = strlen(keyword);
    int i, k = 0;


    for (i = 0; i < len; i++) {
        reversed[i] = keyword[len - 1 - i];
    }
    reversed[len] = '\0';

    for (i = 0; i < len; i += 2) {
        scrambled_buffer[k++] = reversed[i];
    }

    for (i = 1; i < len; i += 2) {
        scrambled_buffer[k++] = reversed[i];
    }
    scrambled_buffer[len] = '\0';
}

int isRightChar(char input, char correct_char) {
    return (input == correct_char);
}


int isEasterEgg(const char* user_word, const char* keyword) {
    return (strcmp(user_word, keyword) == 0);
}


int str_ci_cmp(const char *s1, const char *s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}
