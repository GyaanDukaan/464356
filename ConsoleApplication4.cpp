#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>   // For malloc and free

#define MAX_FILENAME_LENGTH 100
#define MAX_RULE_LENGTH 100

// Structure to hold a naming rule
typedef struct {
    char name[MAX_RULE_LENGTH];    // Rule name (e.g., "max_length", "allowed_chars")
    char value[MAX_RULE_LENGTH];  // Rule value
} rule_t;

int validate_filename(const char* filename, rule_t rules[], int num_rules) {
    // Check for empty filename
    if (strlen(filename) == 0) {
        return 0;
    }

    // Check each rule dynamically
    for (int i = 0; i < num_rules; i++) {
        const char* rule_name = rules[i].name;
        const char* rule_value = rules[i].value;

        if (strcmp(rule_name, "max_length") == 0) {
            int max_length = atoi(rule_value);
            if (strlen(filename) > max_length) {
                printf("Error: Filename length exceeds the maximum limit of %d.\n", max_length);
                return 0;
            }
        }
        else if (strcmp(rule_name, "allowed_chars_name") == 0) {
            char allowed_chars_name[MAX_RULE_LENGTH];
            strcpy(allowed_chars_name, rule_value);

            // Remove hyphen from allowed characters for name part
            for (char* c = allowed_chars_name; *c != '\0'; c++) {
                if (*c == '-') {
                    *c = '\0';
                    break;
                }
            }

            for (char* c = filename; c < strrchr(filename, '.'); c++) {
                if (!strchr(allowed_chars_name, *c)) {
                    printf("Error: Invalid character found in the filename: '%c'. Valid characters are: %s.\n", *c, rule_value);
                    return 0;
                }
            }
        }
        else if (strcmp(rule_name, "allowed_chars_ext") == 0) {
            for (char* c = strrchr(filename, '.') + 1; *c != '\0'; c++) {
                if (!strchr(rule_value, *c)) {
                    printf("Error: Invalid character found in the file extension: '%c'. Valid characters are: %s.\n", *c, rule_value);
                    return 0;
                }
            }
        }
    }

    return 1; // Filename validates
}

int load_rules_from_file(rule_t rules[], int* num_rules) {
    FILE* fp = fopen("rules.cfg", "r");
    if (fp == NULL) {
        perror("Error opening rules.cfg");
        return 0;
    }

    int i = 0;
    char line[MAX_RULE_LENGTH * 2];  // Each rule line may contain name and value
    while (fgets(line, sizeof(line), fp) != NULL) {
        char* token = strtok(line, "=");
        if (token == NULL) {
            continue;  // Skip invalid lines
        }

        strcpy(rules[i].name, token);
        token = strtok(NULL, "\n");
        if (token == NULL) {
            continue;  // Skip invalid lines
        }

        strcpy(rules[i].value, token);
        i++;

        if (i == MAX_RULE_LENGTH) {
            fprintf(stderr, "Error: Maximum number of rules reached. Increase MAX_RULE_LENGTH.\n");
            break;
        }
    }

    fclose(fp);
    *num_rules = i;
    return 1;
}

int main() {
    rule_t rules[MAX_RULE_LENGTH];
    int num_rules;

    if (!load_rules_from_file(rules, &num_rules)) {
        return 1;
    }

    const char* filename1 = "valid-movie.mkv";
    const char* filename2 = "too-long-file-name-with-invalid_characters&.mp4";
    const char* filename3 = "noextension.avi";
    const char* filename4 = "invalid char.mp4";
    const char* filename5 = "valid_name_20230731.avi";


    printf("Testing filename: %s... ", filename1);
    if (validate_filename(filename1, rules, num_rules)) {
        printf("Valid\n");
    }

    // Continue testing other filenames as before
    // ...

    return 0;
}