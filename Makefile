CC := gcc

mem_sim: mem_sim.c mem_sim.h
	$(CC) -o mem_sim mem_sim.c
