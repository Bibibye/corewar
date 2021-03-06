#ifndef _VM_H_
# define _VM_H_

#include "machine.h"
#include "instructions.h"
#include <stdbool.h>

typedef struct vm vm;

typedef struct	process
{
	uint32_t	nb;
	reg			player;
	reg			registers[REG_NUMBER];
	uint32_t	pc;
	bool		carry;
	uint32_t	time;
	uint32_t	begin;
	cell		instruction[2 * OP_SIZE + 3 * DIR_SIZE];
}process;

process	*init_process(reg number, reg player);
void	delete_process(process **p);
bool	execute_process(vm *v, process *p);
void	dump_process(process *p);

typedef struct	queue
{
	process			*proc;
	struct queue	*next;
}queue;

queue	*init_queue(process *p);
void	delete_queue(queue **q);
void	push_queue(queue **head, process *p);
process	*pop_queue(queue **head);

struct vm
{
	cell		mem[MEM_SIZE];
	queue		*procs;
	uint32_t	nb_procs;
	uint32_t	nb_players;
	bool		players[MAX_PLAYERS];
};

vm			*init_vm(uint32_t nb_players);
void		delete_vm(vm **v);
void		add_process(vm *v, reg player, uint32_t pc);
void		copy_process(vm *v, process *p, uint32_t pc);
uint32_t	execute_step_vm(vm *v);
void		dump_vm(vm *v);

#endif /* !_VM_H_ */
