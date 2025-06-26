/**
 * @file stage1.c
 * @brief The complete and integrated code for all "Stage 1" training modules.
 *
 * This version adds a comprehensive "Common Education" module. It allows for
 * inputting student grades, calculating weighted averages, and generating
 * various sorted and filtered reports. The bonus feature for obfuscating
 * student names and nicknames during display is also fully implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // For round()

// --- Constants and Global Data ---
#define NUM_MEMBERS 4
#define NUM_SUBJECTS 5

// --- Data Structures ---
typedef struct {
    char name[50];
    char nickname[50];
    int age;
    char gender;
    char grade_level[20];
} Student;

typedef struct {
    char nickname[50]; // Used to link to a Student
    int grades[NUM_SUBJECTS];
    float weighted_average;
} AcademicRecord;


// --- Global State Variables ---
Student g_students[NUM_MEMBERS];
AcademicRecord g_academic_records[NUM_MEMBERS];
int g_education_data_entered = 0;

// Static, predefined data
const char* SUBJECT_NAMES[NUM_SUBJECTS] = {"English", "Math", "Korean", "Social Studies", "Science"};
const float SUBJECT_WEIGHTS[NUM_SUBJECTS] = {0.30f, 0.25f, 0.20f, 0.15f, 0.10f};

// --- Forward Declarations for Menu System and Other Features ---
void clear_screen();
void show_physical_strength_menu();
// Dummy placeholders for functions from previous problems
void setMyDailyFood() { printf("\n(Placeholder for Meal Log)\n"); }
void mngMyDailyCalorie() { printf("\n(Placeholder for Calorie Management)\n"); }
void setMyWorkoutRoutine() { printf("\n(Placeholder for Set Workout Routine)\n"); }
void statsMyWorkoutRoutine() { printf("\n(Placeholder for Analyze Workout Routine)\n"); }


// --- Utility Functions ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

float round_to_one_decimal(float num) {
    return roundf(num * 10.0f) / 10.0f;
}

// --- Education Feature Sub-Functions ---

void initialize_student_data() {
    if (g_education_data_entered) return;
    const char* names[] = {"Jiyeon Park", "Ethan Smith", "Helena Silva", "Liam Wilson"};
    const char* nicknames[] = {"Ariel", "Simba", "Belle", "Aladdin"};
    const int ages[] = {19, 20, 18, 18}; // Ages for this context
    const char genders[] = {'F', 'M', 'F', 'M'};

    for(int i=0; i<NUM_MEMBERS; i++) {
        strcpy(g_students[i].name, names[i]);
        strcpy(g_students[i].nickname, nicknames[i]);
        g_students[i].age = ages[i];
        g_students[i].gender = genders[i];
        // Simple grade level calculation
        if(ages[i] >= 17) strcpy(g_students[i].grade_level, "High School");
        else strcpy(g_students[i].grade_level, "Middle School");
    }
}

/**
 * @brief Handles the user input for all students' grades.
 */
void inputCommonLearningInfo() {
    clear_screen();
    printf("========================================\n");
    printf("      K. Input Common Education Info\n");
    printf("========================================\n");

    initialize_student_data();
    char buffer[100];

    printf("Please enter grades (0-100) for the 5 mandatory subjects for each student.\n");
    for(int i=0; i<NUM_MEMBERS; i++) {
        printf("\n--- Entering grades for %s (%s) ---\n", g_students[i].name, g_students[i].nickname);
        strcpy(g_academic_records[i].nickname, g_students[i].nickname);
        
        float weighted_sum = 0;
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            printf("  - Grade for %s: ", SUBJECT_NAMES[j]);
            fgets(buffer, sizeof(buffer), stdin);
            int grade = atoi(buffer);
            g_academic_records[i].grades[j] = grade;
            weighted_sum += grade * SUBJECT_WEIGHTS[j];
        }
        g_academic_records[i].weighted_average = round_to_one_decimal(weighted_sum);
    }
    g_education_data_entered = 1;
    printf("\nAll academic data has been successfully recorded.\n");
}


// --- Report Generation and Sorting (Inquiry) Functions ---

// Comparators for qsort
int compare_nicknames_desc(const void* a, const void* b) { return strcmp(((Student*)b)->nickname, ((Student*)a)->nickname); }
int compare_avg_asc(const void* a, const void* b) {
    float diff = ((AcademicRecord*)a)->weighted_average - ((AcademicRecord*)b)->weighted_average;
    if (diff < 0) return -1; if (diff > 0) return 1; return 0;
}
// Global helper for the specific grade sort
int g_sort_subject_index = 0;
int compare_by_specific_grade_desc(const void* a, const void* b) {
    return ((AcademicRecord*)b)->grades[g_sort_subject_index] - ((AcademicRecord*)a)->grades[g_sort_subject_index];
}


// (Bonus) Obfuscation functions
void print_obfuscated_name(const char* name) {
    printf("  Name (ASCII): ");
    for(int i = 0; name[i] != '\0'; i++) printf("%d ", name[i]);
    printf("\n");
}
void print_obfuscated_nickname(const char* nickname) {
    printf("  Nickname (Reversed): ");
    for(int i = strlen(nickname) - 1; i >= 0; i--) printf("%c", nickname[i]);
    printf("\n");
}

void printStudentInfo() {
    Student sorted_students[NUM_MEMBERS];
    memcpy(sorted_students, g_students, sizeof(g_students));
    
    qsort(sorted_students, NUM_MEMBERS, sizeof(Student), compare_nicknames_desc);
    
    printf("\n--- Student Info (Sorted by Nickname Desc, Obfuscated) ---\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("--------------------------------\n");
        print_obfuscated_name(sorted_students[i].name);
        print_obfuscated_nickname(sorted_students[i].nickname);
        printf("  Age: %d, Gender: %c, Grade Level: %s\n", sorted_students[i].age, sorted_students[i].gender, sorted_students[i].grade_level);
    }
}

void printAscendAvgGrade() {
    AcademicRecord sorted_records[NUM_MEMBERS];
    memcpy(sorted_records, g_academic_records, sizeof(g_academic_records));
    
    qsort(sorted_records, NUM_MEMBERS, sizeof(AcademicRecord), compare_avg_asc);

    printf("\n--- Academic Records (Sorted by Average Grade Ascending) ---\n");
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("%-10s (Avg: %4.1f) | Eng:%3d, Mth:%3d, Kor:%3d, Soc:%3d, Sci:%3d\n",
            sorted_records[i].nickname, sorted_records[i].weighted_average,
            sorted_records[i].grades[0], sorted_records[i].grades[1], sorted_records[i].grades[2],
            sorted_records[i].grades[3], sorted_records[i].grades[4]);
    }
}

void printDescendHighWeightGrade() {
    // Find the subject with the highest weight first
    int high_weight_idx = 0;
    for(int i=1; i<NUM_SUBJECTS; i++){
        if(SUBJECT_WEIGHTS[i] > SUBJECT_WEIGHTS[high_weight_idx]) high_weight_idx = i;
    }
    printf("\n--- Records Sorted by Highest Weight Subject: %s (Descending) ---\n", SUBJECT_NAMES[high_weight_idx]);
    
    g_sort_subject_index = high_weight_idx; // Set global for comparator
    AcademicRecord sorted_records[NUM_MEMBERS];
    memcpy(sorted_records, g_academic_records, sizeof(g_academic_records));
    qsort(sorted_records, NUM_MEMBERS, sizeof(AcademicRecord), compare_by_specific_grade_desc);
    
    for (int i = 0; i < NUM_MEMBERS; i++) {
        printf("%-10s | Score in %s: %d\n", sorted_records[i].nickname, SUBJECT_NAMES[high_weight_idx], sorted_records[i].grades[high_weight_idx]);
    }
}

void printExcludeMaxMinGrade() {
    printf("\n--- Grade Report (Excluding Highest and Lowest Score) ---\n");
    printf("Select a subject to analyze (1-5):\n");
    for(int i=0; i<NUM_SUBJECTS; i++) printf("  %d. %s\n", i+1, SUBJECT_NAMES[i]);
    
    char buffer[10];
    printf("Choice: ");
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if(choice < 1 || choice > NUM_SUBJECTS) { printf("Invalid choice.\n"); return; }
    int subj_idx = choice - 1;

    int min_score = 101, max_score = -1;
    int min_idx = -1, max_idx = -1;

    // Find the min and max scores and their indices
    for(int i=0; i<NUM_MEMBERS; i++) {
        int score = g_academic_records[i].grades[subj_idx];
        if (score < min_score) { min_score = score; min_idx = i; }
        if (score > max_score) { max_score = score; max_idx = i; }
    }
    
    printf("\nScores for %s, excluding highest (%d) and lowest (%d):\n", SUBJECT_NAMES[subj_idx], max_score, min_score);
    for(int i=0; i<NUM_MEMBERS; i++) {
        if(i == min_idx || i == max_idx) continue;
        printf("  - %s: %d\n", g_academic_records[i].nickname, g_academic_records[i].grades[subj_idx]);
    }
}

/**
 * @brief The main inquiry hub, providing a sub-menu for various reports.
 */
void inquiryCommonLearningInfo() {
    if (!g_education_data_entered) {
        printf("\nNo education data exists. Please use option 'K' first.\n");
        return;
    }
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("========================================\n");
        printf("      L. Inquire Common Education Results\n");
        printf("========================================\n");
        printf("   1. View Student Info (Sorted by Nickname, Obfuscated)\n");
        printf("   2. View Grades (Sorted by Average, Ascending)\n");
        printf("   3. View Grades (Sorted by Highest Weight Subject)\n");
        printf("   4. View Grades (Excluding Min/Max)\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        fgets(choice_buf, sizeof(choice_buf), stdin);
        int choice = atoi(choice_buf);

        if(choice == 0) break;

        switch(choice) {
            case 1: printStudentInfo(); break;
            case 2: printAscendAvgGrade(); break;
            case 3: printDescendHighWeightGrade(); break;
            case 4: printExcludeMaxMinGrade(); break;
            default: printf("Invalid option.\n"); break;
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// --- Menu System ---
const char* HEALTH_MENU_ITEMS[] = {
    "A-H (Previous Features)", "I. Enter Workout Routine", "J. Analyze Workout Routine",
    "K. Input Common Education Info", "L. Inquire Common Education Results"
};
const int NUM_HEALTH_MENU_ITEMS = sizeof(HEALTH_MENU_ITEMS) / sizeof(char*);

void show_physical_strength_menu() {
    char choice_buf[10];
    while(1) {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: 1. Physical Strength & Knowledge\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < NUM_HEALTH_MENU_ITEMS; i++) printf("   %s\n", HEALTH_MENU_ITEMS[i]);
        printf("   0. Back to Main Menu\n");
        printf("----------------------------------------\n");
        printf("Choice: ");
        
        fgets(choice_buf, sizeof(choice_buf), stdin);
        char choice = toupper(choice_buf[0]);

        if (choice == '0') break;

        switch(choice) {
            case 'I': statsMyWorkoutRoutine(); break;
            case 'J': statsMyWorkoutRoutine(); break;
            case 'K': inputCommonLearningInfo(); break;
            case 'L': inquiryCommonLearningInfo(); break;
            default: printf("\n(This is a placeholder for a previous feature.)\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


int main(void) {
    // Directly go to the relevant menu for this problem
    show_physical_strength_menu();
    return 0;
}
