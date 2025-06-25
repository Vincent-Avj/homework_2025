/**
 * @file candidates.c
 * @brief Manages and displays data for K-POP audition candidates.
 *
 * This program simulates the data entry process for six candidates and then
 * displays their detailed information in a formatted table. It includes bonus
 * features for setting a group name and calculating the age of each candidate
 * based on their date of birth. The implementation strictly uses the ANSI C
 * standard and a limited set of standard libraries as required.
 */

#include <stdio.h>
#include <stdlib.h> // Required for atoi() and sscanf()

// --- Program-wide Constants ---
#define NUM_CANDIDATES 6
#define NUM_ATTRIBUTES 11

// For the age calculation bonus, we must use a fixed "current date" since
// the <time.h> library is not allowed by the problem constraints.
// Using the current date of this solution: June 2025.
#define CURRENT_YEAR 2025
#define CURRENT_MONTH 6
#define CURRENT_DAY 25

// --- Predefined Candidate Data ---
// In C, it's most practical to manage this kind of related data in parallel arrays
// when structs are not an option. This fulfills the "one-dimensional arrays" constraint.

const char* NAMES[] = {
    "Jiyeon Park", "Ethan Smith", "Suphanan Wong", "Helena Silva", "Karolina Nowak", "Liam Wilson"
};
const char* DOBS[] = {
    "2006/04/15", "2005/08/22", "2004/01/05", "2007/03/10", "2006/09/18", "2006/11/08"
};
const char* GENDERS[] = {"F", "M", "F", "F", "F", "M"};
const char* EMAILS[] = {
    "jiyeon@outlook.com", "ethan@outlook.com", "suphanan@outlook.com",
    "helena@outlook.com", "karolina@outlook.com", "liam@outlook.com"
};
const char* NATIONALITIES[] = {"South Korea", "USA", "Thailand", "Brazil", "Poland", "Australia"};
const char* BMIS[] = {"18.5", "21.2", "19.0", "20.8", "17.9", "20.1"};
const char* PRIMARY_SKILLS[] = {"Dance", "Vocal", "Dance", "Vocal", "Dance", "Dance"};
const char* SECONDARY_SKILLS[] = {
    "Composition", "Lyric Writing", "Lyric Writing",
    "Composition & Lyric Writing", "Composition", "Composition & Lyric Writing"
};
const char* TOPIK_LEVELS[] = {"0", "2", "4", "1", "2", "3"}; // "0" means Native
const char* MBTIS[] = {"ENFJ", "ISTP", "INFJ", "ENFP", "ISFP", "ENTJ"};
const char* INTRODUCTIONS[] = {
    "A passionate and creative dancer and composer ready to pursue dreams together.",
    "A vocalist with a rich voice and songwriting skills, eager to share K-POP's charm worldwide.",
    "Aspiring to move hearts with dynamic dance and emotional lyrics, becoming part of the Hallyu wave from Thailand.",
    "A versatile singer from Brazil who wants to inspire through both music and lyrics.",
    "A composer and dancer from Poland, eager to convey emotions through music.",
    "A passionate dancer and composer from Australia, determined to share his unique music through K-POP."
};

// This array holds the names of the attributes for printing the prompts.
const char* MEMBER_INFO[NUM_ATTRIBUTES] = {
    "Name", "Date of Birth (YYYY/MM/DD format)", "Gender (F for Female, M for Male)",
    "Email", "Nationality", "BMI", "Primary Skill", "Secondary Skill",
    "Korean Proficiency Level (TOPIK)", "MBTI", "Introduction"
};


// --- Helper Function for Bonus ---

/**
 * @brief  Calculates a candidate's current age based on their birthdate.
 * @param  dob_str The date of birth as a string in "YYYY/MM/DD" format.
 * @return The calculated age as an integer.
 */
int calculate_age(const char *dob_str) {
    int birth_year, birth_month, birth_day;

    // sscanf is in <stdio.h> and is great for parsing strings.
    sscanf(dob_str, "%d/%d/%d", &birth_year, &birth_month, &birth_day);

    int age = CURRENT_YEAR - birth_year;

    // If the person hasn't had their birthday yet this year, they are one year younger.
    if (CURRENT_MONTH < birth_month || (CURRENT_MONTH == birth_month && CURRENT_DAY < birth_day)) {
        age--;
    }
    return age;
}


// --- The Main Program ---

int main(void) {
    // --- Bonus: Get Group Name using getchar() ---
    char group_name[50];
    int c, i = 0;

    printf("####################################\n");
    printf("Please enter the audition group name: ");
    // The getchar() loop reads one character at a time until the user presses Enter.
    while ((c = getchar()) != '\n' && c != EOF && i < 49) {
        group_name[i++] = (char)c;
    }
    group_name[i] = '\0'; // Add the null terminator to make it a proper string.


    // --- Part 1: Simulate Data Entry (with a `while` loop as required) ---
    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Entry\n", group_name);
    printf("####################################\n");
    
    int candidate_idx = 0;
    const char* ordinals[] = {"first", "second", "third", "fourth", "fifth", "sixth"};
    while (candidate_idx < NUM_CANDIDATES) {
        printf("Entering information for the %s candidate.\n", ordinals[candidate_idx]);
        printf("---------------------------------\n");
        printf("1. %s: %s\n", MEMBER_INFO[0], NAMES[candidate_idx]);
        printf("2. %s: %s\n", MEMBER_INFO[1], DOBS[candidate_idx]);
        printf("3. %s: %s\n", MEMBER_INFO[2], GENDERS[candidate_idx]);
        printf("4. %s: %s\n", MEMBER_INFO[3], EMAILS[candidate_idx]);
        printf("5. %s: %s\n", MEMBER_INFO[4], NATIONALITIES[candidate_idx]);
        printf("6. %s: %s\n", MEMBER_INFO[5], BMIS[candidate_idx]);
        printf("7. %s: %s\n", MEMBER_INFO[6], PRIMARY_SKILLS[candidate_idx]);
        printf("8. %s: %s\n", MEMBER_INFO[7], SECONDARY_SKILLS[candidate_idx]);
        printf("9. %s: %s\n", MEMBER_INFO[8], TOPIK_LEVELS[candidate_idx]);
        printf("10. %s: %s\n", MEMBER_INFO[9], MBTIS[candidate_idx]);
        printf("11. %s: %s\n", MEMBER_INFO[10], INTRODUCTIONS[candidate_idx]);
        printf("=================================\n");
        candidate_idx++;
    }

    printf("\nAll candidate data has been entered. Press Enter to review.");
    getchar(); // Wait for user to continue


    // --- Part 2: Display All Candidate Data (with a `for` loop as required) ---
    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Review \n", group_name);
    printf("####################################\n");
    printf("============================================================================================================================================\n");
    printf("| %-22s | %-8s | %-6s | %-29s | %-12s | %-5s | %-15s | %-29s | %-6s | %-4s |\n", "Name (Age)", "DOB", "Gender", "Email", "Nationality", "BMI", "Primary Skill", "Secondary Skill", "TOPIK", "MBTI");
    printf("============================================================================================================================================\n");

    for (int j = 0; j < NUM_CANDIDATES; j++) {
        // --- Prepare data for printing ---
        
        // Bonus: Calculate age for display
        int age = calculate_age(DOBS[j]);
        char name_and_age[50];
        sprintf(name_and_age, "%s (%d)", NAMES[j], age);

        // Reformat DOB from YYYY/MM/DD to YYYYMMDD
        int y, m, d;
        sscanf(DOBS[j], "%d/%d/%d", &y, &m, &d);
        char dob_formatted[9];
        sprintf(dob_formatted, "%d%02d%02d", y, m, d);

        // Handle special case for TOPIK level
        // Use a ternary operator for a concise check
        const char* topik_display = (atoi(TOPIK_LEVELS[j]) == 0) ? "Native" : TOPIK_LEVELS[j];
        
        // --- Print the main row of data ---
        printf("| %-22s | %-8s | %-6s | %-29s | %-12s | %-5s | %-15s | %-29s | %-6s | %-4s |\n",
            name_and_age,
            dob_formatted,
            GENDERS[j],
            EMAILS[j],
            NATIONALITIES[j],
            BMIS[j],
            PRIMARY_SKILLS[j],
            SECONDARY_SKILLS[j],
            topik_display,
            MBTIS[j]
        );
        
        // --- Print the self-introduction on the next line ---
        printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| Introduction: %-128s |\n", INTRODUCTIONS[j]);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
    }

    return 0;
}
