/**
 * @file decision.c
 * @brief A program to determine the debut status of the "Milliways" idol group.
 *
 * This program simulates the final evaluation process for a K-POP group.
 * It takes team-wide passing criteria as input, processes pre-defined scores
 * for each member, and checks them against the criteria. Based on the final
 * aggregated score, it uses an enum and switch statement to determine the
 * group's fate, including identifying the lowest-scoring categories for a
 * potential retake, fulfilling all bonus requirements.
 */

#include <stdio.h>
#include <math.h> // Allowed by the constraints

// --- Constants and Global Data ---
#define NUM_MEMBERS 4
#define NUM_CATEGORIES 9
#define SCORE_MIN 0
#define SCORE_MAX 100

// Using a 1D array to store scores, as required.
// Access will be: g_scores[member_idx * NUM_CATEGORIES + category_idx]
int g_scores[NUM_MEMBERS * NUM_CATEGORIES];

const char* CATEGORY_NAMES[NUM_CATEGORIES] = {
    "Physical Strength", "Knowledge", "Self-Management", "Teamwork",
    "Korean Language", "Vocal", "Dance", "Visual", "Acting"
};

// Enum to represent the final decision outcomes
typedef enum {
    DEBUT_CONFIRMED,
    FINAL_EVALUATION,
    RETAKE_NEEDED,
    DEBUT_FAILED
} DecisionResult;


// --- Custom Utility Functions (since stdlib.h is disallowed) ---

/**
 * @brief A custom implementation of atoi to convert a string to an integer.
 */
int my_atoi(const char s[]) {
    int result = 0;
    int i = 0;
    // Handle potential whitespace
    while (s[i] == ' ' || s[i] == '\t') {
        i++;
    }
    // Convert number part
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    return result;
}

/**
 * @brief A helper function to truncate a double to an integer.
 * This is equivalent to a floor operation for positive numbers.
 */
int truncate_double(double num) {
    return (int)num;
}


// --- Core Logic Functions ---

/**
* @brief Simulates data entry by populating the global scores array.
* In a real application, this would take user input.
*/
void enter_all_scores() {
    // Hardcoded scores for demonstration purposes.
    // Scores for Member 0 (Ariel)
    g_scores[0*NUM_CATEGORIES+0] = 95; g_scores[0*NUM_CATEGORIES+1] = 88; g_scores[0*NUM_CATEGORIES+2] = 92;
    g_scores[0*NUM_CATEGORIES+3] = 94; g_scores[0*NUM_CATEGORIES+4] = 98; g_scores[0*NUM_CATEGORIES+5] = 91;
    g_scores[0*NUM_CATEGORIES+6] = 96; g_scores[0*NUM_CATEGORIES+7] = 93; g_scores[0*NUM_CATEGORIES+8] = 90;
    // Scores for Member 1 (Simba)
    g_scores[1*NUM_CATEGORIES+0] = 89; g_scores[1*NUM_CATEGORIES+1] = 91; g_scores[1*NUM_CATEGORIES+2] = 85;
    g_scores[1*NUM_CATEGORIES+3] = 93; g_scores[1*NUM_CATEGORIES+4] = 84; g_scores[1*NUM_CATEGORIES+5] = 95;
    g_scores[1*NUM_CATEGORIES+6] = 90; g_scores[1*NUM_CATEGORIES+7] = 88; g_scores[1*NUM_CATEGORIES+8] = 87;
    // Scores for Member 2 (Belle)
    g_scores[2*NUM_CATEGORIES+0] = 92; g_scores[2*NUM_CATEGORIES+1] = 94; g_scores[2*NUM_CATEGORIES+2] = 90;
    g_scores[2*NUM_CATEGORIES+3] = 88; g_scores[2*NUM_CATEGORIES+4] = 89; g_scores[2*NUM_CATEGORIES+5] = 85;
    g_scores[2*NUM_CATEGORIES+6] = 93; g_scores[2*NUM_CATEGORIES+7] = 97; g_scores[2*NUM_CATEGORIES+8] = 91;
    // Scores for Member 3 (Aladdin)
    g_scores[3*NUM_CATEGORIES+0] = 88; g_scores[3*NUM_CATEGORIES+1] = 86; g_scores[3*NUM_CATEGORIES+2] = 93;
    g_scores[3*NUM_CATEGORIES+3] = 95; g_scores[3*NUM_CATEGORIES+4] = 88; g_scores[3*NUM_CATEGORIES+5] = 92;
    g_scores[3*NUM_CATEGORIES+6] = 94; g_scores[3*NUM_CATEGORIES+7] = 90; g_scores[3*NUM_CATEGORIES+8] = 85;
    
    printf("All member scores have been entered.\n");
}

/**
* @brief Finds the three lowest-scoring categories and prints them.
*/
void find_and_print_lowest_three(const double category_averages[]) {
    // A simple approach to find the 3 lowest without a full sort
    int lowest_indices[3] = {-1, -1, -1};
    double lowest_scores[3] = {101.0, 101.0, 101.0}; // Initialize with a high value

    for(int i=0; i<NUM_CATEGORIES; i++) {
        if(category_averages[i] < lowest_scores[0]) {
            lowest_scores[2] = lowest_scores[1]; lowest_indices[2] = lowest_indices[1];
            lowest_scores[1] = lowest_scores[0]; lowest_indices[1] = lowest_indices[0];
            lowest_scores[0] = category_averages[i]; lowest_indices[0] = i;
        } else if (category_averages[i] < lowest_scores[1]) {
            lowest_scores[2] = lowest_scores[1]; lowest_indices[2] = lowest_indices[1];
            lowest_scores[1] = category_averages[i]; lowest_indices[1] = i;
        } else if (category_averages[i] < lowest_scores[2]) {
            lowest_scores[2] = category_averages[i]; lowest_indices[2] = i;
        }
    }
    
    printf("Retake the three lowest-scoring categories ({%s}, {%s}, {%s}) after 30 days.\n",
        CATEGORY_NAMES[lowest_indices[0]],
        CATEGORY_NAMES[lowest_indices[1]],
        CATEGORY_NAMES[lowest_indices[2]]
    );
}

// --- Main Program Entry Point ---
int main(void) {
    int team_avg_pass_score;
    int min_individual_score;
    char buffer[50];
    
    printf("========================================\n");
    printf("      Milliways Debut Decision System\n");
    printf("========================================\n");
    
    // 1. Set Passing Criteria
    printf("Enter Team Average Passing Score (0-100): ");
    fgets(buffer, sizeof(buffer), stdin);
    team_avg_pass_score = my_atoi(buffer);
    
    printf("Enter Minimum Individual Score for any category (0-100): ");
    fgets(buffer, sizeof(buffer), stdin);
    min_individual_score = my_atoi(buffer);
    
    // 2. Load Scores
    enter_all_scores();

    // 3. Check for immediate failure based on minimum individual score
    for (int i = 0; i < NUM_MEMBERS * NUM_CATEGORIES; i++) {
        if (g_scores[i] < min_individual_score) {
            int member_idx = i / NUM_CATEGORIES;
            int cat_idx = i % NUM_CATEGORIES;
            printf("\n--- IMMEDIATE FAILURE ---\n");
            printf("A score of %d in '%s' is below the minimum required score of %d.\n",
                   g_scores[i], CATEGORY_NAMES[cat_idx], min_individual_score);
            printf("The entire team must retake the '%s' category.\n", CATEGORY_NAMES[cat_idx]);
            printf("Program terminated.\n");
            return 1;
        }
    }
    
    // 4. Calculate Average Scores
    double category_averages[NUM_CATEGORIES] = {0.0};
    double total_score_sum = 0;

    for (int cat = 0; cat < NUM_CATEGORIES; cat++) {
        int category_sum = 0;
        for (int mem = 0; mem < NUM_MEMBERS; mem++) {
            category_sum += g_scores[mem * NUM_CATEGORIES + cat];
        }
        category_averages[cat] = (double)category_sum / NUM_MEMBERS;
        total_score_sum += category_averages[cat];
    }
    
    // The final total score is the average of all category averages
    int final_total_score = truncate_double(total_score_sum / NUM_CATEGORIES);
    
    printf("\n--- Preliminary Results ---\n");
    printf("Calculated Team Average Score: %d\n", final_total_score);

    // 5. Check against team average criteria
    if (final_total_score < team_avg_pass_score) {
        printf("\n--- TEAM FAILURE ---\n");
        printf("The team's average score of %d is below the required passing score of %d.\n",
               final_total_score, team_avg_pass_score);
        printf("Program terminated.\n");
        return 1;
    }

    // 6. (Bonus) Determine final fate using enum and switch
    DecisionResult final_decision;

    if (final_total_score >= 95) {
        final_decision = DEBUT_CONFIRMED;
    } else if (final_total_score >= 90) {
        final_decision = FINAL_EVALUATION;
    } else if (final_total_score >= 80) {
        final_decision = RETAKE_NEEDED;
    } else {
        final_decision = DEBUT_FAILED;
    }
    
    printf("\n--- FINAL DECISION ---\n");
    switch (final_decision) {
        case DEBUT_CONFIRMED:
            printf("Congratulations! Your stage debut is confirmed!\n");
            break;
        case FINAL_EVALUATION:
            printf("The Debut Certification Team will hold a final evaluation meeting to make a decision.\n");
            break;
        case RETAKE_NEEDED:
            find_and_print_lowest_three(category_averages);
            break;
        case DEBUT_FAILED:
            printf("Unfortunately, the team did not meet the requirements for debut at this time.\n");
            break;
    }

    return 0;
}
