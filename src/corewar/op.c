#include "op.h"

uint32_t	get_time(cell op)
{
	if (!op)
		return 1;
	else if (op == ADD)
		return 4;
	return 1;
}

static void	get_params(uint8_t params[3], cell param)
{
	for (int i = 0; i < 3; ++i)
		params[i] = decode_parameters(param, i + 1);
}

static void	get_size_params(uint8_t params[3], cell param)
{
	for (int i = 0; i < 3; ++i)
	{
		params[i] = decode_parameters(param, i + 1);
		if (params[i])
			params[i] = params[i] == REG_PARAM ? REG_SIZE : DIR_SIZE;
	}
}

static bool	add(cell *instruction, vm *v, process *p)
{
	(void)v;
	if (instruction[4] - 1 < 0 || instruction[4] - 1 >= REG_NUMBER)
		return false;
	else if (instruction[3] - 1 < 0 || instruction[3] - 1 >= REG_NUMBER)
		return false;
	else if (instruction[2] - 1 < 0 || instruction[2] - 1 >= REG_NUMBER)
		return false;
	p->registers[instruction[4] - 1] = p->registers[instruction[3] - 1]
										+ p->registers[instruction[2] - 1];
	p->pc += 5;
	p->carry = p->registers[instruction[4] - 1] != 0;
	return true;
}

static bool	ld(cell *instruction, vm *v, process *p)
{
	uint8_t	params[3];
	uint8_t	size[3];

	get_params(params, instruction[1]);
	get_size_params(size, instruction[1]);
	if (!params[0] || instruction[2 + size[0]] - 1 < 0 || instruction[2 + size[0]] - 1 >= REG_NUMBER)
		return false;
	reg	tmp = p->registers[instruction[2 + size[0]] - 1];
	if (params[0] == REG_PARAM)
		tmp = p->registers[instruction[2] - 1];
	else if (params[0] == DIR_PARAM)
		tmp = *(reg*)(&instruction[2]);
	else if (params[0] == IND_PARAM)
		tmp = *(reg*)&(v->mem[(p->begin + *(reg*)&(instruction[2])) % MEM_SIZE]);
	p->registers[instruction[2 + size[0]] - 1] = tmp;
	p->pc += (3 + size[0]);
	p->carry = tmp != 0;
	return true;
}

bool	execute_op(vm *v, process *p, cell *instruction)
{
	switch (instruction[0])
	{
		case ADD:
			return add(instruction, v, p);
			break;
		case LD:
			return ld(instruction, v, p);
			break;
	}
	return false;
}
