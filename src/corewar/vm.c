#include "vm.h"
#include "op.h"
#include <stdlib.h>
#include <string.h>

process	*init_process(reg number)
{
	process	*tmp;

	tmp = malloc(sizeof(*tmp));
	memset(tmp->registers, 0, sizeof(tmp->registers));
	tmp->carry = false;
	tmp->pc = 0;
	tmp->registers[0] = number;
	tmp->time = 1;
	tmp->begin = 0;
	memset(tmp->instruction, 0, sizeof(tmp->instruction));
	return tmp;
}

void	delete_process(process **p)
{
	if (p && *p)
	{
		free(*p);
		*p = 0;
	}
}

bool	execute_process(vm *v, process *p)
{
	--p->time;
	if (p->time)
		return true;
	if (!execute_op(v, p, p->instruction))
		return false;
	memcpy(p->instruction, &v->mem[p->pc], sizeof(p->instruction));
	return true;
}

queue	*init_queue(process *p)
{
	queue	*tmp;

	tmp = malloc(sizeof(*tmp));
	tmp->next = 0;
	tmp->proc = p;
	return tmp;
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

vm	*init_vm(uint32_t nb_players)
{
	vm	*tmp;

	tmp = malloc(sizeof(*tmp));
	memset(tmp->mem, 0, sizeof(tmp->mem));
	tmp->procs = 0;
	tmp->nb_procs = 0;
	tmp->nb_players = nb_players;
	memset(tmp->players, 0, MAX_PLAYERS * sizeof(bool));
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
	tmp->begin = pc;
	memcpy(tmp->instruction, &v->mem[pc], sizeof(tmp->instruction));
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
