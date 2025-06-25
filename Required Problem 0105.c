/**
 * @file scorecard.c
 * @brief A program to simulate audition scoring by multiple judges.
 *
 * This program allows a specified number of judges to enter scores for six
 * candidates across five categories. It includes input validation and a review
 * and modification system. After all judges submit their scores, it calculates
 * the final results and selects the top four candidates.
 */

#include <stdio.h>
#include <stdlib.h> // For malloc, free, atoi, srand, rand
#include <string.h> // Includes key string functions
#include <time.h>   // For seeding the random number generator
#include <ctype.h>  // For toupper()

// --- Constants and Global Data ---
#define NUM_CANDIDATES 6
#define NUM_JUDGES 5
#define NUM_CATEGORIES 5

// Data Structures
typedef struct {
    char name[50];
    int id;
} Candidate;

// Global Data Arrays
Candidate g_candidates[NUM_CANDIDATES];
int g_all_scores[NUM_JUDGES][NUM_CANDIDATES][NUM_CATEGORIES];


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


// --- Core Logic Functions ---

/**
 * @brief Generates a unique 6-digit ID that is not already in use.
 */
int generate_unique_id() {
    int new_id;
    int is_unique;
    do {
        is_unique = 1;
        new_id = 100000 + (rand() % 900000); // Generate a 6-digit number
        for (int i = 0; i < NUM_CANDIDATES; i++) {
            if (g_candidates[i].id == new_id) {
                is_unique = 0;
                break;
            }
        }
    } while (!is_unique);
    return new_id;
}

/**
 * @brief Initializes the global candidate array with names and unique IDs.
 */
void initialize_candidates() {
    const char* names[] = {"Jiyeon Park", "Ethan Smith", "Suphanan Wong", "Helena Silva", "Karolina Nowak", "Liam Wilson"};
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        strcpy(g_candidates[i].name, names[i]);
        g_candidates[i].id = generate_unique_id();
    }
}

/**
 * @brief Prompts for and validates a single score input.
 * @return An integer score between 10 and 100.
 */
int get_valid_score() {
    int score;
    char buffer[10];
    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        score = atoi(buffer);
        if (score >= 10 && score <= 100) {
            return score;
        }
        printf("Invalid score. Please enter a value between 10 and 100: ");
    }
}

/**
 * @brief (Bonus) Handles the score modification process for a judge.
 * @param judge_index The index of the current judge making modifications.
 * @param temp_scores A temporary 2D array holding the judge's current scores.
 */
void handle_score_modification(int judge_index, int temp_scores[NUM_CANDIDATES][NUM_CATEGORIES]) {
    char id_buffer[20];
    int target_id;
    
    while(1) {
        printf("\n--- Score Modification Mode ---\n");
        printf("Candidate List:\n");
        for(int i=0; i<NUM_CANDIDATES; i++) {
            printf("  - %s (ID: %d)\n", g_candidates[i].name, g_candidates[i].id);
        }
        printf("\nEnter the 6-digit ID of the candidate to modify, or '0' to finish modifying: ");
        fgets(id_buffer, sizeof(id_buffer), stdin);
        target_id = atoi(id_buffer);
        
        if (target_id == 0) break;
        
        int candidate_idx = -1;
        for(int i=0; i<NUM_CANDIDATES; i++) {
            if (g_candidates[i].id == target_id) {
                candidate_idx = i;
                break;
            }
        }

        if (candidate_idx != -1) {
            printf("Re-entering scores for %s:\n", g_candidates[candidate_idx].name);
            const char* categories[] = {"Music", "Dance", "Vocal", "Visual", "Expression"};
            for(int i=0; i<NUM_CATEGORIES; i++) {
                printf("%s Proficiency: ", categories[i]);
                temp_scores[candidate_idx][i] = get_valid_score();
            }
            printf("Scores for %s have been updated.\n", g_candidates[candidate_idx].name);
        } else {
            printf("Invalid ID. No candidate found with ID %d.\n", target_id);
        }
    }
}

/**
 * @brief Calculates final results, sorts candidates, and displays the winners.
 */
void calculate_and_display_final_results() {
    printf("\n=======================================\n");
    printf("Compiling final selection results...\n");
    printf("=======================================\n");

    // Create a temporary structure to hold names and total scores for sorting
    struct Finalist {
        char name[50];
        int total_score;
    };
    
    struct Finalist finalists[NUM_CANDIDATES];

    // Calculate total score for each candidate
    for(int c=0; c < NUM_CANDIDATES; c++) {
        strcpy(finalists[c].name, g_candidates[c].name);
        finalists[c].total_score = 0;
        for(int j=0; j < NUM_JUDGES; j++) {
            for(int cat=0; cat < NUM_CATEGORIES; cat++) {
                finalists[c].total_score += g_all_scores[j][c][cat];
            }
        }
    }

    // Sort the finalists using a simple Bubble Sort (descending order)
    for(int i=0; i < NUM_CANDIDATES - 1; i++) {
        for(int j=0; j < NUM_CANDIDATES - i - 1; j++) {
            if (finalists[j].total_score < finalists[j+1].total_score) {
                struct Finalist temp = finalists[j];
                finalists[j] = finalists[j+1];
                finalists[j+1] = temp;
            }
        }
    }
    
    // Display the results
    printf("###########################################\n");
    printf("# Congratulations! Welcome to Milliways!  #\n");
    printf("###########################################\n");
    for(int i=0; i<4; i++){
        printf("%d. %s (Total Score: %d)\n", i+1, finalists[i].name, finalists[i].total_score);
    }
}


// --- Main Program Entry Point ---
int main(void) {
    // Seed the random number generator once at the start
    srand(time(NULL));
    
    initialize_candidates();

    const char* judge_names[] = {"Youngsoo Kim", "Jiyoung Lee", "Minjun Park", "Hyunwoo Choi", "Jiyoung Song"};
    const char* judge_expertises[] = {"Music", "Dance", "Vocal", "Visual", "Critic"};
    const char* score_categories[] = {"Music Proficiency", "Dance", "Vocal", "Visual", "Expression"};
    
    // Main loop for each judge
    for (int j = 0; j < NUM_JUDGES; j++) {
        
        int submission_confirmed = 0;
        // This loop allows a judge to re-enter scores if they choose 'N'
        while(!submission_confirmed) {
            clear_screen();
            printf("####################################\n");
            printf("#     Audition Evaluation Entry    #\n");
            printf("####################################\n");
            printf("> Judge Name: %s\n", judge_names[j]);
            printf("> Expertise: %s\n", judge_expertises[j]);
            printf("++++++++++++++++++++++++++++++++++++\n");
            
            // Temporary storage for the current judge's scores
            int temp_scores[NUM_CANDIDATES][NUM_CATEGORIES];

            for(int c = 0; c < NUM_CANDIDATES; c++) {
                printf("Candidate: %s\n", g_candidates[c].name);
                for(int cat=0; cat < NUM_CATEGORIES; cat++) {
                    printf("%s: ", score_categories[cat]);
                    temp_scores[c][cat] = get_valid_score();
                }
                 printf("------------------------------------\n");
            }
            
            // Review and Confirmation
            printf("++++++++++++++++++++++++++++++++++++\n");
            printf("Submission completed.\nPlease review your input!\n");
            printf("------------------------------------\n");
            for(int c = 0; c < NUM_CANDIDATES; c++) {
                printf("%s: ", g_candidates[c].name);
                for(int cat=0; cat < NUM_CATEGORIES; cat++) {
                    printf("%d%s", temp_scores[c][cat], (cat == NUM_CATEGORIES-1) ? "" : ", ");
                }
                printf("\n");
            }
            
            printf("Would you like to submit? (Y/N): ");
            char choice_buffer[10];
            fgets(choice_buffer, sizeof(choice_buffer), stdin);
            
            if (toupper(choice_buffer[0]) == 'Y') {
                // Copy temporary scores to the global array
                for(int c=0; c<NUM_CANDIDATES; c++) {
                    for(int cat=0; cat < NUM_CATEGORIES; cat++) {
                        g_all_scores[j][c][cat] = temp_scores[c][cat];
                    }
                }
                printf("***Final submission completed.***\n");
                submission_confirmed = 1;
            } else {
                printf("Submission cancelled. You can now modify the scores.\n");
                handle_score_modification(j, temp_scores); // Bonus feature call
            }
        }
        printf("\nPress Enter for the next judge...");
        getchar();
    }
    
    // All judges have submitted their scores, now calculate final results
    calculate_and_display_final_results();

    return 0;
}
