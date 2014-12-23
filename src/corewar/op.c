#include "op.h"

uint32_t	get_time(cell op)
{
	if (!op)
		return 1;
	return 1;
}

static bool	add(cell *instruction, vm *v, process *p)
{
	(void)v;
	if (instruction[4] - 1 < 0 || instruction[4] - 1 > REG_NUMBER)
		return false;
	else if (instruction[3] - 1 < 0 || instruction[3] - 1 > REG_NUMBER)
		return false;
	else if (instruction[2] - 1 < 0 || instruction[2] - 1 > REG_NUMBER)
		return false;
	p->registers[instruction[4] - 1] = p->registers[instruction[3] - 1]
										+ p->registers[instruction[2] - 1];
	p->pc += 5;
	return true;
}

bool	execute_op(vm *v, process *p, cell *instruction)
{
	switch (instruction[0])
	{
		case ADD:
			return add(instruction, v, p);
			break;
	}
	return false;
}
