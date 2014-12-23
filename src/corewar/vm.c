#include "vm.h"
#include <stdlib.h>
#include <string.h>

process	*init_process(reg number)
{
	process	*tmp;

	tmp = malloc(sizeof(*tmp));
	memset(tmp->registers, 0, REG_NUMBER * sizeof(reg));
	tmp->carry = false;
	tmp->pc = 0;
	tmp->registers[0] = number;
	return tmp;
}

void	delete_process(process **p)
{
	if (p && *p)
	{
		free(*p);
		*p = 0:
	}
}

bool	execute_process(vm *v, process *p)
{
	return execute_op(v, p, v->mem[p->pc]);
}

queue	*init_queue(process *p)
{
	queue	*tmp;

	tmp = malloc(sizeof(*tmp));
	tmp->next = 0;
	tmp->proc = p;
}

void	delete_queue(queue **q)
{
	if (q && *q)
	{
		free(*q);
		*q = 0;
	}
}

void	push_queue(queue **head, process *p)
{
	if (!head)
		return ;
	queue	*tmp = init_queue(p);
	if (!*head)
	{
		*head = tmp;
		return ;
	}
	queue	*it = *head;
	while (it->next)
		it = it->next;
	it->next = tmp;
}

process	*pop_queue(queue **head)
{
	process	*tmp;
	queue	*next;

	tmp = (*head)->proc;
	next = (*head)->next;
	delete_queue(head);
	*head = next;
	return tmp;
}

vm	*init_vm(void)
{
	vm	*tmp;

	tmp = malloc(sizeof(*tmp));
	memset(tmp->mem, 0, sizeof(int32_t) * MEM_SIZE);
	tmp->procs = 0;
	tmp->nb_procs = 0;
	return tmp;
}

void	delete_vm(vm **v)
{
	if (!v || !*v)
		return ;
	while ((*v)->procs)
		pop_queue(&(*v)->procs);
	free(*v);
	*v = 0;
}

void	add_process(vm *v, uint32_t pc)
{
	process	*tmp = init_process(v->nb_procs);
	tmp->pc = pc;
	push_queue(&v->procs, tmp);
	++v->nb_procs;
}

uint32_t	execute_step_vm(vm *v)
{
	process		*p = 0;
	uint32_t	procs = 0;

	while (v->procs)
	{
		p = pop_queue(&v->procs);
		if (execute_process(v, p))
		{
			push_queue(&v->procs, p);
			++procs;
		}
		else
			delete_process(&p);
	}
	v->nb_procs = procs;
	return procs;
}
