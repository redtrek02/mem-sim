#include "mem_sim.h"

#define MEM_SIZE 255
#define DELIMITERS " "
#define DEFAULT_ARGS_SZ 1000
#define DEFAULT_INPUT_SZ 1

#define DEBUG_PARSE 1
#define DEBUG_TEST 0

block_t* find_block(int address) {

	block_t* current = get_mem_start();
	while (current) {
		if (current->idx == address) {
			return current;
		}
		
		current = current->next;
	}

	return NULL;
}

void print_mem() {
	block_t* current = get_mem_start();
	while (current) {
		printf("{idx: 0x%02x, sz: %03d, st: %s}\n", current->idx, current->size, current->free ? "free" : "used");
		current = current->next;
	}
}

int cmd_print(char** args) {
	printf("show called!\n");
	print_mem();

	return 0;
}

int cmd_malloc(char** args) {
	printf("malloc called!");
	block_t* result;
	
	if (args[0]) {
		int size = atoi(args[0]);
		result = memory_allocate_ff(size);
	} else {
		printf("Invalid size argument for malloc\n");
		return -2;
	}
	
	if (!result) {
		printf("malloc failed\n");
		return -1;
	}

	return 0;
}

int cmd_mallocbf(char** args) {
	printf("malloc called!");
	block_t* result;
	
	if (args[0]) {
		int size = atoi(args[0]);
		result = memory_allocate_bf(size);
	} else {
		printf("Invalid size argument for mallocbf\n");
		return -2;
	}
	
	if (!result) {
		printf("mallocbf failed\n");
		return -1;
	}
	return 0;
}

int cmd_free(char** args) {
	printf("free called!");

	if (args[0]) {
		int address = strtol(args[0], NULL, 16);
		block_t* selected_block = find_block(address);
		if (!selected_block) {
			return -1;
		} else if (selected_block->free == true) {
			return -3;
		}
		memory_free(selected_block);
	} else {
		return -2;
	}
	return 0;	
}

int cmd_draw(char** args) {
	printf("cmd_draw() called\n");

	draw_memory();
	
	return 0;
}

cmd commands_list[] = {
	{"malloc"  , &cmd_malloc},
	{"free"    , &cmd_free},
	{"print"   , &cmd_print},
	{"draw"	   , &cmd_draw},
	{"mallocbf", &cmd_mallocbf},
};

block_t* create_memory(uint8_t size) {
	block_t* my_block = malloc(sizeof(block_t));
	if (my_block == NULL) {
		perror("malloc for memory was unsucessful\n");
	}

	my_block->next = NULL;
	my_block->prev = NULL;
	my_block->size = size;
	my_block->idx  = 0;
	my_block->free = true;
	return my_block;
}

// Global memory
block_t* memory = NULL;

block_t* memory_merge_prev(block_t* selected) {
	block_t* merged = selected;
	if (selected->prev && selected->prev->free) {
		selected->prev->next = selected->next;
		if (selected->next) {
			selected->next->prev = selected->prev;
		}
		selected->prev->size += selected->size;
		merged = selected->prev;
		merged->idx = selected->idx;
		free(selected);
	}
	return merged;
}

block_t* memory_merge_next(block_t* selected) {
	block_t* merged = selected;
	if (selected->next && selected->next->free) {
		selected->next->prev = selected->prev;
		if (selected->prev) {
			selected->prev->next = selected->next;
		}
		selected->next->size += selected->size;
		merged = selected->next;
		merged->idx = selected->idx;
		free(selected);
	}
	return merged;
}

void memory_merge(block_t* selected) {
	selected = memory_merge_prev(selected);
	selected = memory_merge_next(selected);
}

void memory_free(block_t* selected) {
	selected->free = true;
	memory_merge(selected);
}

block_t* get_mem_start() {
	block_t* current = memory;
	while(current->prev != NULL) {
		current = current->prev;
	}
	return current;
}

void clean_block(block_t* selected) {
	if (!selected) {
		printf("Error: selected block in clean_block() is NULL\n");
	}
	if (selected->size == 0) {
		if (selected->next) {
			selected->next->prev = selected->prev;
		}
		if (selected->prev) {
			selected->prev->next = selected->next;
		}

		free(selected);
	}
}

block_t* memory_allocate_ff(int size) {
	block_t* current_block = get_mem_start();
	block_t* allocated_block = NULL; 

	while (current_block) {
		if (current_block->free && current_block->size >= size) {
			allocated_block = malloc(sizeof(block_t));
			if (allocated_block == NULL) {
				printf("malloc for block of size %d failed", size);
			}

			allocated_block->prev = current_block->prev;
			allocated_block->next = current_block;
			allocated_block->size = size;
			allocated_block->idx  = current_block->idx;
			allocated_block->free = false;

			current_block->prev   = allocated_block;
			current_block->size  -= size;
			current_block->idx   += size;
			if (allocated_block->prev) {
				allocated_block->prev->next = allocated_block;
			}
			clean_block(current_block);
			break;
		}
		current_block = current_block->next;
	}

	printf(!allocated_block ? ("allocation of size %d failed") : ("allocation of size %d succeeded"), size);

	return allocated_block;
}

block_t* memory_allocate_bf(int size) {
	block_t* current_block = get_mem_start();
	block_t* allocated_block = NULL;

	int best_fit = MEM_SIZE;
	block_t* best_fit_block = NULL;

	while (current_block) {
		if (current_block->free && current_block->size >= size) {
			if (current_block->size <= best_fit) {
				best_fit = current_block->size;
				best_fit_block = current_block;
			}
		}

		current_block = current_block->next;
	}

	if (!best_fit_block) {
		printf("allocation of size %d failed", size);
		return NULL;
	}

	allocated_block = malloc(sizeof(block_t));
	if (!allocated_block) {
		perror("Error: malloc for block inside of memory_allocate_bf failed");
		return NULL;
	}

	allocated_block->idx  = best_fit_block->idx;
	allocated_block->size = size;
	allocated_block->prev = best_fit_block->prev;
	allocated_block->next = best_fit_block;
	best_fit_block->prev  = allocated_block;
	best_fit_block->idx  += size;
	best_fit_block->size -= size;

	if (allocated_block->prev) {
		allocated_block->prev->next = allocated_block;
	}

	clean_block(best_fit_block);

	return allocated_block;
}


void draw_memory() {
	uint8_t selected_address = 0;
	block_t* current_block = get_mem_start();
	while(current_block != NULL) {
		for (int i = 0; i < current_block->size; i++) {
			if (selected_address % 8 == 0) {
				printf("\n0x%02x  |", selected_address);
			}
			selected_address++;
			printf(current_block->free ? "  O" : "  X");
		}
		current_block = current_block->next;
	}

	printf("\n");
}

char** parse_user_command(char* user_cmd) {
	char** args = malloc(sizeof(char*) * DEFAULT_ARGS_SZ);
	if (args == NULL) {
		perror("ERROR: malloc for args failed\n");
	}

	char* arg = NULL;;
	uint32_t args_idx = 1;
	arg = strtok(user_cmd, DELIMITERS);

	if (arg == NULL) {
		printf("ERROR: No actual arguments found.\n");
		return NULL;
	}
	args[0] = arg;

	while(1) {
		arg = strtok(NULL, DELIMITERS);
		if (arg == NULL) {
			args[args_idx] == NULL;
			break;
		}

		args[args_idx] = arg;
		args_idx++;
	}

	return args;
}

char* read_user_command() {
	char* buffer = malloc(DEFAULT_INPUT_SZ * sizeof(char));
	if (!buffer) {
		perror("read_user_command: malloc of buffer on read failed.");
		return NULL;
	}
	
	char character;
	int size = 0;

	do {
		character = getchar(); 
		
		if (size > DEFAULT_INPUT_SZ) {
			buffer = realloc(buffer, size * 2);	
			if (!buffer) {
				perror("read_user_command(): realloc of buffer on read failed");
				return NULL;
			}
		}
		
		if (character == EOF || character == '\n') {
			buffer[size] = '\0';
			break;
		}

		buffer[size] = character;
		size++; 
	} while (character);

	return buffer;
}

void execute_user_command(char** args) {
	uint32_t current_idx = 0;
	if (args == NULL) {
		printf("Error: arguments list is null\n");
		return;
	}

	int num_commands = sizeof(commands_list) / sizeof(cmd);
	for (int i = 0; i < num_commands; i++) {
		//printf("DEBUG: INP %s\n", args[0]);
		//printf("DEBUG: CMD %s\n", commands_list[i].name);
		if (strcmp(args[0], commands_list[i].name) == 0) {
			//printf("Entered\n");
			printf("%d\n", commands_list[i].handle(args + 1));
		}
	}
	
}

int main() {

#if (DEBUG_TEST == 1)
	void test();
#endif /* #if (DEBUG_TEST == 1) */
	memory = create_memory(MEM_SIZE);
	char* user_cmd = NULL;
	char** args = NULL;

	printf("Use malloc x where x is an integer to allocate memory.\nUse free x where x is a memory address to free memory.\n");

	while(1) {
#if (DEBUG_PARSE == 1)
		user_cmd = read_user_command();
		args = parse_user_command(user_cmd);
		execute_user_command(args);
		//draw_memory();
		/*
		while(*args != NULL) {
			printf("%s\n", *args);
			args++;
		}
		*/
		free(user_cmd);
		free(args);
#else
		draw_memory();
		get_user_input();
		
#endif /* #if (DEBUG_PARSE == 1) */
	}
	return 0;
}
