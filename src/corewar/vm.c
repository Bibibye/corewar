#include "vm.h"
#include "op.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

process	*init_process(reg number, reg player)
{
	process	*tmp;

	tmp = malloc(sizeof(*tmp));
	memset(tmp->registers, 0, sizeof(tmp->registers));
	tmp->carry = false;
	tmp->pc = 0;
	tmp->registers[0] = player;
	tmp->nb = number;
	tmp->player = player;
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
	p->time = get_time(p->instruction[0]);
	return true;
}

void	dump_process(process *p)
{
	if (!p)
		return;
	printf("Process : %d (player %d)\n", p->nb, p->player);
	for (int i = 0; i < REG_NUMBER; i += 2)
		printf("reg %d : %X reg %d : %X\n", i, p->registers[i], i + 1, p->registers[i + 1]);
	printf("pc : %X -> ", p->pc);
	for (unsigned int i = 0; i < sizeof(p->instruction); ++i)
		printf("%X ", p->instruction[i]);
	printf("\n");
	printf("carry : %d\n", p->carry);
	printf("time : %d\n", p->time);
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

void	add_process(vm *v, reg player, uint32_t pc)
{
	process	*tmp = init_process(v->nb_procs, player);
	tmp->pc = pc;
	tmp->begin = pc;
	memcpy(tmp->instruction, &v->mem[pc], sizeof(tmp->instruction));
	tmp->time = get_time(tmp->instruction[0]);
	push_queue(&v->procs, tmp);
	++v->nb_procs;
}

void		copy_process(vm *v, process *p, uint32_t pc)
{
	process	*tmp;

	add_process(v, p->player, pc);
	tmp = v->procs->proc;
	memcpy(tmp->registers, p->registers, sizeof(p->registers));
	tmp->carry = p->carry;
}

uint32_t	execute_step_vm(vm *v)
{
	process		*p = 0;
	uint32_t	procs = 0;
	process		*begin = 0;

	begin = v->procs->proc;
	do
	{
		p = pop_queue(&v->procs);
#if DEBUG
		dump_process(p);
#endif
		if (execute_process(v, p))
		{
			push_queue(&v->procs, p);
			++procs;
		}
		else
			delete_process(&p);
	}while (v->procs && begin != v->procs->proc);
	v->nb_procs = procs;
	return procs;
}

void	dump_vm(vm *v)
{
	for (unsigned int i = 0; i < sizeof(v->mem); ++i)
	{
		if (!(i % 32))
			printf("\n%-4X : ", i);
		printf("%-2X ", v->mem[i]);
	}
	printf("\n");
}
