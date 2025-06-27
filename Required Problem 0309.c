/**
 * @file main.c
 * @brief A self-contained version of the "Manito" (Secret Santa) game for online compilers.
 *
 * This program combines all necessary functions into a single file. It implements
 * a game where a secret message is encoded using a from-scratch Base64
 * algorithm and then decoded by the recipient. The OpenSSL bonus feature has
 * been removed to ensure compatibility with online compilers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constants and Global Data ---
#define NUM_MEMBERS 4

const char* g_milliways_members[NUM_MEMBERS][2] = {
    {"Jiyeon Park", "Ariel"},
    {"Ethan Smith", "Simba"},
    {"Helena Silva", "Belle"},
    {"Liam Wilson", "Aladdin"}
};


// --- Forward Declarations for All Functions ---
void clear_screen();
void show_main_menu();
void show_training_menu();
void show_self_mgmt_menu();
void playManito();
char* base64_encode(const unsigned char *data, size_t input_length);
unsigned char* base64_decode(const char *data, size_t *output_length);


// --- From-Scratch Base64 Implementation ---
const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64_encode(const unsigned char *data, size_t input_length) {
    // Calculate the length of the output string
    size_t output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {
        unsigned int octet_a = i < input_length ? data[i++] : 0;
        unsigned int octet_b = i < input_length ? data[i++] : 0;
        unsigned int octet_c = i < input_length ? data[i++] : 0;
        unsigned int triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = b64_chars[(triple >> 18) & 0x3F];
        encoded_data[j++] = b64_chars[(triple >> 12) & 0x3F];
        encoded_data[j++] = b64_chars[(triple >> 6) & 0x3F];
        encoded_data[j++] = b64_chars[triple & 0x3F];
    }

    // Apply padding
    int mod_table[] = {0, 2, 1};
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';

    encoded_data[output_length] = '\0';
    return encoded_data;
}

unsigned char* base64_decode(const char *data, size_t *output_length) {
    size_t input_length = strlen(data);
    if (input_length % 4 != 0) return NULL;

    *output_length = (input_length / 4) * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length + 1);
    if (decoded_data == NULL) return NULL;

    char decoding_table[256];
    for (int i = 0; i < 64; i++) decoding_table[(unsigned char) b64_chars[i]] = i;
    
    for (int i = 0, j = 0; i < input_length;) {
        unsigned int sextet_a = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        unsigned int sextet_b = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        unsigned int sextet_c = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        unsigned int sextet_d = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        unsigned int triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

        if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;
    }

    decoded_data[*output_length] = '\0';
    return decoded_data;
}


// --- Main Game Logic ---
void playManito() {
    clear_screen();
    printf("========================================\n");
    printf("         H. Manito (Secret Santa) Game\n");
    printf("========================================\n");
    printf("Using Custom Base64 Implementation.\n");
    
    // 1. Select Manito and input gift
    printf("\nWho is the Manito (secret giver)?\n");
    for(int i=0; i<NUM_MEMBERS; i++) printf("  %d. %s\n", i+1, g_milliways_members[i][1]);
    int manito_choice = 0;
    char buffer[50];
    do {
        printf("Choice (1-4): ");
        fgets(buffer, sizeof(buffer), stdin);
        manito_choice = atoi(buffer);
    } while(manito_choice < 1 || manito_choice > 4);
    
    int manito_idx = manito_choice - 1;
    const char* manito_nickname = g_milliways_members[manito_idx][1];
    
    char gift_buf[100];
    printf("Enter the gift from %s: ", manito_nickname);
    fgets(gift_buf, sizeof(gift_buf), stdin);
    gift_buf[strcspn(gift_buf, "\n")] = 0;

    // 2. Encode the information
    char combined_str[200];
    sprintf(combined_str, "%s:%s", manito_nickname, gift_buf);
    
    char* encoded_str = base64_encode((const unsigned char*)combined_str, strlen(combined_str));
    if(!encoded_str) { printf("Encoding failed.\n"); return; }

    printf("\n--- Encoding Data ---\n");
    printf("Original Data: %s\n", combined_str);
    printf("Base64 Encoded: %s\n", encoded_str);
    
    // 3. Guessing Game for Recipient
    char guess_buf[50];
    while(1) {
        int recipient_idx;
        do {
            recipient_idx = rand() % NUM_MEMBERS;
        } while (recipient_idx == manito_idx);
        const char* recipient_nickname = g_milliways_members[recipient_idx][1];

        printf("\nA recipient has been drawn! Who do you think it is?: ");
        fgets(guess_buf, sizeof(guess_buf), stdin);
        guess_buf[strcspn(guess_buf, "\n")] = 0;
        
        if (strcmp(guess_buf, recipient_nickname) == 0) {
            printf("\nCorrect! The recipient is indeed %s.\n", recipient_nickname);
            printf("Please give this encoded message to %s.\n", recipient_nickname);
            break;
        } else {
            printf("Incorrect. The draw will be held again!\n");
        }
    }
    
    // 4. Decode the information
    printf("\n--- Decoding Message ---\n");
    printf("Recipient, please enter the encoded message you received:\n> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    
    size_t decoded_len;
    unsigned char* decoded_str = base64_decode(buffer, &decoded_len);

    if (decoded_str) {
        printf("\n--- Decoded Message ---\n");
        printf("Message: \"%s\"\n", decoded_str);
        free(decoded_str);
    } else {
        printf("Decoding failed. Incorrect Base64 string entered.\n");
    }

    free(encoded_str);
    printf("\nPress Enter to return to menu...");
    getchar();
}


// --- Menu System ---
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_self_mgmt_menu() {
    char choice;
    do {
        clear_screen();
        printf("----------------------------------------\n");
        printf("   Menu: Self-Management & Teamwork\n");
        printf("----------------------------------------\n");
        printf("   H. Manito Game\n");
        printf("   0. Back\n");
        printf("----------------------------------------\n");
        printf("Select an option: ");
        choice = getchar();
        while(getchar() != '\n');
        
        if (toupper(choice) == 'H') {
            playManito();
        }
    } while (choice != '0');
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


// --- Main Entry Point ---
int main() {
    srand(time(NULL));
    show_main_menu();
    printf("\nExiting program. Goodbye!\n");
    return 0;
}
