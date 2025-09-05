#ifndef COMMANDS_H
#define COMMANDS_H

typedef int (*cmd_handle)(char** args);

typedef struct cmd {
	char* name;
	cmd_handle handle;
} cmd;

int cmd_print(char** args);

int cmd_malloc(char** args);

int cmd_mallocbf(char** args);

int cmd_free(char** args);

int cmd_draw(char** args);

extern cmd commands_list[5];

#endif /* #ifndef COMMANDS_H */