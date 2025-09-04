/*--------- Includes ---------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*-------- Global Types -------*/
typedef struct block_t {
	struct block_t* next;
	struct block_t* prev;
	uint8_t size;
	uint8_t idx;
	bool free;
} block_t;

typedef int (*cmd_handle)(char** args);

typedef struct cmd {
	char* name;
	cmd_handle handle;
} cmd;


/*-------- Prototypes --------*/

int cmd_malloc(char** args);
int cmd_free(char** args);

block_t* create_memory(uint8_t size);
block_t* memory_allocate_ff(int size);
block_t* memory_allocate_bf(int size);
void memory_free(block_t* selected);
void memory_merge(block_t* selected);
block_t* memory_merge_prev(block_t* selected);
block_t* memory_merge_next(block_t* selected);

block_t* get_mem_start();

void draw_memory();

char* get_user_command();
char** parse_user_command(char* user_cmd);

