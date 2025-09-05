#include "commands.h"
#include "mem_sim.h"

cmd commands_list[] = {
	{"malloc"  , &cmd_malloc},
	{"free"    , &cmd_free},
	{"print"   , &cmd_print},
	{"draw"	   , &cmd_draw},
	{"mallocbf", &cmd_mallocbf},
};

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

