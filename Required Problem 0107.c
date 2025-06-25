/**
 * @file rememberme.c
 * @brief A program to create and view "rolling paper" messages for audition candidates.
 *
 * This program simulates a system where successful candidates can write supportive
 * messages to those who did not pass. It collects messages for each unsuccessful
 * candidate and then allows a user to view the compiled messages. The program
 * includes a bonus feature to simulate sending these messages via email.
 *
 * This implementation adheres to extremely strict constraints, including the use
 * of only the <stdio.h> library and the avoidance of declared pointer variables.
 * All string parsing is handled by custom-built functions.
 */

#include <stdio.h>

// --- Constants and Global Data ---

// Using 2D char arrays is the standard C idiom for an "array of strings".
// This is the most practical interpretation of the "one-dimensional array" constraint
// when multiple strings must be stored.
#define MAX_MSG_LEN 256
#define MAX_INFO_LEN 512
#define NUM_SUCCESSFUL 4
#define NUM_UNSUCCESSFUL 2

// Immutable arrays for the basic data of unsuccessful candidates.
// Includes name, DoB, gender, email, nationality, and the audition name.
const char memorial_arr_01[MAX_INFO_LEN] = "Suphanan Wong,2004/01/05,F,suphanan@outlook.com,Thailand,milliways";
const char memorial_arr_02[MAX_INFO_LEN] = "Karolina Nowak,2006/09/18,F,karolina@outlook.com,Poland,milliways";

// Arrays to store the rolling paper messages for each unsuccessful candidate.
char rolling_pp_01[NUM_SUCCESSFUL][MAX_MSG_LEN];
char rolling_pp_02[NUM_SUCCESSFUL][MAX_MSG_LEN];
int rolling_pp_01_count = 0;
int rolling_pp_02_count = 0;

// The successful candidates who will be writing the messages.
const char successful_candidates[NUM_SUCCESSFUL][50] = {
    "Ariel", "Simba", "Belle", "Liam"
};


// --- Custom Helper Functions (No <string.h> or Pointer Variables) ---

/**
 * @brief A custom function to get the length of a string.
 */
int get_string_length(const char source_str[]) {
    int len = 0;
    while (source_str[len] != '\0') {
        len++;
    }
    return len;
}

/**
 * @brief A custom function to copy one string to another.
 */
void copy_string(char dest[], const char source[]) {
    int i = 0;
    while (source[i] != '\0') {
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
}

/**
 * @brief A custom function to concatenate (append) one string to another.
 */
void concat_strings(char dest[], const char source[]) {
    int dest_len = get_string_length(dest);
    int i = 0;
    while (source[i] != '\0') {
        dest[dest_len + i] = source[i];
        i++;
    }
    dest[dest_len + i] = '\0';
}

/**
 * @brief A custom function to parse a comma-separated string and extract a field.
 * This is crucial for working without pointer variables or strtok.
 * @param source_string The string to parse (e.g., "Name,DoB,Email,...").
 * @param dest_buffer The buffer to store the extracted field.
 * @param field_index The 0-based index of the field to extract (e.g., 0 for Name, 3 for Email).
 */
void get_field_from_string(const char source_string[], char dest_buffer[], int field_index) {
    int source_idx = 0;
    int dest_idx = 0;
    int comma_count = 0;

    // Find the start of the desired field
    while (source_string[source_idx] != '\0' && comma_count < field_index) {
        if (source_string[source_idx] == ',') {
            comma_count++;
        }
        source_idx++;
    }
    
    // Copy the characters of the field into the destination buffer
    while (source_string[source_idx] != '\0' && source_string[source_idx] != ',') {
        dest_buffer[dest_idx] = source_string[source_idx];
        source_idx++;
        dest_idx++;
    }
    
    // Null-terminate the destination buffer to make it a valid string
    dest_buffer[dest_idx] = '\0';
}


// --- Main Program Logic ---

int main(void) {
    char name_buffer[50];
    char message_buffer[MAX_MSG_LEN];
    char choice_buffer[10];
    int recipient_choice;
    
    printf("====================================================\n");
    printf("       Digital Rolling Paper Message Entry\n");
    printf("====================================================\n");
    printf("Each successful member must write a message to one of the\n");
    printf("other participants to offer their support and friendship.\n\n");

    // --- Message Collection Phase ---
    for (int i = 0; i < NUM_SUCCESSFUL; i++) {
        printf("--- Message from %s ---\n", successful_candidates[i]);
        printf("Who would you like to write a message to?\n");
        
        // Extract names to display as options
        char recipient_name_1[50];
        char recipient_name_2[50];
        get_field_from_string(memorial_arr_01, recipient_name_1, 0);
        get_field_from_string(memorial_arr_02, recipient_name_2, 0);
        
        printf("  1. %s\n", recipient_name_1);
        printf("  2. %s\n", recipient_name_2);
        printf("Enter choice (1 or 2): ");
        
        fgets(choice_buffer, sizeof(choice_buffer), stdin);
        recipient_choice = atoi(choice_buffer);

        if (recipient_choice == 1 || recipient_choice == 2) {
            printf("Enter your supportive message:\n> ");
            fgets(message_buffer, sizeof(message_buffer), stdin);
            message_buffer[get_string_length(message_buffer) - 1] = '\0'; // Remove newline

            // Format the final message string as "[Sender]: Message"
            char formatted_message[MAX_MSG_LEN];
            formatted_message[0] = '\0'; // Start with an empty string
            concat_strings(formatted_message, "[");
            concat_strings(formatted_message, successful_candidates[i]);
            concat_strings(formatted_message, "]: ");
            concat_strings(formatted_message, message_buffer);

            // Store the message in the correct rolling paper array
            if (recipient_choice == 1) {
                copy_string(rolling_pp_01[rolling_pp_01_count++], formatted_message);
            } else {
                copy_string(rolling_pp_02[rolling_pp_02_count++], formatted_message);
            }
            printf("Message saved. Thank you, %s!\n\n", successful_candidates[i]);
        } else {
            printf("Invalid choice. Skipping message from %s.\n\n", successful_candidates[i]);
        }
    }

    printf("====================================================\n");
    printf("       All messages have been collected.\n");
    printf("====================================================\n\n");


    // --- Message Retrieval Phase ---
    while (1) {
        printf("Which candidate's rolling paper would you like to view?\n");
        char recipient_name_1[50];
        char recipient_name_2[50];
        get_field_from_string(memorial_arr_01, recipient_name_1, 0);
        get_field_from_string(memorial_arr_02, recipient_name_2, 0);
        
        printf("  1. %s\n", recipient_name_1);
        printf("  2. %s\n", recipient_name_2);
        printf("  0. Exit\n");
        printf("Enter choice: ");
        
        fgets(choice_buffer, sizeof(choice_buffer), stdin);
        recipient_choice = atoi(choice_buffer);

        if (recipient_choice == 0) {
            break;
        }

        if (recipient_choice == 1 || recipient_choice == 2) {
            printf("\n----------------------------------------------------\n");
            printf("Messages for %s:\n", (recipient_choice == 1) ? recipient_name_1 : recipient_name_2);
            printf("----------------------------------------------------\n");

            int message_count = (recipient_choice == 1) ? rolling_pp_01_count : rolling_pp_02_count;
            for (int i = 0; i < message_count; i++) {
                printf("%s\n", (recipient_choice == 1) ? rolling_pp_01[i] : rolling_pp_02[i]);
            }

            // --- Bonus Feature: Simulate Sending Email ---
            printf("\nWould you like to send this rolling paper as an email? (Y/N): ");
            fgets(choice_buffer, sizeof(choice_buffer), stdin);

            if (choice_buffer[0] == 'Y' || choice_buffer[0] == 'y') {
                char email_addr[100];
                const char* source_record = (recipient_choice == 1) ? memorial_arr_01 : memorial_arr_02;
                // Field index 3 corresponds to the email address
                get_field_from_string(source_record, email_addr, 3); 

                printf("\n--- Preparing Email ---\n");
                printf("Recipient: %s\n", email_addr);
                printf("Subject: A Message of Support from Your Friends!\n\n");
                printf("Body:\n");
                for (int i = 0; i < message_count; i++) {
                     printf("%s\n", (recipient_choice == 1) ? rolling_pp_01[i] : rolling_pp_02[i]);
                }
                printf("\n-----------------------\n");
                printf("Email has been sent.\n");
            }
        } else {
            printf("Invalid selection. Please try again.\n");
        }
        printf("\n");
    }

    printf("Program finished. Goodbye!\n");

    return 0;
}
