/**
 * @file stage2.c
 * @brief A module for Self-Management and Teamwork training, including Mentoring.
 *
 * This file implements the logic for the mentoring feature, which matches
 * trainees with available mentors. The core logic uses a shuffling algorithm
 * to ensure a fair, random, and unique 1:1 pairing, fulfilling the bonus
 * requirement.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constants and Data Structures ---

#define NUM_MEMBERS 4
#define MAX_MENTORS 8

// From Course 1, for reference
const char* milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};

// Structure to hold generated data for a trainee
typedef struct {
    char nickname[50];
    int ascii_sum;
    int ability_score;
} TraineeData;

// Structure to hold information about a single mentor
typedef struct {
    int id;
    char name[100];
    char assigned_mentee_nickname[50]; // To store the matching result
} Mentor;


// --- Utility and Core Logic Functions ---

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Converts a trainee's nickname into a summed ASCII code integer.
 * @param nickname The nickname string to parse.
 * @return The sum of the ASCII values of all characters in the nickname.
 */
int parseIntMember(const char* nickname) {
    int sum = 0;
    for (int i = 0; nickname[i] != '\0'; i++) {
        sum += (int)nickname[i];
    }
    return sum;
}

/**
 * @brief Generates a random ability score for a trainee.
 * @return A random integer between 100 and 1000.
 */
int getRandomAbility() {
    return (rand() % 901) + 100; // Generates a number from 100 to 1000
}

/**
 * @brief Shuffles an array of pointers to Mentor structs. (Fisher-Yates shuffle)
 * This is key to guaranteeing a random, 1:1 match.
 * @param array The array of Mentor pointers to shuffle.
 * @param n The number of elements in the array.
 */
void shuffle_mentor_pointers(Mentor** array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Mentor* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


/**
 * @brief The main function for the Mentoring feature.
 */
void matchMentoring() {
    clear_screen();
    printf("========================================\n");
    printf("      A. Mentoring Matching\n");
    printf("========================================\n");

    // 1. Initialize Trainee Data
    TraineeData trainees[NUM_MEMBERS];
    printf("Initializing trainee data...\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        strcpy(trainees[i].nickname, milliways_members[i][1]);
        trainees[i].ascii_sum = parseIntMember(trainees[i].nickname);
        trainees[i].ability_score = getRandomAbility();
    }
    printf("Trainee data successfully generated.\n\n");

    // 2. Input Mentor Data from the user
    Mentor mentors[MAX_MENTORS];
    int mentor_count = 0;
    char name_buffer[100];

    printf("--- Enter Mentor Information (up to %d mentors) ---\n", MAX_MENTORS);
    printf("Type 'done' when you are finished entering names.\n");
    while (mentor_count < MAX_MENTORS) {
        printf("Enter name for Mentor %d: ", mentor_count + 1);
        fgets(name_buffer, sizeof(name_buffer), stdin);
        name_buffer[strcspn(name_buffer, "\n")] = 0; // Remove newline

        if (strcmp(name_buffer, "done") == 0) {
            if (mentor_count < NUM_MEMBERS) {
                printf("Error: You must enter at least %d mentors to match all trainees.\n", NUM_MEMBERS);
                continue; // Prompt again
            }
            break; // Exit loop
        }
        
        mentors[mentor_count].id = mentor_count + 1;
        strcpy(mentors[mentor_count].name, name_buffer);
        strcpy(mentors[mentor_count].assigned_mentee_nickname, "None"); // Initialize
        mentor_count++;
    }

    // --- (Bonus) 1:1 Random Matching ---
    printf("\nPerforming 1:1 random matching...\n");
    
    // Create an array of pointers to our mentors
    Mentor* mentor_pointers[mentor_count];
    for(int i=0; i < mentor_count; i++) {
        mentor_pointers[i] = &mentors[i];
    }
    
    // Shuffle the array of pointers
    shuffle_mentor_pointers(mentor_pointers, mentor_count);
    
    // Assign the first N shuffled mentors to the N trainees
    for(int i=0; i < NUM_MEMBERS; i++) {
        strcpy(mentor_pointers[i]->assigned_mentee_nickname, trainees[i].nickname);
    }
    
    printf("Matching complete!\n");

    // 4. Output the results
    printf("\n========================================\n");
    printf("          Mentoring Pairings\n");
    printf("========================================\n");
    printf("| %-5s | %-15s | %-10s | %-20s |\n", "#", "Trainee", "Mentor ID", "Mentor Name");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < NUM_MEMBERS; i++) {
        // The i-th trainee is matched with the i-th mentor from the shuffled list
        printf("| %-5d | %-15s | %-10d | %-20s |\n",
            i + 1,
            trainees[i].nickname,
            mentor_pointers[i]->id,
            mentor_pointers[i]->name
        );
    }
    printf("----------------------------------------------------------\n");
    
    printf("\nPress Enter to return to the menu...");
    getchar();
}


// --- Menu System ---
// This would be part of a larger menu system in a real project.

void show_self_mgmt_menu() {
    char choice;
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 2. Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   A. Mentoring\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        choice = getchar();
        while(getchar() != '\n'); // Clear input buffer

        if (choice == '0') break;

        switch(toupper(choice)) {
            case 'A':
                matchMentoring();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
        }
    }
}

// Dummy functions for context, assuming they exist elsewhere
void run_training_system() {
    show_self_mgmt_menu();
}

int main(void) {
    // Seed the random number generator once at the start of the program
    srand(time(NULL));

    // For this standalone example, we'll just call the main feature.
    // In the full project, this would be part of the main menu loop.
    run_training_system();

    return 0;
}
