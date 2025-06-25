/**
 * @file main.c
 * @brief A self-contained version of the "Choreography Patterns" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. It features
 * a quiz on dance pattern knowledge and a bonus game to complete a choreography
 * chain. It includes a fallback mechanism to use hardcoded data if input files
 * are not found, ensuring it can run in any environment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Included for tolower() used in my_stricmp()

// For cross-platform sleep functionality
#ifdef _WIN32
#include <windows.h>
#define sleep_seconds(s) Sleep((s) * 1000)
#else
#include <unistd.h>
#define sleep_seconds(s) sleep(s)
#endif


// --- Constants and Data Structures ---
#define MAX_PATTERNS 10
#define NUM_SONGS 4

// Structure for a single dance pattern from the database
typedef struct {
    char name[100];
    char description[512];
} DancePattern;

// Node for the song's choreography sequence (Singly Linked List)
typedef struct PatternNode {
    char pattern_name[100]; // FIX: Corrected from 'name' to 'pattern_name'
    struct PatternNode* next;
} PatternNode;


// --- Global Data Storage ---
DancePattern* g_pattern_db = NULL;
int g_pattern_count = 0;

// An array of Linked List heads, one for each song
PatternNode* g_song_pattern_heads[NUM_SONGS] = {NULL};
char g_song_titles[NUM_SONGS][100];


// --- Forward Declarations for All Functions ---
void clear_screen();
void cleanup_stage5_data();
void show_main_menu();
void show_training_menu();
void show_dance_menu();
void learnDancePattern();


// --- Main Entry Point ---
int main() {
    // Register the cleanup function to be called on normal program termination
    atexit(cleanup_stage5_data);
    
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
        printf("   B. Choreography Patterns\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'A') {
             printf("\n[Learn Basic Dance Steps functionality not shown in this combined file.]\n");
             printf("Press Enter to continue...");
             getchar();
        } else if (toupper(choice) == 'B') {
            learnDancePattern();
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

// FIX: Custom case-insensitive string compare to replace non-standard stricmp()
int my_stricmp(const char* s1, const char* s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

// --- Linked List Operations ---
void add_pattern_to_list(PatternNode** p_head, const char* pattern_name) {
    PatternNode* newNode = (PatternNode*)malloc(sizeof(PatternNode));
    // FIX: Changed from newNode->name to newNode->pattern_name
    strcpy(newNode->pattern_name, pattern_name);
    newNode->next = NULL;

    if (*p_head == NULL) {
        *p_head = newNode;
    } else {
        PatternNode* current = *p_head;
        while(current->next != NULL) current = current->next;
        current->next = newNode;
    }
}

void display_pattern_list(const PatternNode* head) {
    if(!head) return;
    const PatternNode* current = head;
    while(current != NULL) {
        // FIX: Changed from current->name to current->pattern_name
        printf("%s", current->pattern_name);
        if(current->next != NULL) printf(" -> ");
        current = current->next;
    }
    printf("\n");
}

// --- File I/O and Data Initialization ---
void load_dance_patterns() {
    if (g_pattern_db != NULL) return;
    FILE* file = fopen("dance_pattern.txt", "r");
    if (!file) {
        printf("Notice: 'dance_pattern.txt' not found. Using hardcoded fallback data.\n");
        g_pattern_db = (DancePattern*)malloc(6 * sizeof(DancePattern));
        g_pattern_count = 6;
        strcpy(g_pattern_db[0].name, "Popping"); strcpy(g_pattern_db[0].description, "A style characterized by sudden tensing and releasing of muscles to the rhythm.");
        strcpy(g_pattern_db[1].name, "Locking"); strcpy(g_pattern_db[1].description, "A style involving freezing from a fast movement and 'locking' in a certain position.");
        strcpy(g_pattern_db[2].name, "Waacking"); strcpy(g_pattern_db[2].description, "A form of dance characterized by rotational arm movements to the beat of the music.");
        strcpy(g_pattern_db[3].name, "Krumping"); strcpy(g_pattern_db[3].description, "A style of street dance characterized by free, expressive, and highly energetic moves.");
        strcpy(g_pattern_db[4].name, "Tutting"); strcpy(g_pattern_db[4].description, "A dance style that mimics the angular poses seen in ancient Egyptian art.");
        strcpy(g_pattern_db[5].name, "Voguing"); strcpy(g_pattern_db[5].description, "A modern house dance that evolved out of the Harlem ballroom scene in the 1980s.");
        return;
    }
    
    g_pattern_db = (DancePattern*)malloc(MAX_PATTERNS * sizeof(DancePattern));
    char line[1024];
    while (g_pattern_count < MAX_PATTERNS && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        char* name = strtok(line, ";");
        char* desc = strtok(NULL, "");
        if (name && desc) {
            strcpy(g_pattern_db[g_pattern_count].name, name);
            strcpy(g_pattern_db[g_pattern_count].description, desc);
            g_pattern_count++;
        }
    }
    fclose(file);
}

void load_analyzed_patterns() {
    if (g_song_pattern_heads[0] != NULL) return;
    FILE* file = fopen("analyz_dance-pattern.csv", "r");
    if(!file) {
        printf("Notice: 'analyz_dance-pattern.csv' not found. Using hardcoded fallback data.\n");
        strcpy(g_song_titles[0], "DDU-DU DDU-DU");
        add_pattern_to_list(&g_song_pattern_heads[0], "Popping");
        add_pattern_to_list(&g_song_pattern_heads[0], "Voguing");
        add_pattern_to_list(&g_song_pattern_heads[0], "Waacking");

        strcpy(g_song_titles[1], "Blood Sweat & Tears");
        add_pattern_to_list(&g_song_pattern_heads[1], "Tutting");
        add_pattern_to_list(&g_song_pattern_heads[1], "Voguing");
        add_pattern_to_list(&g_song_pattern_heads[1], "Locking");
        strcpy(g_song_titles[2], "Sherlock");
        add_pattern_to_list(&g_song_pattern_heads[2], "Popping");
        add_pattern_to_list(&g_song_pattern_heads[2], "Locking");
        add_pattern_to_list(&g_song_pattern_heads[2], "Tutting");
        strcpy(g_song_titles[3], "BOOMBAYAH");
        add_pattern_to_list(&g_song_pattern_heads[3], "Waacking");
        add_pattern_to_list(&g_song_pattern_heads[3], "Popping");
        add_pattern_to_list(&g_song_pattern_heads[3], "Krumping");
        return;
    }

    char line[1024];
    int song_idx = 0;
    while(song_idx < NUM_SONGS && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        char* token = strtok(line, ",");
        if(token) {
            strcpy(g_song_titles[song_idx], token);
            token = strtok(NULL, ",");
        }
        while(token != NULL) {
            add_pattern_to_list(&g_song_pattern_heads[song_idx], token);
            token = strtok(NULL, ",");
        }
        song_idx++;
    }
    fclose(file);
}

int runPatternQuiz() {
    printf("\n--- Choreography Pattern Quiz ---\n");
    printf("You will be given a description and a hint. Name the pattern.\n");
    
    int score = 0;
    for (int i=0; i<4; i++) {
        int q_idx = rand() % g_pattern_count;
        const DancePattern* p_q = &g_pattern_db[q_idx];
        
        char hint[11];
        int desc_len = strlen(p_q->description);
        int start_pos = (desc_len > 10) ? (rand() % (desc_len - 10)) : 0;
        strncpy(hint, p_q->description + start_pos, 10);
        hint[10] = '\0';

        printf("\nQuestion %d:\n", i+1);
        printf("Description: %s\n", p_q->description);
        printf("Hint (substring): ...%s...\n", hint);
        
        char user_guess[100];
        printf("Pattern Name: ");
        fgets(user_guess, sizeof(user_guess), stdin);
        user_guess[strcspn(user_guess, "\n")] = 0;
        
        // FIX: Using my_stricmp instead of non-standard stricmp
        if (my_stricmp(user_guess, p_q->name) == 0) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Incorrect. The correct answer was: %s\n", p_q->name);
        }
    }
    return score;
}

void playPatternChainGame() {
    clear_screen();
    printf("\n--- Bonus Game: Complete the Choreography Chain! ---\n");
    printf("Select a song to practice:\n");
    for(int i=0; i<NUM_SONGS; i++) {
        if(g_song_pattern_heads[i] != NULL)
            printf("  %d. %s\n", i+1, g_song_titles[i]);
    }
    
    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if(choice < 1 || choice > NUM_SONGS || g_song_pattern_heads[choice-1] == NULL) {
        printf("Invalid choice.\n");
        return;
    }
    
    PatternNode* current_node = g_song_pattern_heads[choice - 1];
    
    while(current_node != NULL) {
        printf("\nCurrent Pattern: [%s]\n", current_node->pattern_name);
        if(current_node->next == NULL) {
            printf("\n*** Congratulations! You completed the entire chain for %s! ***\n", g_song_titles[choice - 1]);
            break;
        }
        
        char user_guess[100];
        printf("What is the NEXT pattern in the sequence? > ");
        fgets(user_guess, sizeof(user_guess), stdin);
        user_guess[strcspn(user_guess, "\n")] = 0;

        if(my_stricmp(user_guess, current_node->next->pattern_name) == 0) {
            printf("Correct! Moving to the next step.\n");
            current_node = current_node->next;
        } else {
            printf("Incorrect. The next pattern was '%s'. Game over.\n", current_node->next->pattern_name);
            break;
        }
    }
}

/**
 * @brief The main function for the Dance Pattern Learning feature.
 */
void learnDancePattern() {
    clear_screen();
    printf("========================================\n");
    printf("      B. Choreography Patterns\n");
    printf("========================================\n");
    
    load_dance_patterns();
    if(g_pattern_count == 0) { printf("Failed to load pattern data.\n"); return; }
    
    int quiz_score = runPatternQuiz();
    printf("\nYour score: %d/4\n", quiz_score);

    if (quiz_score >= 3) {
        printf("\nGreat job! You qualify to analyze K-POP choreography.\n");
        load_analyzed_patterns();
        
        printf("\n--- Analyzed Choreography Sequences ---\n");
        for(int i=0; i<NUM_SONGS; i++) {
            if(g_song_pattern_heads[i] == NULL) continue;
            printf("%-20s: ", g_song_titles[i]);
            display_pattern_list(g_song_pattern_heads[i]);
        }

        playPatternChainGame();

    } else {
        printf("\nPlease study the patterns more and try again later.\n");
    }

    printf("\nPress Enter to return to menu...");
    getchar();
}

/**
 * @brief Frees all dynamically allocated memory at program exit.
 */
void cleanup_stage5_data() {
    if (g_pattern_db != NULL) {
        free(g_pattern_db);
        g_pattern_db = NULL;
    }
    for (int i=0; i<NUM_SONGS; i++) {
        PatternNode *current = g_song_pattern_heads[i], *next;
        while(current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
    }
}
