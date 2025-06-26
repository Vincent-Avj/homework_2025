/**
 * @file stage2.c
 * @brief A module for Self-Management & Teamwork, including the "Record Journey" feature.
 *
 * This version adds the "Record Journey" (Memorial Park) module. It allows
 * members to log significant memories. The feature is time-locked and uses a
 * dynamic array for storage. It includes a bonus "cheat key" for testing.
 * All string operations are performed with custom-built functions to adhere
 * to the strict "no <string.h>" constraint.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Data Structures ---
#define NUM_MEMBERS 4
#define MAX_JOURNEY_LOGS 100 // Initial capacity, can grow
#define CONTENT_MIN_LEN 30
#define CONTENT_MAX_LEN 70

// Structure for a single journey journal entry
typedef struct {
    char member_nickname[50];
    char content[256];
    char moment_date[9]; // "YYYYMMDD"
    int wow_effect;      // 0-10
} JourneyEntry;


// --- Global Data (Dynamic Array) ---
JourneyEntry* g_journey_log = NULL;
int g_journey_log_count = 0;
int g_journey_log_capacity = 0;

const char* g_milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"}, {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"}, {"Liam Wilson", "Aladdin"}
};


// --- Custom String Helper Functions (No <string.h>) ---
int my_strlen(const char* s) { int i=0; while(s[i]!='\0') i++; return i; }
void my_strcpy(char* dest, const char* src) { int i=0; while(src[i]!='\0'){ dest[i]=src[i]; i++; } dest[i]='\0'; }
int my_strcmp(const char* s1, const char* s2) { while(*s1 && (*s1==*s2)){s1++;s2++;} return *(unsigned char*)s1 - *(unsigned char*)s2; }
void my_str_toupper(char* s) { for(int i=0; s[i]!='\0'; i++) s[i] = toupper(s[i]); }
void my_strncpy(char* dest, const char* src, int n) { int i; for(i=0; i<n && src[i]!='\0'; i++) dest[i]=src[i]; dest[i]='\0'; }
int my_atoi(const char* s) { int res=0; for(int i=0; s[i]!='\0'; ++i) res = res*10+s[i]-'0'; return res; }


// --- Utility and Core Logic Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

const char* get_wow_effect_string(int effect) {
    if (effect <= 3) return "Calm";
    if (effect <= 6) return "Exciting";
    return "Meaningful";
}

/**
 * @brief Adds a new journey entry to the global dynamic array, resizing if necessary.
 */
void add_journey_to_db(JourneyEntry entry) {
    if (g_journey_log_count == g_journey_log_capacity) {
        g_journey_log_capacity = (g_journey_log_capacity == 0) ? 10 : g_journey_log_capacity * 2;
        JourneyEntry* new_log = (JourneyEntry*)realloc(g_journey_log, g_journey_log_capacity * sizeof(JourneyEntry));
        if (!new_log) {
            perror("Failed to expand journey log");
            return;
        }
        g_journey_log = new_log;
    }
    g_journey_log[g_journey_log_count++] = entry;
}

void displayMemory(const char* nickname) {
    char full_name[50] = "Unknown";
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(my_strcmp(nickname, g_milliways_members[i][1])==0) {
            my_strcpy(full_name, g_milliways_members[i][0]);
            break;
        }
    }

    printf("\n--- Journey Records for %s (%s) ---\n", full_name, nickname);
    int records_found = 0;
    for(int i=0; i<g_journey_log_count; i++) {
        if(my_strcmp(g_journey_log[i].member_nickname, nickname) == 0) {
            records_found++;
            char y[5], m[3], d[3];
            my_strncpy(y, g_journey_log[i].moment_date, 4);
            my_strncpy(m, g_journey_log[i].moment_date + 4, 2);
            my_strncpy(d, g_journey_log[i].moment_date + 6, 2);

            printf("\nRecord %d:\n", records_found);
            printf("  Content: %s\n", g_journey_log[i].content);
            printf("  Impressive Moment: %s년 %s월 %s일 - %s\n", y, m, d, get_wow_effect_string(g_journey_log[i].wow_effect));
        }
    }
    if (records_found == 0) printf("No records found for this member.\n");
}

void findImpressiveMoment(const char* nickname) {
    char wow_str[20];
    printf("Enter Wow Effect to find (Calm, Exciting, Meaningful): ");
    fgets(wow_str, sizeof(wow_str), stdin);
    wow_str[my_strlen(wow_str)-1] = '\0';
    
    printf("\n--- Finding '%s' moments for %s ---\n", wow_str, nickname);
    int found_count = 0;
    for(int i=0; i<g_journey_log_count; i++) {
        if(my_strcmp(g_journey_log[i].member_nickname, nickname) == 0) {
            if(my_strcmp(get_wow_effect_string(g_journey_log[i].wow_effect), wow_str) == 0) {
                found_count++;
                char y[5], m[3], d[3];
                my_strncpy(y, g_journey_log[i].moment_date, 4); my_strncpy(m, g_journey_log[i].moment_date + 4, 2); my_strncpy(d, g_journey_log[i].moment_date + 6, 2);
                printf("  - On %s년 %s월 %s일: \"%s\"\n", y, m, d, g_journey_log[i].content);
            }
        }
    }
    if(found_count == 0) printf("No matching moments found.\n");
}

/**
 * @brief Handles the main interaction loop for the Memorial Park feature.
 */
void createMemory(int member_idx) {
    const char* nickname = g_milliways_members[member_idx][1];
    char choice_buf[10];
    int invalid_count = 0;

    while(1) {
        printf("\n--- %s's Memorial Park ---\n", nickname);
        printf("Would you like to record your journey? (y/n): ");
        fgets(choice_buf, sizeof(choice_buf), stdin);
        choice_buf[my_strlen(choice_buf)-1] = '\0';
        my_str_toupper(choice_buf);

        if(my_strcmp(choice_buf, "Y") == 0) {
            invalid_count = 0;
            JourneyEntry new_entry;
            my_strcpy(new_entry.member_nickname, nickname);
            
            do {
                printf("Enter content for your journey (30-70 chars): ");
                fgets(new_entry.content, sizeof(new_entry.content), stdin);
                new_entry.content[my_strlen(new_entry.content)-1] = '\0';
                int len = my_strlen(new_entry.content);
                if(len >= CONTENT_MIN_LEN && len <= CONTENT_MAX_LEN) break;
                printf("Error: Content length must be between %d and %d.\n", CONTENT_MIN_LEN, CONTENT_MAX_LEN);
            } while(1);

            printf("Enter date of impressive moment (YYYYMMDD): ");
            fgets(new_entry.moment_date, sizeof(new_entry.moment_date), stdin);
            new_entry.moment_date[my_strlen(new_entry.moment_date)-1] = '\0';

            do {
                printf("Enter 'Wow Effect' score (0-10): ");
                fgets(choice_buf, sizeof(choice_buf), stdin);
                new_entry.wow_effect = my_atoi(choice_buf);
                if(new_entry.wow_effect >= 0 && new_entry.wow_effect <= 10) break;
                printf("Error: Score must be between 0 and 10.\n");
            } while(1);
            
            add_journey_to_db(new_entry);
            printf("--- Journey successfully recorded! ---\n");

        } else if(my_strcmp(choice_buf, "N") == 0) {
            printf("\nSelect an option:\n  1. Display my journey records\n  2. Find my impressive moments\n  3. Return to previous menu\nChoice: ");
            fgets(choice_buf, sizeof(choice_buf), stdin);
            if(choice_buf[0] == '1') displayMemory(nickname);
            else if(choice_buf[0] == '2') findImpressiveMoment(nickname);
            else if(choice_buf[0] == '3') break;
            printf("\nPress Enter to continue..."); getchar();

        } else {
            invalid_count++;
            printf("Invalid input. Please enter 'y' or 'n'. (%d/2 attempts)\n", invalid_count);
            if(invalid_count >= 2) { printf("Too many invalid attempts. Returning.\n"); break; }
        }
    }
}


/**
 * @brief The main entry point for the "Record Journey" feature.
 */
void buildMemorialPark() {
    clear_screen();
    printf("========================================\n");
    printf("         E. Record Journey (Memorial Park)\n");
    printf("========================================\n");
    
    int current_hour;
    char buffer[20];

    // Bonus: Cheat Key Logic
    printf("Enter cheat key (T/t) to set time, or press Enter for current time: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[my_strlen(buffer)-1] = '\0';
    my_str_toupper(buffer);

    if (my_strcmp(buffer, "T") == 0) {
        printf("Cheat activated. Enter simulated time (YYYYMMDDHH): ");
        fgets(buffer, sizeof(buffer), stdin);
        int len = my_strlen(buffer);
        if (len >= 2) { // Ensure there are hour digits
            char hour_str[] = {buffer[len-3], buffer[len-2], '\0'};
            current_hour = my_atoi(hour_str);
        } else { current_hour = -1; } // Invalid cheat input
        printf("Using simulated hour: %d\n", current_hour);
    } else {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        current_hour = t->tm_hour;
        printf("Using current system hour: %d\n", current_hour);
    }
    
    // Time-Lock Check
    if (current_hour < 18 || current_hour > 22) {
        printf("\nThis feature is only available from 18:00 to 22:00.\n");
        printf("Please try again during the allowed hours.\n");
        printf("\nPress Enter to return..."); getchar(); return;
    }
    printf("\nWelcome to the Memorial Park. Access granted.\n");

    // Login
    printf("\nSelect your nickname:\n");
    for(int i=0; i<NUM_MEMBERS; i++) printf("  %d. %s\n", i+1, g_milliways_members[i][1]);
    int choice = 0;
    do {
        printf("Choice (1-4): ");
        fgets(buffer, sizeof(buffer), stdin);
        choice = my_atoi(buffer);
    } while (choice < 1 || choice > NUM_MEMBERS);
    
    printf("Welcome, %s!\n", g_milliways_members[choice-1][0]);
    createMemory(choice - 1);
}

// --- Cleanup and Menu System ---
void cleanup_stage2_data() {
    if(g_journey_log != NULL) { free(g_journey_log); g_journey_log = NULL; }
}

// Dummy for other feature
void completeMission() {}
void overcomeTrauma() {}

void show_self_mgmt_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   C. Teamwork Mission\n");
        printf("   D. Trauma Management\n");
        printf("   E. Record Journey\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');

        switch(toupper(choice)) {
            case 'C': completeMission(); break;
            case 'D': overcomeTrauma(); break;
            case 'E': buildMemorialPark(); break;
        }
    } while (choice != '0');
}

int main() {
    atexit(cleanup_stage2_data);
    srand(time(NULL));
    show_self_mgmt_menu();
    return 0;
}
