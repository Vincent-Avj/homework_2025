/**
 * @file main.c
 * @brief A self-contained version of the "Self-Management & Teamwork" module for online compilers.
 *
 * This final version adds a customizable "Dashboard" that aggregates data from all other
 * features in this module. It includes a fallback mechanism to use hardcoded dummy
 * data for all features, ensuring it can run in any environment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define MAX_LOG_ENTRIES 5 

// --- Structs for All Features in this Module ---

// FIX: Added missing struct definitions for the dummy data features.
typedef struct {
    char member_nickname[50];
    char mentor_name[50];
    char mentor_id[10];
} MentoringRecord;

typedef struct {
    char member_nickname[50];
    char trauma_type[256];
    char counseling_content[512];
} TraumaRecord;

typedef struct {
    char nickname[50];
    char date[11];
    char question[100];
    char answer[256];
} ReflectionRecord;

typedef struct {
    char nickname[50];
    char date[9]; // YYYYMMDD
    char content[256];
    int wow_effect;
} JourneyRecord;

typedef struct {
    char nickname[50];
    char question[100];
    char answer[256];
} EmotionDiaryRecord;

// The key new structure for the Dashboard configuration
typedef struct {
    char member_nickname[50];
    char selected_ids[10]; // e.g., "ADEF"
    int is_configured;
} MyDashboard;


// --- Global Data Storage ---
const char* g_milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"}, {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"}, {"Liam Wilson", "Aladdin"}
};

// Data for each feature will be stored here
MentoringRecord g_mentoring_log[NUM_MEMBERS];
TraumaRecord g_trauma_log[NUM_MEMBERS];
ReflectionRecord g_reflection_log[NUM_MEMBERS];
JourneyRecord g_journey_log[NUM_MEMBERS];
EmotionDiaryRecord g_diary_log[NUM_MEMBERS];
MyDashboard g_dashboard_configs[NUM_MEMBERS];


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_self_mgmt_menu();
void showDashboard();


// --- Custom String Helper Functions (No <string.h>) ---
int my_strlen(const char* s) { int i=0; while(s[i]!='\0') i++; return i; }
void my_strcpy(char* dest, const char* src) { int i=0; while(src[i]!='\0'){ dest[i]=src[i]; i++; } dest[i]='\0'; }
int my_strcmp(const char* s1, const char* s2) { while(*s1 && (*s1==*s2)){s1++;s2++;} return *(unsigned char*)s1 - *(unsigned char*)s2; }


// --- Main Entry Point ---
int main() {
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
        printf("   2. Self-Management & Teamwork\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        if (choice == '2') {
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
        printf("   G. Dashboard\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        if (toupper(choice) == 'G') {
            showDashboard();
        } else if (choice != '0') {
             printf("\n[This feature is a placeholder in this file.]\n");
             printf("Press Enter to continue...");
             getchar();
        }
    } while (choice != '0');
}


// --- Dashboard Feature Functions ---

// Initialize all data with dummy values so the dashboard has something to show.
void initialize_all_dummy_data() {
    static int initialized = 0;
    if (initialized) return;
    
    for(int i=0; i<NUM_MEMBERS; i++) {
        const char* nickname = g_milliways_members[i][1];
        my_strcpy(g_mentoring_log[i].member_nickname, nickname); my_strcpy(g_mentoring_log[i].mentor_name, "SeniorDev"); my_strcpy(g_mentoring_log[i].mentor_id, "SD-01");
        my_strcpy(g_trauma_log[i].member_nickname, nickname); my_strcpy(g_trauma_log[i].trauma_type, "Performance Anxiety"); my_strcpy(g_trauma_log[i].counseling_content, "Practiced mindfulness techniques.");
        my_strcpy(g_reflection_log[i].nickname, nickname); my_strcpy(g_reflection_log[i].date, "2025-06-26"); my_strcpy(g_reflection_log[i].question, "What did I achieve today?"); my_strcpy(g_reflection_log[i].answer, "I successfully hit the high note in the chorus.");
        my_strcpy(g_journey_log[i].nickname, nickname); my_strcpy(g_journey_log[i].date, "20250520"); my_strcpy(g_journey_log[i].content, "Our first time recording as a full group was unforgettable."); g_journey_log[i].wow_effect = 9;
        my_strcpy(g_diary_log[i].nickname, nickname); my_strcpy(g_diary_log[i].question, "How are you feeling right now?"); my_strcpy(g_diary_log[i].answer, "A little nervous but mostly excited for the debut.");
        g_dashboard_configs[i].is_configured = 0; my_strcpy(g_dashboard_configs[i].member_nickname, nickname);
    }
    initialized = 1;
}

// --- Data Printing "Widget" Functions ---
void print_mentoring_data(const char* nickname) {
    printf("\n---------- [A] Mentoring ----------\n");
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(my_strcmp(g_mentoring_log[i].member_nickname, nickname) == 0) {
            printf("  Mentor ID: %s\n  Mentor Name: %s\n", g_mentoring_log[i].mentor_id, g_mentoring_log[i].mentor_name);
            return;
        }
    }
}
void print_trauma_data(const char* nickname) {
    printf("\n---------- [B] Trauma Management ----------\n");
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(my_strcmp(g_trauma_log[i].member_nickname, nickname) == 0) {
            printf("  Trauma Type: %s\n  Counseling Note: %s\n", g_trauma_log[i].trauma_type, g_trauma_log[i].counseling_content);
            return;
        }
    }
}
void print_reflection_data(const char* nickname) {
    printf("\n---------- [D] Self-Reflection ----------\n");
     for(int i=0; i<NUM_MEMBERS; i++) {
        if(my_strcmp(g_reflection_log[i].nickname, nickname) == 0) {
            printf("  Date: %s\n  Q: %s\n  A: %s\n", g_reflection_log[i].date, g_reflection_log[i].question, g_reflection_log[i].answer);
            return;
        }
    }
}
void print_journey_data(const char* nickname) {
    printf("\n---------- [E] Record Journey ----------\n");
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(my_strcmp(g_journey_log[i].nickname, nickname) == 0) {
            printf("  Date: %s\n  Wow Effect: %d\n  Content: %s\n", g_journey_log[i].date, g_journey_log[i].wow_effect, g_journey_log[i].content);
            return;
        }
    }
}
void print_emotion_diary_data(const char* nickname) {
     printf("\n---------- [F] Emotional Diary ----------\n");
     for(int i=0; i<NUM_MEMBERS; i++) {
         if(my_strcmp(g_diary_log[i].nickname, nickname) == 0) {
             printf("  Q: %s\n  A: %s\n", g_diary_log[i].question, g_diary_log[i].answer);
             return;
         }
     }
}

/**
 * @brief (Bonus) The central hub function that dispatches print calls.
 */
void getDataHub(const MyDashboard* config) {
    printf("\n========================================\n");
    printf("        %s's Dashboard\n", config->member_nickname);
    
    for (int i = 0; i < my_strlen(config->selected_ids); i++) {
        char id = toupper(config->selected_ids[i]);
        switch (id) {
            case 'A': print_mentoring_data(config->member_nickname); break;
            case 'B': print_trauma_data(config->member_nickname); break;
            case 'D': print_reflection_data(config->member_nickname); break;
            case 'E': print_journey_data(config->member_nickname); break;
            case 'F': print_emotion_diary_data(config->member_nickname); break;
        }
    }
    printf("========================================\n");
}

/**
 * @brief Allows a user to select which modules appear on their dashboard.
 */
void selectSubMenu(int member_idx) {
    const char* sub_menus[][2] = {
        {"A", "Mentoring"}, {"B", "Trauma Management"}, {"D", "Self-Reflection"},
        {"E", "Record Journey"}, {"F", "Emotional Diary"}
    };
    int num_sub_menus = 5;

    printf("\n--- Edit Dashboard for %s ---\n", g_milliways_members[member_idx][1]);
    printf("Available modules:\n");
    for (int i = 0; i < num_sub_menus; i++) {
        printf("  %s. %s\n", sub_menus[i][0], sub_menus[i][1]);
    }
    
    char selection[20];
    printf("\nEnter the letters of the modules to display (e.g., ADEF): ");
    fgets(selection, sizeof(selection), stdin);
    selection[my_strlen(selection)-1] = '\0';

    my_strcpy(g_dashboard_configs[member_idx].selected_ids, selection);
    g_dashboard_configs[member_idx].is_configured = 1;
    printf("Dashboard updated successfully!\n");
}


/**
 * @brief The main function for the Dashboard feature.
 */
void showDashboard() {
    initialize_all_dummy_data();
    
    char buffer[10];
    printf("\n--- Dashboard ---\nSelect your nickname:\n");
    for(int i=0; i<NUM_MEMBERS; i++) printf("  %d. %s\n", i+1, g_milliways_members[i][1]);
    
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if(choice < 1 || choice > NUM_MEMBERS) { printf("Invalid choice.\n"); return; }
    int member_idx = choice - 1;
    
    MyDashboard* p_config = &g_dashboard_configs[member_idx];
    
    while(1) {
        clear_screen();
        printf("--- Dashboard Menu for %s ---\n", p_config->member_nickname);
        printf("1. Edit Dashboard Layout\n");
        printf("2. Display My Dashboard %s\n", p_config->is_configured ? "" : "(Please configure first)");
        printf("0. Back\n");
        printf("Choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        if(buffer[0] == '0') break;

        if (buffer[0] == '1') {
            selectSubMenu(member_idx);
        } else if (buffer[0] == '2') {
            if (p_config->is_configured) {
                getDataHub(p_config);
            } else {
                printf("\nDashboard not configured. Please use 'Edit Dashboard' first.\n");
            }
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}
