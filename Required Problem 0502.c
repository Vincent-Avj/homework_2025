/**
 * @file main.c
 * @brief A self-contained version of the "Find My Voice" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. It simulates
 * a vocal evaluation game where a user tries to guess their randomly generated
 * vocal profile values. The program demonstrates the use of function pointers
 * to switch between a main game and a bonus game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h> // For fabs()

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4

// Structure for the main "Find My Voice" game data
typedef struct {
    char nickname[50];
    double frequency;
    double amplitude;
    double jitter;
    int grbas[5]; // Bonus: G, R, B, A, S scores
    int game_played;
} MyVoice;


// --- Global Data Storage ---

// Constant struct holding the valid ranges for all voice characteristics
const struct {
    double freq_m[2], freq_f[2];
    double amp_m[2], amp_f[2];
    double jitter_m[2], jitter_f[2];
    int grbas_range[2];
} g_ranges = {
    {85.0, 180.0}, {165.0, 255.0}, // Frequency M, F
    {2.5, 3.6}, {3.5, 4.7},       // Amplitude M, F
    {0.2, 0.6}, {0.3, 1.0},       // Jitter M, F
    {0, 3}                        // GRBAS range for both
};

// Global array to store each member's voice data
MyVoice g_member_voice_data[NUM_MEMBERS];
int g_voice_data_initialized = 0;

const char* g_milliways_members[NUM_MEMBERS][3] = {
    {"Jiyeon Park", "Ariel", "F"},
    {"Ethan Smith", "Simba", "M"},
    {"Helena Silva", "Belle", "F"},
    {"Liam Wilson", "Aladdin", "M"}
};


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_vocal_menu();
void findMyVoice();
void runMainGame(MyVoice* p_target_voice);
void runGRBASGame(MyVoice* p_target_voice);


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
        printf("   4. Vocal Training\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '4') {
            show_vocal_menu();
        }
    } while (choice != '0');
}

void show_vocal_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Vocal Training\n");
        printf("----------------------------------------\n");
        printf("   A. Vocal Evaluation\n");
        printf("   B. Find My Voice\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'A') {
            printf("\n[Vocal Evaluation feature not shown in this file.]\n");
            printf("Press Enter to continue...");
            getchar();
        } else if (toupper(choice) == 'B') {
            findMyVoice();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

// Generates a random double within a given range
double rand_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

void initialize_voice_data() {
    if (g_voice_data_initialized) return;
    for(int i=0; i<NUM_MEMBERS; i++) {
        strcpy(g_member_voice_data[i].nickname, g_milliways_members[i][1]);
        g_member_voice_data[i].game_played = 0;
    }
    g_voice_data_initialized = 1;
}

/**
 * @brief A generic up-and-down guessing game for a double value.
 */
int play_double_guesser(double target, const char* prompt) {
    char buffer[50];
    printf("\n--- Guessing %s ---\n", prompt);
    for (int i = 0; i < 3; i++) {
        printf("Attempt %d of 3: Enter your guess > ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        double guess = atof(buffer);

        if (fabs(guess - target) < 0.01) { // Compare doubles with a tolerance
            printf("Correct!\n");
            return 1;
        } else if (guess < target) {
            printf("Up! Higher than that.\n");
        } else {
            printf("Down! Lower than that.\n");
        }
    }
    printf("Out of attempts. The correct value was %.2f.\n", target);
    return 0;
}

/**
 * @brief A generic up-and-down guessing game for an integer value.
 */
int play_int_guesser(int target, const char* prompt) {
    char buffer[50];
    printf("\n--- Guessing %s ---\n", prompt);
    for (int i = 0; i < 3; i++) {
        printf("Attempt %d of 3: Enter your guess > ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        int guess = atoi(buffer);

        if (guess == target) {
            printf("Correct!\n");
            return 1;
        } else if (guess < target) {
            printf("Up! Higher than that.\n");
        } else {
            printf("Down! Lower than that.\n");
        }
    }
    printf("Out of attempts. The correct value was %d.\n", target);
    return 0;
}

/**
 * @brief Creates a random target voice profile for the user to guess.
 */
void getMyVoiceFromHeaven(MyVoice* p_my_voice, char gender) {
    if (gender == 'M') {
        p_my_voice->frequency = rand_double(g_ranges.freq_m[0], g_ranges.freq_m[1]);
        p_my_voice->amplitude = rand_double(g_ranges.amp_m[0], g_ranges.amp_m[1]);
        p_my_voice->jitter = rand_double(g_ranges.jitter_m[0], g_ranges.jitter_m[1]);
    } else {
        p_my_voice->frequency = rand_double(g_ranges.freq_f[0], g_ranges.freq_f[1]);
        p_my_voice->amplitude = rand_double(g_ranges.amp_f[0], g_ranges.amp_f[1]);
        p_my_voice->jitter = rand_double(g_ranges.jitter_f[0], g_ranges.jitter_f[1]);
    }
    // For GRBAS bonus
    for(int i=0; i<5; i++){
        p_my_voice->grbas[i] = g_ranges.grbas_range[0] + (rand() % (g_ranges.grbas_range[1] - g_ranges.grbas_range[0] + 1));
    }
    p_my_voice->game_played = 1;
}

// --- Game Mode Functions ---
void runMainGame(MyVoice* p_target_voice) {
    int success_count = 0;
    if(play_double_guesser(p_target_voice->frequency, "Frequency (Hz)")) success_count++;
    if(play_double_guesser(p_target_voice->amplitude, "Amplitude (dB)")) success_count++;
    if(play_double_guesser(p_target_voice->jitter, "Jitter (%)")) success_count++;
    
    if (success_count == 3) {
        printf("\nCongratulations! You successfully found your voice profile!\n");
    } else {
        printf("\nYou didn't find all your voice values. Please try again later.\n");
    }
}

// Bonus Feature
void runGRBASGame(MyVoice* p_target_voice) {
    int success_count = 0;
    const char* grbas_names[] = {"G (Grade)", "R (Roughness)", "B (Breathiness)", "A (Asthenia)", "S (Strain)"};
    for(int i=0; i<5; i++){
        if(play_int_guesser(p_target_voice->grbas[i], grbas_names[i])) {
            success_count++;
        }
    }
    
    if (success_count == 5) {
        printf("\nCongratulations! You successfully found your GRBAS voice tone profile!\n");
    } else {
        printf("\nYou didn't find all your GRBAS values. Please try again later.\n");
    }
}

/**
 * @brief The main function for the "Find My Voice" feature.
 */
void findMyVoice() {
    initialize_voice_data();
    clear_screen();
    printf("========================================\n");
    printf("         B. Find My Voice\n");
    printf("========================================\n");

    printf("Enter your nickname: ");
    char nickname_buf[50];
    fgets(nickname_buf, sizeof(nickname_buf), stdin);
    nickname_buf[strcspn(nickname_buf, "\n")] = 0;
    
    int member_idx = -1;
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(strcmp(nickname_buf, g_milliways_members[i][1])==0){
            member_idx = i;
            break;
        }
    }
    if(member_idx == -1) { printf("Nickname not found. Returning.\n"); return; }
    
    char gender = g_milliways_members[member_idx][2][0];
    MyVoice* p_current_user_voice = &g_member_voice_data[member_idx];
    
    // Generate a new random voice profile for the session
    getMyVoiceFromHeaven(p_current_user_voice, gender);

    // Using a Function Pointer to select the game mode
    void (*p_game_function)(MyVoice*);
    
    printf("\n--- Welcome, %s! ---\n", p_current_user_voice->nickname);
    printf("A random voice profile has been generated for you based on your gender (%c).\n", gender);
    printf("Play the up-and-down game to find the values!\n");
    
    printf("\nSelect a game to play:\n");
    printf("  1. Find My Voice (Freq, Amp, Jitter)\n");
    printf("  2. Find My Voice Tone (GRBAS) [Bonus]\n");
    printf("Choice: ");
    
    char buffer[10];
    fgets(buffer, sizeof(buffer), stdin);
    
    if(buffer[0] == '2') {
        p_game_function = &runGRBASGame;
    } else {
        p_game_function = &runMainGame;
    }

    // Call the selected function via the pointer
    p_game_function(p_current_user_voice);
    
    printf("\nPress Enter to return to menu...");
    getchar();
}
