#include "main.h"

int main() {
    while (1) {
        // print prompt
        printf(PROMPT);
        // get input
        char input[MAX_INPUT_LEN] = {0};
        fgets(input, MAX_INPUT_LEN, stdin);
        // too long input
        if (input[strlen(input) - 1] != '\n' && input[strlen(input) - 1] != 0) {
            prtError();
            // flush stdin
            while (getchar() != '\n')
                ;
            continue;
        }
        // remove '\n'
        input[strlen(input) - 1] = '\0';
        // no input
        if (strlen(input) == 0) continue;

        // split
        char command[MAX_COMMAND_LEN] = {0};
        char params[MAX_PARAMS_LEN] = {0};
        split(input, command, params);

        // check command
        command_mode c_mode = check_command(command);

        char fullcmd[MAX_FULLCMD_LEN] = {0};
        char LBA[4] = {0}, value[16] = {0};

        switch (c_mode) {
        case READ:
            // if command is valid
            if (isValidLBA(params)) {
                // render command and execute
                makeFullCmd(fullcmd, params, READ);
                system(fullcmd);

                // print result
                uint32_t result;
                FILE *fp = fopen(RESULT_FILE, "r");
                fread(&result, sizeof(uint32_t), 1, fp);
                printf("0x%.8X\n", result);
                fclose(fp);
            }
            // else print error message
            else
                prtError();
            break;

        case WRITE:
            // split params to LBA and value
            sscanf(params, "%s %s", LBA, value);

            // if command is valid
            if (isValidLBA(LBA) && isValidValue(value)) {
                // render command and execute
                makeFullCmd(fullcmd, params, WRITE);
                system(fullcmd);
            }
            // else print error message
            else
                prtError();
            break;

        case FULLREAD:
            // if command is valid (no params)
            if (strlen(params) == 0) {
                // for every LBA
                for (int i = 0; i < STORAGE_SIZE; i++) {
                    // render command and execute
                    sprintf(params, "%d", i);
                    makeFullCmd(fullcmd, params, READ);
                    system(fullcmd);

                    // print result
                    uint32_t value;
                    FILE *fp = fopen(RESULT_FILE, "r");
                    fread(&value, sizeof(uint32_t), 1, fp);
                    printf("0x%.8X\n", value);
                    fclose(fp);
                }
            }
            else
                prtError();
            break;

        case FULLWRITE:
            // if command is valid
            if (isValidValue(params)) {
                // for every LBA in ssd
                for (int i = 0; i < STORAGE_SIZE; i++) {
                    // render command(add LBA) and execute
                    char p[48];
                    sprintf(p, "%d %s", i, params);
                    makeFullCmd(fullcmd, p, WRITE);
                    system(fullcmd);
                }
            }
            // else print error message
            else
                prtError();
            break;

        case HELP:
            prtHelp();
            break;

        case EXIT:
            return 0;

        default:
            // else print error message
            prtError();
        }
    }
    return 0;
}
