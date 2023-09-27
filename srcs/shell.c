#include "shell.h"

void prtError() {
    printf(ERROR_MSG);
}

void prtHelp() {
    system("cat help.txt");
}

void makeFullCmd(char fullcmd[MAX_FULLCMD_LEN], char params[MAX_PARAMS_LEN], command_mode c_mode) {
    switch (c_mode) {
    case READ:
        sprintf(fullcmd, "%s R %s", SSD_EXEC_FILE, params);
        break;
    case WRITE:
        sprintf(fullcmd, "%s W %s", SSD_EXEC_FILE, params);
        break;
    default:
        break;
    }
}

int isValidValue(char *value) {
    // empty
    if (strlen(value) == 0) return 0;

    for (size_t i = 0; i < strlen(value); i++) {
        if ((value[i] < '0' || value[i] > '9') && (value[i] < 'A' || value[i] > 'F')) return 0;
    }

    return 1;
}

void split(char input[MAX_INPUT_LEN], char command[MAX_COMMAND_LEN], char params[MAX_PARAMS_LEN]) {
    sscanf(input, "%s %[^\0]", command, params);
    for (size_t i = 0; i < strlen(params); i++)
        params[i] = toupper(params[i]);
    for (size_t i = 0; i < strlen(command); i++)
        command[i] = tolower(command[i]);
}

int isValidLBA(char *LBA) {
    if (strlen(LBA) == 0) return 0;

    for (unsigned long i = 0; i < strlen(LBA); i++) {
        if ('0' <= LBA[i] && LBA[i] <= '9')
            continue;
        else
            return 0;
    }

    int num = atoi(LBA);
    if (num < 0 || num >= STORAGE_SIZE) return 0;

    return 1;
}

command_mode check_command(char *command) {
    if (strcmp(command, "read") == 0)
        return READ;
    else if (strcmp(command, "write") == 0)
        return WRITE;
    else if (strcmp(command, "fullread") == 0)
        return FULLREAD;
    else if (strcmp(command, "fullwrite") == 0)
        return FULLWRITE;
    else if (strcmp(command, "help") == 0)
        return HELP;
    else if (strcmp(command, "exit") == 0)
        return EXIT;
    else
        return NONE;
}
