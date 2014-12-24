#include "op.h"
#include <string.h>

uint32_t	get_time(cell op)
{
	if (!op)
		return 1;
	else if (op == ADD || op == SUB)
		return 4;
	return 1;
}

static uint32_t	get_offset(uint8_t param, cell *instruction)
{
	uint32_t	total = 2;

	for (int i = 0; i < param - 1; ++i)
		switch (decode_parameters(instruction[1], i + 1))
		{
			case REG_PARAM:
				total += REG_SIZE;
				break;
			case DIR_PARAM:
				total += DIR_SIZE;
				break;
			case IND_PARAM:
				total += IND_SIZE;
				break;
		}
	return total;
}

static uint32_t	get_size(cell *instruction)
{
	uint32_t	total = get_offset(3, instruction);

	switch (decode_parameters(instruction[1], 3))
	{
		case REG_PARAM:
			total += REG_SIZE;
			break;
		case DIR_PARAM:
			total += DIR_SIZE;
			break;
		case IND_PARAM:
			total += IND_SIZE;
			break;
	}
	return total;
}

static cell	*get_mem(bool relative, reg instruction, vm *v, process *p)
{
	uint32_t	rel = relative ? p->begin : 0;

	while ((int32_t)rel + instruction < 0)
		instruction += MEM_SIZE;
	instruction += rel;
	return &v->mem[(instruction) % MEM_SIZE];
}

static void	copy_mem_to_other(vm *v, cell *dest, cell *src, uint32_t size)
{
	uint32_t	pos = src - v->mem;
	for (uint32_t i = 0; i < size; ++i)
		dest[i] = v->mem[(pos + i) % MEM_SIZE];
}

static void	copy_other_to_mem(vm *v, cell *dest, cell *src, uint32_t size)
{
	uint32_t	pos = dest - v->mem;
	for (uint32_t i = 0; i < size; ++i)
		v->mem[(pos + i) % MEM_SIZE] = src[i];
}

/* static void	copy_mem_to_mem(vm *v, cell *dest, cell *src, uint32_t size) */
/* { */
	/* uint32_t	pos1 = src - v->mem; */
	/* uint32_t	pos2 = dest - v->mem; */
	/* for (uint32_t i = 0; i < size; ++i) */
	/* 	v->mem[(pos2 + i) % MEM_SIZE] = v->mem[(pos1 + i) % MEM_SIZE]; */
/* } */

static t_param	get_param(uint8_t param, cell *instruction, vm *v, process *p)
{
	t_param	tmp = {0, 0, 0, 0, 0};

	tmp.type = decode_parameters(instruction[1], param);
	if (tmp.type == REG_PARAM && is_reg(instruction[get_offset(param, instruction)]))
	{
		tmp.size = REG_SIZE;
		tmp.ptr = (cell*)&p->registers[instruction[get_offset(param, instruction)] - 1];
		tmp.value = *(reg*)tmp.ptr;
	}else if (tmp.type == DIR_PARAM)
	{
		tmp.size = DIR_SIZE;
		tmp.ptr = get_mem(false, p->pc + get_offset(param, instruction), v, p);
		copy_mem_to_other(v, (cell*)&tmp.value, tmp.ptr, tmp.size);
	}else if (tmp.type == IND_PARAM)
	{
		tmp.size = IND_SIZE;
		tmp.ptr = get_mem(true, instruction[get_offset(param, instruction)], v, p);
		copy_mem_to_other(v, (cell*)&tmp.value, tmp.ptr, tmp.size);
	}
	tmp.v = v;
	return tmp;
}

static void	set_param(t_param *param, reg value)
{
	switch (param->type)
	{
		case REG_PARAM:
			*(reg*)param->ptr = value;
			break;
		case DIR_PARAM:
		case IND_PARAM:
			copy_other_to_mem(param->v, param->ptr, (cell*)&value, param->size);
			break;
	}
	param->value = value;
}

static bool	sti(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[0].type != REG_PARAM)
		return false;
	set_param(&params[0], params[1].value + params[2].value);
	p->pc += get_size(instruction);
	return true;
}

static bool	ldi(cell *instruction, vm *v, process *p)
{
	t_param	params[3];
	reg		value;

	for (int i = 0; i < 3; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[2].type != REG_PARAM)
		return false;
	copy_mem_to_other(v, (cell*)&value, get_mem(true, params[0].value + params[1].value, v, p), REG_SIZE);
	set_param(&params[2], value);
	p->pc += get_size(instruction);
	return true;
}

static bool	xor(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[2].type != REG_PARAM)
		return false;
	set_param(&params[2], params[0].value ^ params[1].value);
	p->carry = (params[0].value ^ params[1].value) != 0;
	p->pc += get_size(instruction);
	return true;
}

static bool	and(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[2].type != REG_PARAM)
		return false;
	set_param(&params[2], params[0].value & params[1].value);
	p->carry = (params[0].value & params[1].value) != 0;
	p->pc += get_size(instruction);
	return true;
}

static bool	or(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[2].type != REG_PARAM)
		return false;
	set_param(&params[2], params[0].value | params[1].value);
	p->carry = (params[0].value | params[1].value) != 0;
	p->pc += get_size(instruction);
	return true;
}

static bool	zjmp(cell *instruction, vm *v, process *p)
{
	(void)v;
	if (!p->carry)
	{
		p->pc += get_size(instruction);
		return true;
	}
	while ((int32_t)p->begin + *(reg*)&instruction[2] < 0)
		*(reg*)&instruction[2] += MEM_SIZE;
	p->pc = (p->begin + *(reg*)&instruction[2]) % MEM_SIZE;
	return true;
}

static bool	sub(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
	{
		params[i] = get_param(i + 1, instruction, v, p);
		if (params[i].type != REG_PARAM)
			return false;
	}
	set_param(&params[2], params[1].value - params[0].value);
	p->pc += get_size(instruction);
	p->carry = params[2].value != 0;
	return true;
}

static bool	add(cell *instruction, vm *v, process *p)
{
	t_param	params[3];

	for (int i = 0; i < 3; ++i)
	{
		params[i] = get_param(i + 1, instruction, v, p);
		if (params[i].type != REG_PARAM)
			return false;
	}
	set_param(&params[2], params[1].value + params[0].value);
	p->pc += get_size(instruction);
	p->carry = params[2].value != 0;
	return true;
}

static bool	lld(cell *instruction, vm *v, process *p)
{
	(void)instruction;
	(void)v;
	(void)p;
	return false;
}

static bool	ld(cell *instruction, vm *v, process *p)
{
	t_param	params[2];

	for (int i = 0; i < 2; ++i)
		params[i] = get_param(i + 1, instruction, v, p);
	if (params[1].type != REG_PARAM)
		return false;
	set_param(&params[1], params[0].value);
	p->carry = params[1].value != 0;
	p->pc += get_size(instruction);
	return true;
}

static bool	lfork(cell *instruction, vm *v, process *p)
{
	t_param	param = get_param(1, instruction, v, p);

	copy_process(v, p, param.value);
	p->pc += get_size(instruction);
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
		case STI:
			return sti(instruction, v, p);
			break;
		case LDI:
			return ldi(instruction, v, p);
			break;
	}
	return false;
}
