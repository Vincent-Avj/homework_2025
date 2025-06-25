/**
 * @file stage1.c
 * @brief A state-aware, hierarchical menu system for the Magrathea project.
 *
 * This program implements the main menu and a multi-stage training menu.
 * This version adds full functionality for "Stage 1: Physical Strength & Knowledge",
 * allowing a user to input and view detailed fitness data for each member.
 * It includes a bonus feature to display a member's full name with their data.
 */

#include <stdio.h>
#include <stdlib.h> // For system(), atoi(), atof()
#include <string.h> // For strcmp(), strcspn(), strtok()
#include <ctype.h>  // For toupper()

// --- Constants and Global Data ---

#define NUM_MEMBERS 4
#define NUM_FITNESS_TESTS 7

// Structure to hold one member's complete fitness scores
typedef struct {
    char nickname[50];
    float scores[NUM_FITNESS_TESTS];
} FitnessRecord;

// Global array to store the health scores. It persists between function calls.
FitnessRecord g_health_scores[NUM_MEMBERS];
int g_fitness_data_entered = 0; // Flag to check if data has been entered

// Static, predefined data for members and tests
const char* milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};

const char* FITNESS_TEST_NAMES[NUM_FITNESS_TESTS] = {
    "1-Mile Run (min)", "100m Sprint (sec)", "30 Push-ups (min)",
    "50 Squats (min)", "50 Arm Strength Push-ups (min)",
    "400m Swim (min)", "Bench Press (x bodyweight)"
};


// --- Utility and Core Logic Functions ---

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief The dedicated parsing function for fitness data.
 * It takes a comma-separated string and populates a float array.
 * @param input_str The comma-separated string of scores.
 * @param scores A pointer to the float array to store the parsed scores.
 */
void parse_and_store_scores(char* input_str, float* scores) {
    char* token = strtok(input_str, ",");
    int i = 0;
    while (token != NULL && i < NUM_FITNESS_TESTS) {
        // Use atof (ascii to float) to convert the string token to a number
        scores[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }
}

/**
 * @brief Prompts the user to enter fitness data for all members.
 */
void setHealth() {
    clear_screen();
    printf("========================================\n");
    printf("      A. Enter Fitness Data\n");
    printf("========================================\n");
    printf("Enter the 7 test results for each member as a comma-separated list.\n");
    printf("e.g., 5.5,12.3,1.2,2.0,2.5,8.0,0.75\n\n");

    char input_buffer[256];

    // Loop through each member to get their data
    for (int i = 0; i < NUM_MEMBERS; i++) {
        // Set the nickname in our data structure
        strcpy(g_health_scores[i].nickname, milliways_members[i][1]);

        printf("Enter scores for %s (%s):\n> ", milliways_members[i][0], milliways_members[i][1]);
        fgets(input_buffer, sizeof(input_buffer), stdin);
        input_buffer[strcspn(input_buffer, "\n")] = 0; // Remove newline

        // Parse the input string and store the scores
        parse_and_store_scores(input_buffer, g_health_scores[i].scores);
    }
    
    g_fitness_data_entered = 1; // Set the flag indicating data is ready
    printf("\nAll fitness data has been successfully recorded.\n");
}

/**
 * @brief Provides various options for viewing the stored fitness data.
 */
void getHealth() {
    clear_screen();
    printf("========================================\n");
    printf("      B. View Fitness Data\n");
    printf("========================================\n");

    if (!g_fitness_data_entered) {
        printf("No fitness data has been entered yet. Please use option 'A' first.\n");
        return;
    }

    char choice_buf[10];
    while(1) {
        printf("\n--- View Options ---\n");
        printf("1. View All Member Data\n");
        printf("2. View Specific Member's Data (Bonus)\n");
        printf("3. View Specific Test for a Member\n");
        printf("0. Back to previous menu\n");
        printf("Choice: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        int choice = atoi(choice_buf);

        if (choice == 0) break;

        char nickname_buf[50];
        int member_idx = -1;

        switch(choice) {
            case 1: // View All
                printf("\n--- Complete Fitness Data for All Members ---\n");
                for(int i=0; i<NUM_MEMBERS; i++) {
                    printf("\nMember: %s (%s)\n", milliways_members[i][0], g_health_scores[i].nickname);
                    for(int j=0; j<NUM_FITNESS_TESTS; j++) {
                        printf("  - %-30s: %.2f\n", FITNESS_TEST_NAMES[j], g_health_scores[i].scores[j]);
                    }
                }
                break;

            case 2: // View by Member (Bonus)
                printf("Enter nickname to view (e.g., Ariel, Simba): ");
                fgets(nickname_buf, sizeof(nickname_buf), stdin);
                nickname_buf[strcspn(nickname_buf, "\n")] = 0;

                member_idx = -1;
                for(int i=0; i<NUM_MEMBERS; i++){
                    if(strcmp(nickname_buf, g_health_scores[i].nickname) == 0) {
                        member_idx = i;
                        break;
                    }
                }

                if(member_idx != -1) {
                    printf("\n--- Detailed Report ---\n");
                    printf("Member Name: %s\n", milliways_members[member_idx][0]);
                    printf("Nickname:    %s\n", g_health_scores[member_idx].nickname);
                    printf("--------------------------\n");
                    for(int j=0; j<NUM_FITNESS_TESTS; j++) {
                        printf("  - %-30s: %.2f\n", FITNESS_TEST_NAMES[j], g_health_scores[member_idx].scores[j]);
                    }
                } else {
                    printf("Member '%s' not found.\n", nickname_buf);
                }
                break;
            
            case 3: // View Specific Test
                 printf("Enter nickname to view (e.g., Ariel, Simba): ");
                 fgets(nickname_buf, sizeof(nickname_buf), stdin);
                 nickname_buf[strcspn(nickname_buf, "\n")] = 0;

                 member_idx = -1;
                 for(int i=0; i<NUM_MEMBERS; i++){
                    if(strcmp(nickname_buf, g_health_scores[i].nickname) == 0) {
                        member_idx = i;
                        break;
                    }
                 }
                 if(member_idx != -1) {
                     printf("Which test result to view? (1-7): ");
                     fgets(choice_buf, sizeof(choice_buf), stdin);
                     int test_choice = atoi(choice_buf);
                     if(test_choice >= 1 && test_choice <= NUM_FITNESS_TESTS) {
                         printf("\nResult for %s:\n", g_health_scores[member_idx].nickname);
                         printf("  - %s: %.2f\n", FITNESS_TEST_NAMES[test_choice-1], g_health_scores[member_idx].scores[test_choice-1]);
                     } else {
                         printf("Invalid test number.\n");
                     }
                 } else {
                     printf("Member '%s' not found.\n", nickname_buf);
                 }
                 break;

            default:
                printf("Invalid choice.\n");
        }
        printf("\nPress Enter to return to the view menu...");
        getchar();
    }
}


// --- Menu System Integration ---

/**
 * @brief This new function provides the menu for Stage 1.
 */
void show_physical_strength_menu() {
    char choice;
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        printf("   A. Enter Fitness Data\n");
        printf("   B. View Fitness Data\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        choice = getchar();
        while(getchar() != '\n'); // Clear input buffer

        if (choice == '0') break;

        switch(toupper(choice)) {
            case 'A':
                setHealth();
                break;
            case 'B':
                getHealth();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


/**
 * @brief The main training menu, modified to call the new sub-menu for Stage 1.
 */
void run_training_system() {
    while (1) {
        clear_screen();
        printf("========================================\n");
        printf("         II. Training Menu\n");
        printf("========================================\n");
        printf("   1. Physical Strength & Knowledge\n");
        printf("   2. Self-Management & Teamwork\n");
        // ... Other stages would be listed here ...
        printf("----------------------------------------\n");
        printf("   0. Return to Main Menu\n");
        printf("========================================\n");
        printf("Choice: ");
        
        char input_buf[10];
        fgets(input_buf, sizeof(input_buf), stdin);
        
        if (input_buf[0] == '0') break;
        
        int choice = atoi(input_buf);
        if (choice == 1) {
            show_physical_strength_menu(); // Call the new, dedicated menu
        } else if (choice == 2) {
             printf("\nThis feature is not yet implemented.\n");
             printf("Press Enter to continue...");
             getchar();
        } else {
            printf("\nInvalid choice. Please select a number from the menu.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    }
}


// --- Main Entry Point ---

int main(void) {
    // This is the main program loop.
    while (1) {
        clear_screen();
        printf("========================================\n");
        printf("        MAGRATHEA Main Menu\n");
        printf("========================================\n");
        printf("   I. Audition Management\n");
        printf("   II. Training\n");
        printf("   III. Debut\n");
        printf("----------------------------------------\n");
        printf("Enter a menu number, or 'Q' to quit.\n");
        printf("Choice: ");

        char input_buf[10];
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strcspn(input_buf, "\n")] = 0;

        if (strlen(input_buf) == 0 || strcmp(input_buf, "0") == 0 || toupper(input_buf[0]) == 'Q') {
            break;
        }

        if (strcmp(input_buf, "II") == 0 || strcmp(input_buf, "2") == 0) {
            run_training_system();
        } else {
            printf("\nThis feature is not yet implemented.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    }

    printf("\nExiting Magrathea System. Goodbye!\n");
    return 0;
}
