#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT "testshell >> "
#define SSD_EXEC_FILE "./ssd"
#define RESULT_FILE "./result"
#define STORAGE_SIZE 100
#define MAX_INPUT_LEN 32
#define MAX_COMMAND_LEN 16
#define MAX_PARAMS_LEN 16
#define MAX_FULLCMD_LEN 32
#define DATA_LEN 11

typedef enum mode
{
    NONE,
    READ,
    WRITE,
    FULLREAD,
    FULLWRITE
} command_mode;

void split(char input[64], char command[16], char params[48]);
void makeFullCmd(char *fullcmd, char params[48], command_mode c_mode);
int isValidLBA(char *LBA);
int isValidValue(char *value);
command_mode check_command(char *command);

int main()
{
    while (1)
    {
        printf(PROMPT);
        char input[MAX_INPUT_LEN] = {0};
        fgets(input, MAX_INPUT_LEN, stdin);
        // too long input
        if (input[strlen(input) - 1] != '\n' && input[strlen(input) - 1] != 0)
        {
            printf("INVALID COMMAND\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        input[strlen(input) - 1] = '\0';
        // no input
        if (strlen(input) == 0)
            continue;
        // exit
        if (strcmp(input, "exit") == 0)
            break;
        // help
        if (strcmp(input, "help") == 0)
        {
            system("cat ./help.txt");
            continue;
        }

        // split
        char command[MAX_COMMAND_LEN] = {0};
        char params[MAX_PARAMS_LEN] = {0};
        split(input, command, params);
        command_mode c_mode = check_command(command);

        char fullcmd[MAX_FULLCMD_LEN] = {0};

        char LBA[4] = {0}, value[16] = {0};
        switch (c_mode)
        {
        case READ:
            // if command is valid
            if (isValidLBA(params))
            {
                // render command and execute
                makeFullCmd(fullcmd, params, READ);
                system(fullcmd);

                // print result
                uint32_t value;
                FILE *fp = fopen(RESULT_FILE, "r");
                fread(&value, sizeof(uint32_t), 1, fp);
                printf("0x%.8X\n", value);
                fclose(fp);
            }
            // else print error message
            else
                printf("INVALID COMMAND\n");
            break;

        case WRITE:
            // split params to LBA number and value
            sscanf(params, "%s %s", LBA, value);

            // if command is valid
            if (isValidLBA(LBA) && isValidValue(value))
            {
                // render command and execute
                makeFullCmd(fullcmd, params, WRITE);
                system(fullcmd);
            }
            // else print error message
            else
                printf("INVALID COMMAND\n");
            break;

        case FULLREAD:
            // if command is valid (no params)
            if (strlen(params) == 0)
            {
                // for every LBA
                for (int i = 0; i < STORAGE_SIZE; i++)
                {
                    // render command and execute
                    sprintf(params, "%d", i);
                    makeFullCmd(fullcmd, params, READ);
                    system(fullcmd);

                    // print result
                    uint32_t value;
                    FILE *fp = fopen(RESULT_FILE, "r");
                    fread(&value, sizeof(uint32_t), 1, fp);
                    printf("LBA[%d]: 0x%.8X\n", i, value);
                    fclose(fp);
                }
            }
            else
                printf("INVALID COMMAND\n");
            break;

        case FULLWRITE:
            // if command is valid
            if (isValidValue(params))
            {
                // for every LBA
                for (int i = 0; i < STORAGE_SIZE; i++)
                {
                    // render command and execute
                    char p[48];
                    sprintf(p, "%d %s", i, params);
                    makeFullCmd(fullcmd, p, WRITE);
                    system(fullcmd);
                }
            }
            // else print error message
            else
                printf("INVALID COMMAND\n");
            break;

        default:
            // invalid command
            printf("INVALID COMMAND\n");
        }
    }
    return 0;
}

void split(char input[64], char command[16], char params[48])
{
    sscanf(input, "%s %[^\0]", command, params);
}

command_mode check_command(char *command)
{
    if (strcmp(command, "read") == 0)
        return READ;
    else if (strcmp(command, "write") == 0)
        return WRITE;
    else if (strcmp(command, "fullread") == 0)
        return FULLREAD;
    else if (strcmp(command, "fullwrite") == 0)
        return FULLWRITE;
    else
        return NONE;
}

int isValidLBA(char *LBA)
{
    if (strlen(LBA) == 0)
    {
        return 0;
    }
    for (unsigned long i = 0; i < strlen(LBA); i++)
    {
        if ('0' <= LBA[i] && LBA[i] <= '9')
            continue;
        else
        {
            return 0;
        }
    }
    int num = atoi(LBA);
    if (num < 0 || num >= STORAGE_SIZE)
    {
        return 0;
    }
    return 1;
}

int isValidValue(char *value)
{
    // empty
    if (strlen(value) == 0)
        return 0;

    // hex
    if (value[0] == '0' && value[1] == 'x')
    {
        if (strlen(value) > 10)
            return 0;
        if (value[0] != '0' || value[1] != 'x')
            return 0;

        for (unsigned long i = 2; i < strlen(value); i++)
        {
            if ((value[i] < 'A' && value[i] > 'F') && (value[i] < '0' && value[i] > '9'))
                return 0;
        }
        return 16;
    }

    // bin
    else if (value[0] == 0 && value[1] == 'b')
    {
        if (strlen(value) > 34)
            return 0;
        for (unsigned long i = 2; i < strlen(value); i++)
        {
            if (value[i] != '0' && value[i] != '1')
                return 0;
        }
        return 2;
    }

    // dec
    else
    {
        if (strlen(value) > 10)
            return 0;
        unsigned long long num = strtoull(value, NULL, 10);
        if (num > 0xFFFFFFFF)
            return 0;
        for (unsigned long i = 0; i < strlen(value); i++)
        {
            if (value[i] < '0' && value[i] > '9')
                return 0;
        }
        return 10;
    }
    // not either of them (hex, bin, dec)
    return 0;
}

void makeFullCmd(char *fullcmd, char params[48], command_mode c_mode)
{
    switch (c_mode)
    {
    case READ:
        sprintf(fullcmd, "%s R %s", SSD_EXEC_FILE, params);
        break;
    case WRITE:
        sprintf(fullcmd, "%s W %s", SSD_EXEC_FILE, params);
        break;
    case FULLREAD:
        sprintf(fullcmd, "%s R %s", SSD_EXEC_FILE, params);
        break;
    case FULLWRITE:
        sprintf(fullcmd, "%s W %s", SSD_EXEC_FILE, params);
        break;
    default:
        break;
    }
}
