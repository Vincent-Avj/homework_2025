/**
 * @file milliways.c
 * @brief A program to process audition candidates, filter for successful members,
 * and compile a complete profile with newly "interviewed" data.
 *
 * This program demonstrates data filtering, string parsing, and calculation
 * under strict C library constraints. It includes a bonus feature to classify
 * and display a member's BMI status using a switch statement. All string
 * operations are handled by custom-built functions.
 */

#include <stdio.h>
#include <stdlib.h> // For atof()

// --- Custom String Helper Functions (as <string.h> is disallowed) ---

int my_strlen(const char* s) { int i=0; while(s[i]!='\0') i++; return i; }
void my_strcpy(char* dest, const char* src) { int i=0; while(src[i]!='\0'){ dest[i]=src[i]; i++; } dest[i]='\0'; }
int my_strncmp(const char* s1, const char* s2, int n) {
    for(int i=0; i<n; i++) {
        if(s1[i] != s2[i]) return s1[i] - s2[i];
        if(s1[i] == '\0') return 0;
    }
    return 0;
}
// A simple version of strstr
const char* my_strstr(const char* haystack, const char* needle) {
    int n_len = my_strlen(needle);
    if(n_len == 0) return haystack;
    for(int i=0; haystack[i] != '\0'; i++) {
        if(my_strncmp(&haystack[i], needle, n_len) == 0) {
            return &haystack[i];
        }
    }
    return NULL;
}


// --- Data Structures and Constants ---
#define NUM_INITIAL_CANDIDATES 6
#define NUM_FINAL_MEMBERS 4

typedef struct {
    char name[50], nickname[50], dob[11], gender[2], email[50], nationality[50];
    double bmi, height_m, weight_kg;
    char blood_type[3], allergies[50], hobbies[100], sns[100], education[50];
} MilliwaysMember;


// --- Initial Data Sources ---
// Data for the 6 initial candidates in a "JSON-like" string format
const char* candidate_db[NUM_INITIAL_CANDIDATES] = {
    "{ \"id\": \"CD-001\", \"name\": \"Jiyeon Park\", \"dob\": \"20060415\", \"gender\": \"F\", \"bmi\": \"18.5\", \"passed\": true }",
    "{ \"id\": \"CD-002\", \"name\": \"Ethan Smith\", \"dob\": \"20050822\", \"gender\": \"M\", \"bmi\": \"21.2\", \"passed\": true }",
    "{ \"id\": \"CD-003\", \"name\": \"Suphanan Wong\", \"dob\": \"20040105\", \"gender\": \"F\", \"bmi\": \"19.0\", \"passed\": false }",
    "{ \"id\": \"CD-004\", \"name\": \"Helena Silva\", \"dob\": \"20070310\", \"gender\": \"F\", \"bmi\": \"20.8\", \"passed\": true }",
    "{ \"id\": \"CD-005\", \"name\": \"Karolina Nowak\", \"dob\": \"20060918\", \"gender\": \"F\", \"bmi\": \"17.9\", \"passed\": false }",
    "{ \"id\": \"CD-006\", \"name\": \"Liam Wilson\", \"dob\": \"20061108\", \"gender\": \"M\", \"bmi\": \"20.1\", \"passed\": true }"
};

// New "interview" data for the 4 successful members
const char* NICKNAMES[] = {"Ariel", "Simba", "Belle", "Aladdin"};
const char* EDUCATIONS[] = {"High School Dropout", "Middle School Dropout", "Middle School Graduate", "Middle School Dropout"};
const double HEIGHTS_CM[] = {168.0, 178.0, 163.0, 175.0};
const char* BLOOD_TYPES[] = {"A", "O", "B", "AB"};
const char* ALLERGIES[] = {"Dairy", "Peanuts", "Fish", "Shellfish"};
const char* HOBBIES[] = {"Dance practice, composing", "Songwriting, weight training", "Singing, drawing", "Dancing, music producing"};
const char* SNS_ACCOUNTS[] = {"Instagram - @Ariel_Jiyeon", "Twitter - @Simba_Ethan", "Instagram - @Belle_Helena", "Instagram - @Aladdin_Liam"};


// --- Helper Functions ---

/**
 * @brief A custom parser to extract a value from our simple JSON-like string.
 * @param json_str The string to parse (e.g., "{ "name": "Ariel", ... }").
 * @param key The key to find (e.g., "\"name\"").
 * @param value_buf The buffer where the found value will be stored.
 */
void parse_json_value(const char* json_str, const char* key, char* value_buf) {
    const char* key_ptr = my_strstr(json_str, key);
    if (!key_ptr) { value_buf[0] = '\0'; return; }

    const char* value_start = key_ptr + my_strlen(key);
    // Move past " : "
    while(*value_start == ' ' || *value_start == ':' || *value_start == '"') {
        value_start++;
    }

    int i = 0;
    while(value_start[i] != '\0' && value_start[i] != '"') {
        value_buf[i] = value_start[i];
        i++;
    }
    value_buf[i] = '\0';
}

/**
 * @brief (Bonus) Takes a BMI value and prints it with a classification.
 * This function uses a switch statement as required.
 * @param bmi The BMI value to classify.
 */
void print_bmi_with_status(double bmi) {
    const char* status;
    // Cast bmi to an int to use it in the switch statement
    switch ((int)bmi) {
        case 0 ... 17:
            status = "Underweight";
            break;
        case 18:
            status = (bmi < 18.5) ? "Underweight" : "Normal";
            break;
        case 19 ... 24:
            status = "Normal";
            break;
        case 25 ... 29:
            status = "Overweight";
            break;
        default:
            status = "Obese";
            break;
    }
    printf("  BMI: %.1f (%s)\n", bmi, status);
}


// --- Main Program Logic ---

int main(void) {
    MilliwaysMember final_members[NUM_FINAL_MEMBERS];
    int final_member_count = 0;
    
    printf("####################################\n");
    printf("   Processing Audition Results...\n");
    printf("####################################\n");

    // This for loop simulates the "interview" process for passed members.
    for (int i = 0; i < NUM_INITIAL_CANDIDATES; i++) {
        // Check if the candidate's record contains the "passed: true" string
        if (my_strstr(candidate_db[i], "\"passed\": true") != NULL) {
            if (final_member_count >= NUM_FINAL_MEMBERS) continue; // Safety check
            
            MilliwaysMember* p_member = &final_members[final_member_count];
            char temp_buf[100];
            
            // 1. Copy existing basic info by parsing the JSON-like string
            parse_json_value(candidate_db[i], "\"name\"", p_member->name);
            parse_json_value(candidate_db[i], "\"dob\"", p_member->dob);
            parse_json_value(candidate_db[i], "\"gender\"", p_member->gender);
            
            parse_json_value(candidate_db[i], "\"bmi\"", temp_buf);
            p_member->bmi = atof(temp_buf); // Convert string to double

            // 2. Add new, detailed info for the final members
            my_strcpy(p_member->nickname, NICKNAMES[final_member_count]);
            my_strcpy(p_member->education, EDUCATIONS[final_member_count]);
            p_member->height_m = HEIGHTS_CM[final_member_count] / 100.0;
            my_strcpy(p_member->blood_type, BLOOD_TYPES[final_member_count]);
            my_strcpy(p_member->allergies, ALLERGIES[final_member_count]);
            my_strcpy(p_member->hobbies, HOBBIES[final_member_count]);
            my_strcpy(p_member->sns, SNS_ACCOUNTS[final_member_count]);
            
            // 3. Calculate weight from BMI and height
            p_member->weight_kg = p_member->bmi * p_member->height_m * p_member->height_m;
            
            final_member_count++;
        }
    }

    printf("\n--- Final Milliways Member Profiles ---\n");
    
    // Display the final, combined data for each of the 4 members
    for (int i = 0; i < final_member_count; i++) {
        MilliwaysMember* p = &final_members[i];
        printf("\n=================================\n");
        printf("  Member %d: %s\n", i + 1, p->name);
        printf("---------------------------------\n");
        printf("  Nickname: %s\n", p->nickname);
        printf("  Date of Birth: %s\n", p->dob);
        printf("  Gender: %s\n", p->gender);
        printf("  Education: %s\n", p->education);
        printf("  Height: %.2f m\n", p->height_m);
        printf("  Calculated Weight: %.1f kg\n", p->weight_kg);
        
        // Bonus: Call the function that uses a switch statement
        print_bmi_with_status(p->bmi);

        printf("  Blood Type: %s\n", p->blood_type);
        printf("  Allergies: %s\n", p->allergies);
        printf("  Hobbies: %s\n", p->hobbies);
        printf("  SNS: %s\n", p->sns);
        printf("=================================\n");
    }

    return 0;
}
