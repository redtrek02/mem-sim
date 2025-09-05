#ifndef MEM_SIM_H
#define MEM_SIM_H

/*--------- Includes ---------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*---------- Macros -----------*/
#define MEM_SIZE 255
#define DELIMITERS " "
#define DEFAULT_ARGS_SZ 1000
#define DEFAULT_INPUT_SZ 1

/*-------- Global Types -------*/
typedef struct block_t {
	struct block_t* next;
	struct block_t* prev;
	uint8_t size;
	uint8_t idx;
	bool free;
} block_t;

extern block_t* memory;


/*-------- Prototypes --------*/
block_t* create_memory(uint8_t size);
block_t* get_mem_start();

block_t* memory_allocate_ff(int size);
block_t* memory_allocate_bf(int size);
void clean_block(block_t* selected);

void memory_free(block_t* selected);
block_t* find_block(int address);
void memory_merge(block_t* selected);
block_t* memory_merge_prev(block_t* selected);
block_t* memory_merge_next(block_t* selected);

void print_mem();
void draw_memory();

char* read_user_command();
char** parse_user_command(char* user_cmd);
void execute_user_command(char** args);

#endif /* #ifndef MEM_SIM_H */