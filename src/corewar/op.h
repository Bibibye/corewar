#ifndef _OP_H_
# define _OP_H_

#include <stdbool.h>
#include "instructions.h"
#include "vm.h"

#define is_reg(a) ((a) - 1 >= 0 && (a) - 1 < REG_NUMBER)

typedef struct	s_param
{
	uint8_t	type;
	uint8_t	size;
	reg		value;
	cell	*ptr;
	vm		*v;
}t_param;

bool		execute_op(vm *v, process *p, cell *instruction);
uint32_t	get_time(cell op);

#endif /* !_OP_H_ */
