#ifndef SHELL_H
#define SHELL_H

#define SSD_EXEC_FILE   "./ssd"
#define ERROR_MSG       "INVALID COMMAND\n"
#define STORAGE_SIZE    100
#define MAX_INPUT_LEN   32
#define MAX_COMMAND_LEN 16
#define MAX_PARAMS_LEN  16
#define MAX_FULLCMD_LEN 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum mode { NONE, READ, WRITE, FULLREAD, FULLWRITE, HELP, EXIT } command_mode;

int isValidLBA(char *LBA);
int isValidValue(char *value);
void prtHelp();
void prtError();
void split(char input[MAX_INPUT_LEN], char command[MAX_COMMAND_LEN], char params[MAX_PARAMS_LEN]);
void makeFullCmd(char fullcmd[MAX_FULLCMD_LEN], char params[MAX_PARAMS_LEN], command_mode c_mode);
command_mode check_command(char *command);

#endif
