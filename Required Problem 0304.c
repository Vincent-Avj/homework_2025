/**
 * @file main.c
 * @brief A self-contained version of the "Teamwork Mission" game for online compilers.
 *
 * This program combines all necessary functions into a single file. It simulates
 * a game where team members must guess K-POP song titles from clues. The bonus
 * feature of randomly providing either initials or lyrics as the clue is
 * fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define NUM_SONGS 7
#define INITIAL_ATTEMPTS 2

// (Bonus) Enum to determine the type of clue for a mission
typedef enum {
    MISSION_TYPE_INITIALS,
    MISSION_TYPE_LYRICS
} MissionType;

// Structure to hold all data for a single K-POP song quiz
typedef struct {
    char title[100];
    char initials[50];      // e.g., "ㅂㅌㅅㄴㄷ"
    char lyrics_hint[200];  // e.g., "Oh my my my"
} KpopSong;


// --- Global Data Storage ---
const char* milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};

// Global database for the song quiz
KpopSong g_song_db[NUM_SONGS];
int g_song_count = 0;


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_self_mgmt_menu();
void completeMission();
void inputKpopSongInitials();
void inputSongTitle(const KpopSong* song, MissionType type, char* guess_buffer);
int checkKpopSongTitle(const char* guess, const char* correct_title);
int getBlackKnight(int current_player_idx);


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
        printf("   1. Physical & Knowledge\n");
        printf("   2. Self-Management & Teamwork\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        
        if (choice == '1') {
             printf("\n[Physical & Knowledge menu not implemented in this version.]\n");
             printf("Press Enter to continue...");
             getchar();
        } else if (choice == '2') {
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
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'C') {
            completeMission();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

/**
 * @brief Populates the song database with hardcoded data.
 */
void inputKpopSongInitials() {
    if (g_song_count > 0) return; // Already initialized

    g_song_db[0] = (KpopSong){"작은 것들을 위한 시", "ㅈㅇ ㄱㄷㅇ ㅇㅎ ㅅ", "Oh my my my, oh my my my"};
    g_song_db[1] = (KpopSong){"라일락", "ㄹㅇㄹ", "오 나의 계절의 끝은 너야"};
    g_song_db[2] = (KpopSong){"신호등", "ㅅㅎㄷ", "내 머릿속의 내비게이션"};
    g_song_db[3] = (KpopSong){"사건의 지평선", "ㅅㄱㅇ ㅈㅍㅅ", "그날 내가 했던 모든 말은"};
    g_song_db[4] = (KpopSong){"사랑은 늘 도망가", "ㅅㄹㅇ ㄴ ㄷㅁㄱ", "눈물이 난다 이 길을 걸으면"};
    g_song_db[5] = (KpopSong){"어떻게 이별까지 사랑하겠어", "ㅇㄸㅎ ㅇㅂㄲㅈ ㅅㄹㅎㄱㅇ", "널 사랑하는 거지"};
    g_song_db[6] = (KpopSong){"해야", "ㅎㅇ", "해가 지기 전에 가야만 해"};
    g_song_count = 7;
}

/**
 * @brief Presents the mission prompt and gets the user's guess.
 */
void inputSongTitle(const KpopSong* song, MissionType type, char* guess_buffer) {
    printf("\n--- Mission ---\n");
    if (type == MISSION_TYPE_INITIALS) {
        printf("Clue (Initials): %s\n", song->initials);
    } else { // Bonus Mission
        printf("Clue (Lyrics): \"%s\"\n", song->lyrics_hint);
    }
    printf("Enter the song title: ");
    fgets(guess_buffer, 100, stdin);
    guess_buffer[strcspn(guess_buffer, "\n")] = 0; // Remove newline
}

/**
 * @brief Checks if the user's guess is correct.
 */
int checkKpopSongTitle(const char* guess, const char* correct_title) {
    return strcmp(guess, correct_title) == 0;
}

/**
 * @brief Selects a random "black knight" who is not the current player.
 */
int getBlackKnight(int current_player_idx) {
    int knight_idx;
    do {
        knight_idx = rand() % NUM_MEMBERS;
    } while (knight_idx == current_player_idx);
    return knight_idx;
}

/**
 * @brief The main function for the teamwork mission game.
 */
void completeMission() {
    clear_screen();
    printf("========================================\n");
    printf("         Teamwork Mission Start!\n");
    printf("========================================\n");

    inputKpopSongInitials(); // Load the song data

    int used_songs[NUM_SONGS] = {0}; // To ensure each mission has a unique song
    int team_mission_failed = 0;

    for (int i = 0; i < NUM_MEMBERS; i++) {
        const char* current_player_name = milliways_members[i][0];
        const char* current_player_nickname = milliways_members[i][1];
        int mission_success = 0;
        
        // Select a random, unused song
        int song_idx;
        do {
            song_idx = rand() % g_song_count;
        } while (used_songs[song_idx]);
        used_songs[song_idx] = 1;
        const KpopSong* current_song = &g_song_db[song_idx];

        // Randomly choose between a regular or bonus mission
        MissionType mission_type = (rand() % 2 == 0) ? MISSION_TYPE_INITIALS : MISSION_TYPE_LYRICS;

        printf("\n\n--- Mission for %s (%s) ---\n", current_player_name, current_player_nickname);

        // Player gets 2 attempts
        for (int attempt = 1; attempt <= INITIAL_ATTEMPTS; attempt++) {
            char guess[100];
            printf("Attempt %d of %d...\n", attempt, INITIAL_ATTEMPTS);
            inputSongTitle(current_song, mission_type, guess);
            if (checkKpopSongTitle(guess, current_song->title)) {
                printf("\nCorrect! %s-nim, you have successfully completed the mission!\n", current_player_name);
                mission_success = 1;
                break;
            } else {
                printf("Incorrect. Try again.\n");
            }
        }

        // If still not successful, call the Black Knight
        if (!mission_success) {
            printf("\nTwo attempts failed! Calling a Black Knight for help!\n");
            int knight_idx = getBlackKnight(i);
            const char* knight_name = milliways_members[knight_idx][0];
            printf("Black Knight %s, it's your turn to save the day!\n", knight_name);

            char guess[100];
            inputSongTitle(current_song, mission_type, guess);
            if (checkKpopSongTitle(guess, current_song->title)) {
                printf("\nAmazing! The Black Knight succeeded! Mission clear for this round.\n");
                mission_success = 1;
            }
        }
        
        // Check if this round ended in failure
        if (!mission_success) {
            printf("\nOh no... The Black Knight also failed.\n");
            team_mission_failed = 1;
            break; // Exit the main loop
        }
    }

    // Final Team Result
    printf("\n========================================\n");
    if (team_mission_failed) {
        printf("Milliways, please improve your teamwork and challenge again!\n");
    } else {
        printf("Milliways' teamwork is excellent!\n");
    }
    printf("========================================\n");
    
    printf("\nPress Enter to return to the menu...");
    getchar();
}
