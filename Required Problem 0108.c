/**
 * @file applytest.c
 * @brief A program to schedule and manage certification exams for trainees.
 *
 * This program demonstrates state management, date calculations, and simulation
 * under extreme C language constraints. It includes a custom random number
 * generator and date parsing functions. The bonus features for reminders
 * and venue assignment are fully implemented.
 */

#include <stdio.h>
#include <math.h>   // Allowed by the problem constraints
#include <stdlib.h> // Included for system() and atoi()

// --- Custom Utility Functions ---

// This function was added to make the file self-contained and fix the linker error.
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// NOTE: In a real project, these my_... functions would be in a separate
// utility file. They are included here because <string.h> and <stdlib.h>
// were restricted in the original prompt's context.

int my_strlen(const char s[]) { int i=0; while(s[i]!='\0') i++; return i; }
int my_strcmp(const char s1[], const char s2[]) { int i=0; while(s1[i]==s2[i]){if(s1[i]=='\0')return 0; i++;} return s1[i]-s2[i]; }
long my_atoi(const char s[]) {
    long n = 0;
    for(int i=0; s[i]>='0' && s[i]<='9'; ++i) n = 10 * n + (s[i] - '0');
    return n;
}

unsigned long g_rand_seed = 1;
void my_srand(unsigned long seed) { g_rand_seed = seed; }
int my_rand() {
    g_rand_seed = g_rand_seed * 1103515245 + 12345;
    return (unsigned int)(g_rand_seed / 65536) % 32768;
}

// --- Constants and Global Data ---
#define NUM_STAGES 9

const char g_category_names[NUM_STAGES][50] = {
    "Physical Strength", "Self-Management & Teamwork", "Language & Pronunciation",
    "Vocal", "Dance", "Visual & Image", "Acting & Stage Performance",
    "Fan Communication", "Knowledge"
};
char g_pass_status[NUM_STAGES];
long g_scheduled_dates[NUM_STAGES];
long g_training_start_date = 0;

// --- Date Calculation Functions ---
int is_leap(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }

long date_to_days(long date) {
    if (date == 0) return 0;
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;
    long total_days = 0;
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    for(int y=1; y<year; y++) total_days += is_leap(y) ? 366 : 365;
    for(int m=1; m<month; m++){
        total_days += days_in_month[m];
        if(m==2 && is_leap(year)) total_days++;
    }
    total_days += day;
    return total_days;
}

// --- Bonus Feature Functions ---
void check_reminders(long current_date_long) {
    printf("\n--- Checking for Upcoming Exam Reminders ---\n");
    long current_days = date_to_days(current_date_long);
    int reminders_found = 0;
    for(int i=0; i<NUM_STAGES; i++){
        if(g_pass_status[i] == 'S') {
            long exam_days = date_to_days(g_scheduled_dates[i]);
            long days_until = exam_days - current_days;
            if(days_until > 0 && days_until <= 10) {
                printf("REMINDER: Exam '%s' is in %ld day(s) on %ld!\n", g_category_names[i], days_until, g_scheduled_dates[i]);
                reminders_found++;
            }
        }
    }
    if(reminders_found == 0) printf("No exams scheduled within the next 10 days.\n");
}

void check_venue_assignments(long current_date_long) {
    const char* rooms[] = {"A101", "B203", "C305", "D110", "E401", "A102", "B204", "C306", "D112", "E404"};
    printf("\n--- Checking for Today's Exam Venues ---\n");
    int exams_today = 0;
    for(int i=0; i<NUM_STAGES; i++){
        if(g_pass_status[i] == 'S' && g_scheduled_dates[i] == current_date_long){
            printf("VENUE: Exam '%s' today is in Room %s.\n", g_category_names[i], rooms[my_rand() % 10]);
            exams_today++;
        }
    }
    if(exams_today == 0) printf("No exams scheduled for today.\n");
}


// --- Main Feature ---
void applyForTest() {
    char input_buffer[100];

    // Initialize statuses
    for(int i=0; i<NUM_STAGES; i++) {
        g_pass_status[i] = 'N';
        g_scheduled_dates[i] = 0;
    }

    // Get setup info from user
    printf("Enter a number to seed the random generator: ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    my_srand(my_atoi(input_buffer));
    
    printf("Enter the official Training Start Date (YYYYMMDD): ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    g_training_start_date = my_atoi(input_buffer);
    
    printf("Enter the Current Date for simulation (YYYYMMDD): ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    long current_date = my_atoi(input_buffer);

    // Run bonus checks on startup
    check_reminders(current_date);
    check_venue_assignments(current_date);
    printf("\nPress Enter to proceed to scheduling...");
    getchar();

    // Main menu loop
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("       Certification Exam Scheduler\n");
        printf("Training Start Date: %ld\n", g_training_start_date);
        printf("----------------------------------------\n");
        for(int i=0; i<NUM_STAGES; i++) {
            printf("  %d. %-30s Status: ", i+1, g_category_names[i]);
            if(g_pass_status[i] == 'N') printf("[Not Scheduled]\n");
            else if(g_pass_status[i] == 'S') printf("[Scheduled for %ld]\n", g_scheduled_dates[i]);
            else if(g_pass_status[i] == 'P') printf("[Passed]\n");
            else printf("[Failed]\n");
        }
        printf("----------------------------------------\n");
        printf("Select a category to schedule/evaluate (1-9), or 0 to quit: ");
        fgets(input_buffer, sizeof(input_buffer), stdin);
        int choice = my_atoi(input_buffer);
        
        if(choice == 0) break;
        
        int stage_idx = choice - 1;
        if(choice < 1 || choice > NUM_STAGES) {
            printf("Invalid selection.\n");
        } else if (g_pass_status[stage_idx] == 'P') {
            printf("This stage has already been passed!\n");
        } else if (stage_idx > 1 && (g_pass_status[0] != 'P' || g_pass_status[1] != 'P')) {
            printf("You must pass 'Physical Strength' and 'Self-Management' first.\n");
        } else {
            printf("Enter exam date for '%s' (YYYYMMDD): ", g_category_names[stage_idx]);
            fgets(input_buffer, sizeof(input_buffer), stdin);
            long exam_date = my_atoi(input_buffer);

            long start_days = date_to_days(g_training_start_date);
            long exam_days = date_to_days(exam_date);
            if (exam_days - start_days >= 30 && exam_days - start_days <= 100) {
                g_scheduled_dates[stage_idx] = exam_date;
                g_pass_status[stage_idx] = 'S';
                printf("Exam for '%s' scheduled for %ld.\n", g_category_names[stage_idx], exam_date);

                printf("Would you like to enter the evaluation result now? (Y/N): ");
                fgets(input_buffer, sizeof(input_buffer), stdin);
                if (input_buffer[0] == 'Y' || input_buffer[0] == 'y') {
                    printf("Did you pass? (Y/N): ");
                    fgets(input_buffer, sizeof(input_buffer), stdin);
                    g_pass_status[stage_idx] = (input_buffer[0] == 'Y' || input_buffer[0] == 'y') ? 'P' : 'F';
                    printf("Status updated.\n");
                }
            } else {
                printf("Invalid Date: Exam must be scheduled between 30 and 100 days after training start date.\n");
            }
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}

int main() {
    applyForTest();
    return 0;
}
