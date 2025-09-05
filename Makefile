CC := gcc

program: main.o mem_sim.o commands.o
	$(CC) main.o mem_sim.o commands.o -o program 

main: main.c mem_sim.h commands.h
	$(CC) -c main.c

mem_sim: mem_sim.c mem_sim.h commands.h
	$(CC) -c mem_sim.c

commands: commands.c commands.h mem_sim.h
	$(CC) -c commands.c