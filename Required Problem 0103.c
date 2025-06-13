#include <stdio.h>
#include <stdlib.h>

#define NUM_CANDIDATES 6
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_NATIONALITY_LEN 50
#define MAX_SKILL_LEN 50
#define MAX_MBTI_LEN 5
#define MAX_INTRO_LEN 200
#define MAX_GROUP_NAME_LEN 50

// Function to calculate age based on a birthdate string (YYYY/MM/DD)
// Note: As <time.h> is disallowed, the current date is hardcoded to the time of writing.
int calculate_age(const char* dob_str) {
    int birth_year, birth_month, birth_day;
    int current_year = 2024;
    int current_month = 5;
    int current_day = 21;
    int age;

    // Parse the birthdate string
    sscanf(dob_str, "%d/%d/%d", &birth_year, &birth_month, &birth_day);

    age = current_year - birth_year;

    // Adjust age if the birthday hasn't occurred yet this year
    if (current_month < birth_month || (current_month == birth_month && current_day < birth_day)) {
        age--;
    }

    return age;
}


int main(void) {
    // Parallel arrays to store candidate data as per constraints.
    // This structure is used instead of a more optimal array of structs
    // to adhere to the implementation task's specific constraints.
    char names[NUM_CANDIDATES][MAX_NAME_LEN];
    char dobs[NUM_CANDIDATES][11]; // YYYY/MM/DD + null terminator
    char genders[NUM_CANDIDATES][2];
    char emails[NUM_CANDIDATES][MAX_EMAIL_LEN];
    char nationalities[NUM_CANDIDATES][MAX_NATIONALITY_LEN];
    char bmis[NUM_CANDIDATES][10];
    char primary_skills[NUM_CANDIDATES][MAX_SKILL_LEN];
    char secondary_skills[NUM_CANDIDATES][MAX_SKILL_LEN];
    char topik_levels[NUM_CANDIDATES][3];
    char mbtis[NUM_CANDIDATES][MAX_MBTI_LEN];
    char introductions[NUM_CANDIDATES][MAX_INTRO_LEN];

    // (Bonus Feature) Use getchar() to read the group name
    char group_name[MAX_GROUP_NAME_LEN];
    char ch;
    int i = 0;
    
    printf("Enter the audition group name: ");
    while ((ch = getchar()) != '\n' && i < MAX_GROUP_NAME_LEN - 1) {
        group_name[i++] = ch;
    }
    group_name[i] = '\0'; // Null-terminate the string

    // --- Data Entry Phase ---
    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Entry\n", group_name);
    printf("####################################\n");

    int candidate_index = 0;
    while (candidate_index < NUM_CANDIDATES) {
        printf("Entering information for candidate %d.\n", candidate_index + 1);
        printf("---------------------------------\n");

        printf("%d. Name: ", 1);
        scanf(" %[^\n]", names[candidate_index]);

        printf("%d. Date of Birth (YYYY/MM/DD format): ", 2);
        scanf("%10s", dobs[candidate_index]);

        printf("%d. Gender (F for Female, M for Male): ", 3);
        scanf("%1s", genders[candidate_index]);

        printf("%d. Email: ", 4);
        scanf("%s", emails[candidate_index]);

        printf("%d. Nationality: ", 5);
        scanf("%s", nationalities[candidate_index]);

        printf("%d. BMI: ", 6);
        scanf("%s", bmis[candidate_index]);

        printf("%d. Primary Skill: ", 7);
        scanf(" %[^\n]", primary_skills[candidate_index]);

        printf("%d. Secondary Skill: ", 8);
        scanf(" %[^\n]", secondary_skills[candidate_index]);

        printf("%d. Korean Proficiency Level (TOPIK): ", 9);
        scanf("%s", topik_levels[candidate_index]);

        printf("%d. MBTI: ", 10);
        scanf("%s", mbtis[candidate_index]);

        printf("%d. Introduction: ", 11);
        scanf(" %[^\n]", introductions[candidate_index]);
        
        printf("=================================\n");
        candidate_index++;
    }

    // --- Data Review Phase ---
    printf("\n####################################\n");
    printf("     [%s] Audition Candidate Data Review\n", group_name);
    printf("####################################\n");
    printf("===================================================================================================================================================\n");
    printf("%-20s | %-10s | %-6s | %-21s | %-12s | %-4s | %-13s | %-25s | %-6s | %-4s |\n",
           "Name", "DOB", "Gender", "Email", "Nationality", "BMI", "Primary Skill", "Secondary Skill", "TOPIK", "MBTI");
    printf("===================================================================================================================================================\n");

    for (i = 0; i < NUM_CANDIDATES; ++i) {
        // (Bonus Feature) Calculate age
        int age = calculate_age(dobs[i]);
        char name_with_age[MAX_NAME_LEN + 5];
        sprintf(name_with_age, "%s (%d)", names[i], age);

        // Format DOB without slashes
        char formatted_dob[9];
        sscanf(dobs[i], "%4s/%2s/%2s", formatted_dob, formatted_dob + 4, formatted_dob + 6);
        
        // Display "Native" for TOPIK level 0
        const char* topik_display = (atoi(topik_levels[i]) == 0) ? "Native" : topik_levels[i];

        printf("%-20s | %-10s | %-6s | %-21s | %-12s | %-4s | %-13s | %-25s | %-6s | %-4s |\n",
               name_with_age,
               formatted_dob,
               genders[i],
               emails[i],
               nationalities[i],
               bmis[i],
               primary_skills[i],
               secondary_skills[i],
               topik_display,
               mbtis[i]);
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%s\n", introductions[i]);
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }

    return 0;
}
