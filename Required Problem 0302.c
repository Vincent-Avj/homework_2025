/**
 * @file stage2.c
 * @brief A module for Self-Management and Teamwork training.
 *
 * This version adds a "Trauma Management" feature. This allows for recording
 * member traumas and then conducting a guided counseling session. The session
 * logic includes a bonus feature to pause and resume, allowing progress to be
 * saved and continued later.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---

#define NUM_MEMBERS 4
#define NUM_COUNSELING_QUESTIONS 5
#define SESSION_QUESTIONS 3
#define MAX_RESPONSE_LEN 100

// From Problem 1, for reference
const char* milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"}, {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"}, {"Liam Wilson", "Aladdin"}
};
const int MEMBER_AGES[] = {19, 20, 18, 18}; // Ages for June 2025

// --- Data Structures for Trauma Management ---

typedef struct {
    char nickname[50];
    int age;
    char trauma_description[512];
    int is_set; // Flag to check if trauma has been entered
} TraumaInfo;

typedef struct {
    int id;
    char content[256];
} CounselingQuestion;

// (Bonus) Structure to hold the state of a single counseling session
typedef struct {
    char nickname[50];
    int questions_to_ask[SESSION_QUESTIONS]; // Stores the IDs of the 3 random questions
    char responses[SESSION_QUESTIONS][MAX_RESPONSE_LEN + 1];
    int num_answered; // Key to pausing and resuming (0, 1, 2, or 3)
} CounselingSessionState;


// --- Global State Variables ---
TraumaInfo g_trauma_info[NUM_MEMBERS];
CounselingSessionState g_session_states[NUM_MEMBERS];
int g_trauma_system_initialized = 0;

const CounselingQuestion g_counseling_questions[NUM_COUNSELING_QUESTIONS] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

// --- Forward Declarations & Dummy Implementations ---
void clear_screen();
void matchMentoring(); // From previous problem


// --- Utility and Core Logic Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Finds a member's index by their nickname.
 * @return The index if found, otherwise -1.
 */
int find_member_by_nickname(const char* nickname) {
    for (int i = 0; i < NUM_MEMBERS; i++) {
        if (strcmp(nickname, milliways_members[i][1]) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Initializes all trauma-related data structures for each member.
 */
void initialize_trauma_system() {
    if (g_trauma_system_initialized) return;

    for (int i = 0; i < NUM_MEMBERS; i++) {
        strcpy(g_trauma_info[i].nickname, milliways_members[i][1]);
        g_trauma_info[i].age = MEMBER_AGES[i];
        g_trauma_info[i].trauma_description[0] = '\0'; // Empty string
        g_trauma_info[i].is_set = 0;
        
        // Initialize session state for pause/resume
        strcpy(g_session_states[i].nickname, milliways_members[i][1]);
        g_session_states[i].num_answered = 0;
    }
    g_trauma_system_initialized = 1;
}

/**
 * @brief Handles the user interface for inputting trauma details.
 */
void input_trauma_data() {
    char nickname_buf[50];
    while(1) {
        printf("\nEnter nickname to input trauma data for (or 'done' to exit): ");
        fgets(nickname_buf, sizeof(nickname_buf), stdin);
        nickname_buf[strcspn(nickname_buf, "\n")] = 0;

        if (strcmp(nickname_buf, "done") == 0) break;

        int member_idx = find_member_by_nickname(nickname_buf);
        
        // Using a ternary operator as required
        printf(" -> Member lookup status: %s\n", (member_idx != -1) ? "FOUND" : "NOT FOUND");

        if (member_idx != -1) {
            printf("Enter trauma description for %s:\n> ", nickname_buf);
            fgets(g_trauma_info[member_idx].trauma_description, 
                  sizeof(g_trauma_info[0].trauma_description), stdin);
            g_trauma_info[member_idx].trauma_description[strcspn(g_trauma_info[member_idx].trauma_description, "\n")] = 0;
            g_trauma_info[member_idx].is_set = 1;
            printf("Trauma data for %s saved.\n", nickname_buf);
        } else {
            printf("Please enter a valid nickname from the member list.\n");
        }
    }
}

/**
 * @brief Manages a single counseling session, with pause/resume functionality.
 */
void run_counseling_session() {
    printf("\n--- Start or Resume Counseling Session ---\n");
    printf("Select a member who has trauma data entered:\n");
    
    int available_members[NUM_MEMBERS];
    int available_count = 0;
    for(int i=0; i<NUM_MEMBERS; i++) {
        if (g_trauma_info[i].is_set) {
            printf("  %d. %s (%d/3 questions answered)\n", available_count + 1, g_trauma_info[i].nickname, g_session_states[i].num_answered);
            available_members[available_count] = i; // Store original index
            available_count++;
        }
    }

    if (available_count == 0) {
        printf("No members have trauma data entered yet.\n");
        return;
    }
    
    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    
    if (choice < 1 || choice > available_count) {
        printf("Invalid selection.\n");
        return;
    }
    int member_idx = available_members[choice-1];
    CounselingSessionState* session = &g_session_states[member_idx];
    
    // If this is a new session, select 3 random questions
    if (session->num_answered == 0) {
        int q_indices[] = {0, 1, 2, 3, 4};
        for(int i=4; i>0; i--) { int j = rand()%(i+1); int tmp=q_indices[i]; q_indices[i]=q_indices[j]; q_indices[j]=tmp; }
        for(int i=0; i<SESSION_QUESTIONS; i++) session->questions_to_ask[i] = q_indices[i];
    }
    
    printf("\n--- Counseling for %s ---\n", session->nickname);
    printf("Type your answer (1-100 chars). Type 'pause' to save progress and exit.\n");

    // This loop automatically picks up where the session left off
    for (int i = session->num_answered; i < SESSION_QUESTIONS; i++) {
        int question_idx = session->questions_to_ask[i];
        printf("\nQ: %s\n", g_counseling_questions[question_idx].content);
        
        do {
            printf("> ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            
            if (strcmp(buffer, "pause") == 0) {
                printf("Session paused. Your progress is saved.\n");
                return; // Exit the function, progress is saved in the global struct
            }
            
            int len = strlen(buffer);
            if (len > 0 && len <= MAX_RESPONSE_LEN) {
                strcpy(session->responses[i], buffer);
                session->num_answered++;
                break; // Valid answer, move to next question
            } else {
                 printf("Response must be between 1 and %d characters. Please try again.\n", MAX_RESPONSE_LEN);
            }
        } while(1);
    }
    
    if(session->num_answered == SESSION_QUESTIONS) {
        printf("\n--- Counseling session complete for %s! ---\n", session->nickname);
    }
}

void view_counseling_results() {
    printf("\n--- View Completed Counseling Sessions ---\n");
    printf("Select a member to view their session details:\n");

    int completed_members[NUM_MEMBERS];
    int completed_count = 0;
    for(int i=0; i<NUM_MEMBERS; i++){
        if(g_session_states[i].num_answered == SESSION_QUESTIONS) {
            printf("  %d. %s\n", completed_count + 1, g_session_states[i].nickname);
            completed_members[completed_count++] = i;
        }
    }
    if (completed_count == 0) {
        printf("No sessions have been completed yet.\n");
        return;
    }

    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if(choice < 1 || choice > completed_count) {
        printf("Invalid selection.\n");
        return;
    }
    int member_idx = completed_members[choice-1];

    printf("\n--- Session Report for %s ---\n", g_trauma_info[member_idx].nickname);
    printf("Trauma: %s\n", g_trauma_info[member_idx].trauma_description);
    printf("--------------------------------\n");
    for(int i=0; i<SESSION_QUESTIONS; i++) {
        int q_idx = g_session_states[member_idx].questions_to_ask[i];
        printf("Q: %s\n", g_counseling_questions[q_idx].content);
        printf("A: %s\n\n", g_session_states[member_idx].responses[i]);
    }
}


/**
 * @brief The main function for the Trauma Management feature.
 */
void overcomeTrauma() {
    initialize_trauma_system();
    
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("      B. Trauma Management\n");
        printf("========================================\n");
        printf("   A. Input Trauma Data\n");
        printf("   B. Start/Resume Counseling Session\n");
        printf("   C. View Completed Session Report\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        char choice = toupper(choice_buf[0]);

        if (choice == '0') break;

        switch(choice) {
            case 'A': input_trauma_data(); break;
            case 'B': run_counseling_session(); break;
            case 'C': view_counseling_results(); break;
            default: printf("\nInvalid choice.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Menu System Integration ---

void show_self_mgmt_menu() {
    char choice;
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 2. Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   A. Mentoring\n");
        printf("   B. Trauma Management\n");
        printf("   0. Back to Training Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        choice = getchar();
        while(getchar() != '\n');

        if (choice == '0') break;

        switch(toupper(choice)) {
            case 'A':
                // matchMentoring();
                printf("\n'Mentoring' is from the previous problem.\n");
                break;
            case 'B':
                overcomeTrauma();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Main Entry Point and Dummy Functions ---
void run_training_system() { show_self_mgmt_menu(); }
int main(void) {
    srand(time(NULL));
    run_training_system();
    return 0;
}
