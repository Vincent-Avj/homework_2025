/**
 * @file main.c
 * @brief A self-contained version of the "Vocal Evaluation" feature for online compilers.
 *
 * This program combines all necessary functions into a single file. It allows
 * for the evaluation of trainees' vocal skills by generating a random set of
 * MIDI notes within their range, calculating a "stability" score, and
 * providing qualitative feedback. The results for all members are then
 * sorted and displayed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h> // <-- ADDED THIS LINE TO FIX THE WARNING

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define MAX_FEEDBACK_LEN 51
#define MAX_MIDI_VALUES 12

// Structure to hold the complete vocal evaluation for one trainee
typedef struct {
    char name[50];
    int vocal_range_low;
    int vocal_range_high;
    double vocal_stability;
    char feedback[MAX_FEEDBACK_LEN];
} VocalRecord;


// --- Global Data ---
const char* g_milliways_names[NUM_MEMBERS] = {"Jiyeon Park", "Ethan Smith", "Helena Silva", "Liam Wilson"};


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_vocal_menu();
void evalVocal();
void measure(VocalRecord* p_record, const char* name);
void provideFeedback(VocalRecord* p_record);
void printVocalInfo(VocalRecord* records, int count);
int compare_integers(const void* a, const void* b);
int compare_by_stability(const void* a, const void* b);


// --- Main Entry Point ---
int main() {
    // Seed the random number generator once at the start of the program
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
    char choice_buf[10];
    char choice_char;
    do {
        clear_screen();
        printf("========================================\n");
        printf("      Milliways Management System\n");
        printf("========================================\n");
        printf("   II. Training\n");
        printf("   0. Exit Program\n");
        printf("========================================\n");
        printf("Select an option: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        choice_char = choice_buf[0];

        if (choice_char == '2') {
             show_training_menu();
        }
    } while (choice_char != '0' && toupper(choice_char) != 'Q');
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
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (choice == 'A' || choice == 'a') {
            evalVocal();
        }
    } while (choice != '0');
}


// --- Feature-Specific Functions ---

// Comparison function for qsort to sort MIDI values (integers)
int compare_integers(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Comparison function for qsort to sort VocalRecord structs by stability
int compare_by_stability(const void* a, const void* b) {
    VocalRecord* rec_a = (VocalRecord*)a;
    VocalRecord* rec_b = (VocalRecord*)b;
    if (rec_a->vocal_stability < rec_b->vocal_stability) return -1;
    if (rec_a->vocal_stability > rec_b->vocal_stability) return 1;
    return 0;
}

/**
 * @brief Prompts a trainer to provide written feedback for a trainee.
 */
void provideFeedback(VocalRecord* p_record) {
    printf("Please provide feedback for %s (max 50 characters):\n> ", p_record->name);
    
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Ensure feedback does not exceed the length limit
    strncpy(p_record->feedback, buffer, MAX_FEEDBACK_LEN - 1);
    p_record->feedback[MAX_FEEDBACK_LEN - 1] = '\0'; // Ensure null termination
}

/**
 * @brief Conducts the vocal measurement for a single trainee.
 */
void measure(VocalRecord* p_record, const char* name) {
    strcpy(p_record->name, name);
    
    // --- Bonus: Get vocal range and number of notes ---
    char buffer[100];
    int num_notes_to_generate = 10; // Default

    printf("\nEnter vocal range and note count for %s (Format: low,high,count e.g., 48,72,10)\n> ", name);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d,%d,%d", &p_record->vocal_range_low, &p_record->vocal_range_high, &num_notes_to_generate);

    // --- Validation ---
    if (num_notes_to_generate < 6 || num_notes_to_generate > 12) {
        printf("Invalid note count (must be 6-12). Defaulting to 10.\n");
        num_notes_to_generate = 10;
    }
    int range_size = p_record->vocal_range_high - p_record->vocal_range_low + 1;
    if (range_size <= 0) {
        printf("Invalid range. Setting to default (48-72).\n");
        p_record->vocal_range_low = 48;
        p_record->vocal_range_high = 72;
        range_size = 25;
    }
    if (num_notes_to_generate > range_size) {
        printf("Cannot generate %d unique notes from a range of size %d. Adjusting to %d.\n", num_notes_to_generate, range_size, range_size);
        num_notes_to_generate = range_size;
    }

    // --- Generate unique random MIDI notes ---
    printf("Generating %d unique MIDI notes between %d and %d...\n", num_notes_to_generate, p_record->vocal_range_low, p_record->vocal_range_high);
    // Seed the generator with the high note, as required
    srand((unsigned int)p_record->vocal_range_high);

    int* full_range = (int*)malloc(range_size * sizeof(int));
    int* midi_values = (int*)malloc(num_notes_to_generate * sizeof(int));

    for (int i = 0; i < range_size; i++) {
        full_range[i] = p_record->vocal_range_low + i;
    }
    // Shuffle the full range of possible notes
    for (int i = range_size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = full_range[i];
        full_range[i] = full_range[j];
        full_range[j] = temp;
    }
    // Take the first N notes from the shuffled list
    for (int i = 0; i < num_notes_to_generate; i++) {
        midi_values[i] = full_range[i];
    }
    free(full_range);
    
    // --- Calculate Vocal Stability ---
    // 1. Sort the notes to find differences between neighbors
    qsort(midi_values, num_notes_to_generate, sizeof(int), compare_integers);
    
    // 2. Sum the absolute differences
    double diff_sum = 0;
    for (int i = 0; i < num_notes_to_generate - 1; i++) {
        diff_sum += fabs((double)midi_values[i+1] - midi_values[i]);
    }
    
    // 3. Calculate the average and store it
    if (num_notes_to_generate > 1) {
        p_record->vocal_stability = diff_sum / (num_notes_to_generate - 1);
    } else {
        p_record->vocal_stability = 0.0;
    }
    free(midi_values);
    
    // --- Get Qualitative Feedback ---
    provideFeedback(p_record);
}

/**
 * @brief Prints the final, sorted evaluation report for all trainees.
 */
void printVocalInfo(VocalRecord* records, int count) {
    printf("\n======================================================\n");
    printf("         Final Vocal Evaluation Report\n");
    printf("======================================================\n");

    // Sort the records by stability score (lowest is best)
    qsort(records, count, sizeof(VocalRecord), compare_by_stability);

    printf("%-20s | %-10s | %-50s\n", "Trainee", "Stability", "Feedback");
    printf("------------------------------------------------------------------------------------------\n");

    // Print using standard array indexing
    for (int i = 0; i < count; i++) {
        printf("%-20s | %-10.2f | %-50s\n", records[i].name, records[i].vocal_stability, records[i].feedback);
    }
}

/**
 * @brief The main function for the Vocal Evaluation feature.
 */
void evalVocal() {
    clear_screen();
    printf("========================================\n");
    printf("         A. Vocal Evaluation\n");
    printf("========================================\n");
    
    VocalRecord results[NUM_MEMBERS];

    // Evaluate each member
    for (int i = 0; i < NUM_MEMBERS; i++) {
        measure(&results[i], g_milliways_names[i]);
    }
    
    printf("\nAll evaluations complete. Generating sorted report...\n");
    
    // Display the final sorted results
    printVocalInfo(results, NUM_MEMBERS);

    printf("\nPress Enter to return to menu...");
    getchar();
}
