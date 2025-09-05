#include "mem_sim.h"
#include "commands.h"

int main() {

	memory = create_memory(MEM_SIZE);
	char* user_cmd = NULL;
	char** args = NULL;

	printf("Use malloc x where x is an integer to allocate memory.\nUse free x where x is a memory address to free memory.\n");

	while(1) {

		user_cmd = read_user_command();
		args = parse_user_command(user_cmd);
		execute_user_command(args);
		
		free(user_cmd);
		free(args);
	}
	return 0;
}
