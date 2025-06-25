/**
 * @file stage1.c
 * @brief A state-aware, hierarchical menu system for the Magrathea project.
 *
 * This program implements the main menu and a multi-stage training menu.
 * It tracks the completion status of each training stage and enforces rules,
 * such as requiring stages 1 and 2 to be passed before accessing others.
 *
 * The solution includes two bonus implementations for menu data structures,
 * selectable at compile time using the -DBONUS_SINGLE_ARRAY flag.
 */

#include <stdio.h>
#include <stdlib.h> // For clear_screen()
#include <string.h> // For strcmp(), strcspn()
#include <ctype.h>  // For toupper()

// --- Constants and Global State ---

#define NUM_MAIN_MENU_ITEMS 3
#define NUM_TRAINING_STAGES 8

// This array holds the state of each training stage.
// 'N': Not Started, 'F': Failed, 'P': Passed
char g_stage_status[NUM_TRAINING_STAGES];


// --- Bonus: Menu Item Data Structures ---

#ifdef BONUS_SINGLE_ARRAY
    // Bonus A: A single 2D array for both menus.
    // We use a "type" field to distinguish between them.
    // 0 = Main Menu, 1 = Training Menu
    const char* g_menu_items[][3] = {
        {"0", "I", "Audition Management"},
        {"0", "II", "Training"},
        {"0", "III", "Debut"},
        {"1", "1", "Physical Strength & Knowledge"},
        {"1", "2", "Self-Management & Teamwork"},
        {"1", "3", "Language & Pronunciation"},
        {"1", "4", "Vocal"},
        {"1", "5", "Dance"},
        {"1", "6", "Visual & Image"},
        {"1", "7", "Acting & Stage Performance"},
        {"1", "8", "Fan Communication"}
    };
    const int g_total_menu_items = sizeof(g_menu_items) / sizeof(g_menu_items[0]);

#else
    // Bonus B (Default): Two separate 2D arrays for menu items.
    // This is generally a cleaner and more modular approach.
    const char* g_main_menu_items[NUM_MAIN_MENU_ITEMS][2] = {
        {"I", "Audition Management"},
        {"II", "Training"},
        {"III", "Debut"}
    };
    const char* g_training_menu_items[NUM_TRAINING_STAGES][2] = {
        {"1", "Physical Strength & Knowledge"},
        {"2", "Self-Management & Teamwork"},
        {"3", "Language & Pronunciation"},
        {"4", "Vocal"},
        {"5", "Dance"},
        {"6", "Visual & Image"},
        {"7", "Acting & Stage Performance"},
        {"8", "Fan Communication"}
    };
#endif


// --- Utility Functions ---

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Initializes the status of all training stages to 'Not Started'.
 */
void initialize_status() {
    for (int i = 0; i < NUM_TRAINING_STAGES; i++) {
        g_stage_status[i] = 'N'; // 'N' for Not Started
    }
}


// --- Menu Display Functions ---

void display_main_menu() {
    clear_screen();
    printf("========================================\n");
    printf("        MAGRATHEA Main Menu\n");
    printf("========================================\n");

#ifdef BONUS_SINGLE_ARRAY
    for (int i = 0; i < g_total_menu_items; i++) {
        if (strcmp(g_menu_items[i][0], "0") == 0) { // Type '0' is Main Menu
            printf("   %s. %s\n", g_menu_items[i][1], g_menu_items[i][2]);
        }
    }
#else
    for (int i = 0; i < NUM_MAIN_MENU_ITEMS; i++) {
        printf("   %s. %s\n", g_main_menu_items[i][0], g_main_menu_items[i][1]);
    }
#endif

    printf("----------------------------------------\n");
    printf("Enter a menu number, or 'Q' to quit.\n");
    printf("Choice: ");
}

void display_training_menu() {
    clear_screen();
    printf("========================================\n");
    printf("         II. Training Menu\n");
    printf("========================================\n");

    int stages_3_to_8_locked = (g_stage_status[0] != 'P' || g_stage_status[1] != 'P');

#ifdef BONUS_SINGLE_ARRAY
    for (int i = 0; i < g_total_menu_items; i++) {
        if (strcmp(g_menu_items[i][0], "1") == 0) { // Type '1' is Training Menu
            int stage_idx = atoi(g_menu_items[i][1]) - 1;
            char status_char = g_stage_status[stage_idx];
            const char* status_str = (status_char == 'P') ? "[Passed]" : ((status_char == 'F') ? "[Failed]" : "");

            if (stages_3_to_8_locked && stage_idx >= 2) {
                 printf("   %s. %-30s [Locked]\n", g_menu_items[i][1], g_menu_items[i][2]);
            } else {
                 printf("   %s. %-30s %s\n", g_menu_items[i][1], g_menu_items[i][2], status_str);
            }
        }
    }
#else
    for (int i = 0; i < NUM_TRAINING_STAGES; i++) {
        char status_char = g_stage_status[i];
        const char* status_str = (status_char == 'P') ? "[Passed]" : ((status_char == 'F') ? "[Failed]" : "");

        if (stages_3_to_8_locked && i >= 2) {
            printf("   %s. %-30s [Locked]\n", g_training_menu_items[i][0], g_training_menu_items[i][1]);
        } else {
            printf("   %s. %-30s %s\n", g_training_menu_items[i][0], g_training_menu_items[i][1], status_str);
        }
    }
#endif

    printf("----------------------------------------\n");
    printf("Select a stage to evaluate, or '0' to return to the main menu.\n");
    printf("Choice: ");
}


// --- Logic Handling Functions ---

/**
 * @brief Handles the logic for a single training stage selection.
 * @param choice The 1-based stage number selected by the user.
 */
void handle_training_selection(int choice) {
    int stage_idx = choice - 1; // Convert to 0-based index

    // Rule: Check if stages 3-8 are locked
    if (stage_idx >= 2 && (g_stage_status[0] != 'P' || g_stage_status[1] != 'P')) {
        printf("\nError: You must pass stages 1 and 2 before accessing this stage.\n");
        return;
    }

    // Rule: Check if the stage has already been passed
    if (g_stage_status[stage_idx] == 'P') {
        printf("\nThis stage has already been passed and cannot be re-selected.\n");
        return;
    }

    char input_buf[10];
    printf("\nWould you like to enter the evaluation result for this stage? (Y/N): ");
    fgets(input_buf, sizeof(input_buf), stdin);

    if (toupper(input_buf[0]) == 'Y') {
        printf("Did you complete the training and pass the certification? (Y/N): ");
        fgets(input_buf, sizeof(input_buf), stdin);
        if (toupper(input_buf[0]) == 'Y') {
            g_stage_status[stage_idx] = 'P';
            printf("Status updated to [Passed].\n");
        } else {
            g_stage_status[stage_idx] = 'F';
            printf("Status updated to [Failed].\n");
        }
    } else {
        printf("Returning to training menu.\n");
    }
}

/**
 * @brief Manages the display and user interaction for the training sub-menu.
 */
void run_training_system() {
    // This function uses an infinite while loop for menu navigation.
    while (1) {
        display_training_menu();
        
        char input_buf[10];
        fgets(input_buf, sizeof(input_buf), stdin);
        
        // Return to main menu on '0'
        if (input_buf[0] == '0') {
            break;
        }
        
        int choice = atoi(input_buf);
        if (choice >= 1 && choice <= NUM_TRAINING_STAGES) {
            handle_training_selection(choice);
        } else {
            printf("\nInvalid choice. Please select a number from the menu.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Main Entry Point ---

int main(void) {
    initialize_status();

    // The main program loop. It's an infinite loop as required.
    while (1) {
        display_main_menu();

        char input_buf[10];
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strcspn(input_buf, "\n")] = 0; // Remove newline character

        // Termination conditions
        if (strlen(input_buf) == 0 || strcmp(input_buf, "0") == 0 || 
            toupper(input_buf[0]) == 'Q') {
            break;
        }

        // Menu navigation
        if (strcmp(input_buf, "II") == 0 || strcmp(input_buf, "2") == 0) {
            run_training_system();
        } else if (strcmp(input_buf, "I") == 0 || strcmp(input_buf, "1") == 0 ||
                   strcmp(input_buf, "III") == 0 || strcmp(input_buf, "3") == 0) {
            printf("\nThis feature is not yet implemented.\n");
            printf("Press Enter to continue...");
            getchar();
        } else {
            printf("\nInvalid selection. Please try again.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    }

    printf("\nExiting Magrathea System. Goodbye!\n");
    return 0;
}
