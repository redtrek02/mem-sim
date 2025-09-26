#include "commands.h"
#include "mem_sim.h"
#include "error_codes.h"

cmd commands_list[] = {
	{"malloc"  , cmd_malloc  },
	{"free"    , cmd_free    },
	{"print"   , cmd_print   },
	{"draw"	   , cmd_draw    },
	{"mallocbf", cmd_mallocbf},
	{"help"	   , cmd_help    },
	{"quit"	   , cmd_quit	 },
};

int commands_list_sz = sizeof(commands_list) / sizeof(commands_list[0]);

int cmd_help(char** args) {
	printf("===== AVAILABLE COMMANDS ===== \n");
	for (int i = 0; i < sizeof(commands_list) / sizeof(cmd); i++) {
		printf("%s\n", commands_list[i].name);
	}

	printf("============================== \n");
	return E_PASS;
}

int cmd_print(char** args) {
	printf("show called!\n");
	print_mem();

	return E_PASS;
}

int cmd_malloc(char** args) {
	printf("malloc called!");
	block_t* result;
	
	if (args[0] && atoi(args[0]) > 0) {
		int size = atoi(args[0]);
		result = memory_allocate_ff(size);
	} else {
		printf("Invalid size argument for malloc\n");
		return E_FAIL;
	}
	
	if (!result) {
		printf("malloc failed\n");
		return E_FAIL;
	}

	return E_PASS;
}

int cmd_mallocbf(char** args) {
	printf("malloc called!");
	block_t* result;
	
	if (args[0]) {
		int size = atoi(args[0]);
		result = memory_allocate_bf(size);
	} else {
		printf("Invalid size argument for mallocbf\n");
		return E_FAIL;
	}
	
	if (!result) {
		printf("mallocbf failed\n");
		return E_FAIL;
	}
	return E_PASS;
}

int cmd_free(char** args) {
	printf("free called!");

	if (args[0]) {
		int address = strtol(args[0], NULL, 16);
		block_t* selected_block = find_block(address);
		if (!selected_block) {
			return E_FAIL;
		} else if (selected_block->free == true) {
			return E_FAIL;
		}
		memory_free(selected_block);
	} else {
		return E_FAIL;
	}
	return E_PASS;	
}

int cmd_draw(char** args) {
	printf("cmd_draw() called\n");

	draw_memory();
	
	return E_PASS;
}

int cmd_quit(char** args) {
	exit(0);
	return E_FAIL;
}

