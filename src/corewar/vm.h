#ifndef _VM_H_
# define _VM_H_

#include "machine.h"
#include <stdbool.h>

typedef struct	process
{
	reg			registers[REG_NUMBER];
	uint32_t	pc;
	bool		carry;
}process;

process	*init_process(reg number);
void	delete_process(process **p);
bool	execute_process(process *p);

typedef struct	queue
{
	process			*proc;
	struct queue	next;
}queue;

queue	*init_queue(process *p);
void	delete_queue(queue **q);
void	push_queue(queue **head, process *p);
process	*pop_queue(queue **head);

typedef struct vm
{
	int32_t		mem[MEM_SIZE];
	queue		procs;
	uint32_t	nb_procs;
}vm;

vm			*init_vm(void);
void		delete_vm(vm **v);
void		add_process(vm *v, uint32_t pc);
uint32_t	execute_step_vm(vm *v);

#endif /* !_VM_H_ */
