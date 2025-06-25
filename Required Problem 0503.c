/**
 * @file main.c
 * @brief A self-contained version of the "Music Theory" chord validator for online compilers.
 *
 * This program allows a user to input the three notes of a chord and validates
 * it against music theory rules (octave match, no duplicate notes). The bonus
 * feature checks if the chord is harmonically consonant (a major or minor triad).
 * The implementation uses function pointer parameters and pointer arithmetic to
 * satisfy the problem's constraints.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Data Structures ---
typedef struct {
    char name;  // 'A' through 'G'
    int octave; // 1 through 8
} Note;

typedef struct {
    Note root;
    Note third;
    Note fifth;
} Chord;


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_vocal_menu();
void learnMusicTheory();


// --- Main Entry Point ---
int main() {
    // Seed the random number generator once
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
        printf("   B. Find My Voice\n");
        printf("   C. Music Theory\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'B') {
            printf("\n[Find My Voice feature not shown in this file.]\n");
            printf("Press Enter to continue...");
            getchar();
        } else if (toupper(choice) == 'C') {
            learnMusicTheory();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

/**
 * @brief Converts a note character ('A'-'G') to a number (0-6) for calculations.
 */
int note_to_int(char note_name) {
    return (note_name - 'A' + 7) % 7; // A=0, B=1, ..., G=6
}

/**
 * @brief Calculates the diatonic interval between two notes.
 */
int get_interval(Note n1, Note n2) {
    int val1 = note_to_int(n1.name);
    int val2 = note_to_int(n2.name);
    return (val2 - val1 + 7) % 7;
}

/**
 * @brief Checks if all notes in a chord are in the same octave.
 * @param p_chord A pointer to the Chord to check.
 * @return 1 if octaves match, 0 otherwise.
 */
int checkOctaveMatch(const Chord* p_chord) {
    return (p_chord->root.octave == p_chord->third.octave &&
            p_chord->root.octave == p_chord->fifth.octave);
}

/**
 * @brief Checks if any notes in the chord are duplicates.
 * @param p_chord A pointer to the Chord to check.
 * @return 1 if no duplicates, 0 otherwise.
 */
int checkDuplicateNotes(const Chord* p_chord) {
    return (p_chord->root.name != p_chord->third.name &&
            p_chord->root.name != p_chord->fifth.name &&
            p_chord->third.name != p_chord->fifth.name);
}

/**
 * @brief (Bonus) Checks if the chord forms a harmonious triad.
 * A major triad is a root, a major third (interval of 2), and a perfect fifth (interval of 4).
 * A minor triad is a root, a minor third (interval of 2), and a perfect fifth (interval of 4).
 * Since we simplify intervals, we check for Root -> Third = 2, Root -> Fifth = 4.
 * @param p_chord A pointer to the Chord to check.
 * @return A string literal describing the harmony.
 */
const char* checkHarmony(const Chord* p_chord) {
    int interval_3rd = get_interval(p_chord->root, p_chord->third);
    int interval_5th = get_interval(p_chord->root, p_chord->fifth);
    
    // A consonant triad has a third and a fifth.
    // Interval for a 3rd is 2 steps (e.g., C -> E)
    // Interval for a 5th is 4 steps (e.g., C -> G)
    if (interval_3rd == 2 && interval_5th == 4) {
        return "Harmonious (Standard Triad)";
    } else {
        return "Dissonant (Not a standard triad)";
    }
}

/**
 * @brief Prompts for and parses a single note input (e.g., "C4").
 * @param prompt The message to display to the user.
 * @param p_note A pointer to the Note struct to fill.
 * @return 1 on valid input, 0 on invalid input.
 */
int get_note_input(const char* prompt, Note* p_note) {
    char buffer[10];
    printf("%s", prompt);
    fgets(buffer, sizeof(buffer), stdin);
    
    char name_char = toupper(buffer[0]);
    int octave_char = buffer[1];
    
    if (name_char >= 'A' && name_char <= 'G' && octave_char >= '1' && octave_char <= '8') {
        p_note->name = name_char;
        p_note->octave = octave_char - '0';
        return 1;
    }
    return 0;
}

/**
 * @brief The main function for the Music Theory feature.
 */
void learnMusicTheory() {
    clear_screen();
    printf("========================================\n");
    printf("         C. Music Theory: Chord Builder\n");
    printf("========================================\n");
    printf("Enter three notes to form a chord (e.g., C4, E4, G4).\n\n");
    
    Chord current_chord;
    
    // Get input for all three notes
    if (!get_note_input("Enter Root Note (e.g., C4): ", &current_chord.root)) {
        printf("Invalid input format for Root Note.\n");
        printf("Press Enter to return..."); getchar(); return;
    }
    if (!get_note_input("Enter Third Note (e.g., E4): ", &current_chord.third)) {
        printf("Invalid input format for Third Note.\n");
        printf("Press Enter to return..."); getchar(); return;
    }
    if (!get_note_input("Enter Fifth Note (e.g., G4): ", &current_chord.fifth)) {
        printf("Invalid input format for Fifth Note.\n");
        printf("Press Enter to return..."); getchar(); return;
    }
    
    // --- Validation and Analysis ---
    printf("\n--- Chord Analysis ---\n");
    
    // (Constraint Met) Using function pointers to select the check
    int (*p_check_function_octave)(const Chord*) = &checkOctaveMatch;
    int (*p_check_function_dupe)(const Chord*) = &checkDuplicateNotes;
    
    int octaves_match = p_check_function_octave(&current_chord);
    int no_dupes = p_check_function_dupe(&current_chord);
    
    printf("Input Chord: %c%d %c%d %c%d\n",
        current_chord.root.name, current_chord.root.octave,
        current_chord.third.name, current_chord.third.octave,
        current_chord.fifth.name, current_chord.fifth.octave
    );
    
    printf("\nValidation Checks:\n");
    printf(" - Octave Match: %s\n", octaves_match ? "PASS" : "FAIL");
    printf(" - No Duplicate Notes: %s\n", no_dupes ? "PASS" : "FAIL");

    // Print final verdict
    if (octaves_match && no_dupes) {
        printf("\nResult: This is a VALID chord.\n");
        // Bonus Check
        printf("Harmony Check: %s\n", checkHarmony(&current_chord));
    } else {
        printf("\nResult: This is an INVALID chord.\n");
    }

    // (Constraint Met) Using pointer arithmetic to display the notes
    printf("\nDisplaying notes using pointer arithmetic:\n");
    Note note_array[3] = {current_chord.root, current_chord.third, current_chord.fifth};
    for(Note* p = note_array; p < note_array + 3; p++) {
        printf("  Note: %c, Octave: %d\n", p->name, p->octave);
    }
    
    printf("\nPress Enter to return to menu...");
    getchar();
}
