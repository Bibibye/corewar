#include "op.h"

uint32_t	get_time(cell op)
{
	if (!op)
		return 1;
	else if (op == ADD || op == SUB)
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

static bool	is_reg(cell instruction)
{
	return instruction - 1 >= 0 && instruction - 1 < REG_NUMBER;
}

static cell	get_mem(bool relative, reg instruction, vm *v, process *p)
{
	uint32_t	rel = relative ? p->begin : 0;

	if ((int32_t)rel + instruction < 0)
		instruction = instruction + rel + MEM_SIZE;
	else
		instruction += rel;
	return v->mem[(instruction) % MEM_SIZE];
}

static reg	get_data(bool relative, uint8_t param, cell *instruction, vm *v, process *p)
{
	reg			tmp = 0;
	uint8_t		params[3];
	uint8_t		size[3];
	uint32_t	total = 0;

	get_params(params, instruction[1]);
	get_size_params(size, instruction[1]);
	for (int i = 0; i < param - 1; ++i)
		total += size[i];
	if (params[param - 1] == REG_PARAM && is_reg(instruction[2 + total]))
		tmp = p->registers[instruction[2 + total] - 1];
	else if (params[param - 1] == DIR_PARAM)
		tmp = get_param(instruction[2 + total]);
	else if (params[param - 1] == IND_PARAM)
		tmp = get_mem(relative, get_param(instruction[2 + total]), v, p);
	return tmp;
}

static bool	xor(cell *instruction, vm *v, process *p)
{
	uint8_t	size[3];
	reg		pa[2];

	get_size_params(size, instruction[1]);
	if (!is_reg(instruction[2 + size[0] + size[1]]))
		return false;
	for (int i = 0; i < 2; ++i)
		pa[i] = get_data(true, i + 1, instruction, v, p);
	p->registers[instruction[2 + size[0] + size[1]]] = pa[0] ^ pa[1];
	p->carry = (pa[0] ^ pa[1]) != 0;
	return true;
}

static bool	and(cell *instruction, vm *v, process *p)
{
	uint8_t	size[3];
	reg		pa[2];

	get_size_params(size, instruction[1]);
	if (!is_reg(instruction[2 + size[0] + size[1]]))
		return false;
	for (int i = 0; i < 2; ++i)
		pa[i] = get_data(true, i + 1, instruction, v, p);
	p->registers[instruction[2 + size[0] + size[1]]] = pa[0] & pa[1];
	p->carry = (pa[0] & pa[1]) != 0;
	return true;
}

static bool	or(cell *instruction, vm *v, process *p)
{
	uint8_t	size[3];
	reg		pa[2];

	get_size_params(size, instruction[1]);
	if (!is_reg(instruction[2 + size[0] + size[1]]))
		return false;
	for (int i = 0; i < 2; ++i)
		pa[i] = get_data(true, i + 1, instruction, v, p);
	p->registers[instruction[2 + size[0] + size[1]]] = pa[0] | pa[1];
	p->carry = (pa[0] | pa[1]) != 0;
	return true;
}

static bool	zjmp(cell *instruction, vm *v, process *p)
{
	(void)v;
	if (!p->carry)
	{
		p->pc += 2 + sizeof(reg);
		return true;
	}
	p->pc = (p->begin + *(reg*)&instruction[2]) % MEM_SIZE;
	return true;
}

static bool	sub(cell *instruction, vm *v, process *p)
{
	(void)v;
	for (int i = 2; i < 5; ++i)
		if (!is_reg(instruction[i]))
			return false;
	p->registers[instruction[4] - 1] = p->registers[instruction[3] - 1]
										- p->registers[instruction[2] - 1];
	p->pc += 5;
	p->carry = p->registers[instruction[4] - 1] != 0;
	return true;
}

static bool	add(cell *instruction, vm *v, process *p)
{
	(void)v;
	for (int i = 2; i < 5; ++i)
		if (!is_reg(instruction[i]))
			return false;
	p->registers[instruction[4] - 1] = p->registers[instruction[3] - 1]
										+ p->registers[instruction[2] - 1];
	p->pc += 5;
	p->carry = p->registers[instruction[4] - 1] != 0;
	return true;
}

static bool	lld(cell *instruction, vm *v, process *p)
{
	uint8_t	params[3];
	uint8_t	size[3];

	get_params(params, instruction[1]);
	get_size_params(size, instruction[1]);
	if (!params[0] || !is_reg(instruction[2 + size[0]]))
		return false;
	reg	tmp = p->registers[instruction[2 + size[0]] - 1];
	tmp = get_data(false, 1, instruction, v, p);
	p->registers[instruction[2 + size[0]] - 1] = tmp;
	p->pc += (3 + size[0]);
	p->carry = tmp != 0;
	return true;
}

static bool	ld(cell *instruction, vm *v, process *p)
{
	uint8_t	params[3];
	uint8_t	size[3];

	get_params(params, instruction[1]);
	get_size_params(size, instruction[1]);
	if (!params[0] || !is_reg(instruction[2 + size[0]]))
		return false;
	reg	tmp = p->registers[instruction[2 + size[0]] - 1];
	tmp = get_data(true, 1, instruction, v, p);
	p->registers[instruction[2 + size[0]] - 1] = tmp;
	p->pc += (3 + size[0]);
	p->carry = tmp != 0;
	return true;
}

static bool	lfork(cell *instruction, vm *v, process *p)
{
	reg	addr = get_data(false, 1, instruction, v, p);
	uint8_t	size[3];

	copy_process(v, p, addr);
	get_size_params(size, instruction[1]);
	p->pc += (2 + size[0]);
	return true;
}

bool	execute_op(vm *v, process *p, cell *instruction)
{
	switch (instruction[0])
	{
		case ZJMP:
			return zjmp(instruction, v, p);
			break;
		case SUB:
			return sub(instruction, v, p);
			break;
		case ADD:
			return add(instruction, v, p);
			break;
		case LD:
			return ld(instruction, v, p);
			break;
		case LLD:
			return lld(instruction, v, p);
			break;
		case OR:
			return or(instruction, v, p);
			break;
		case AND:
			return and(instruction, v, p);
			break;
		case XOR:
			return xor(instruction, v, p);
			break;
		case LFORK:
			return lfork(instruction, v, p);
			break;
	}
	return false;
}
