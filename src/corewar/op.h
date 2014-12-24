#ifndef _OP_H_
# define _OP_H_

#include <stdbool.h>
#include "instructions.h"
#include "vm.h"

#define get_param(a) (*(reg*)&(a))

bool		execute_op(vm *v, process *p, cell *instruction);
uint32_t	get_time(cell op);

#endif /* !_OP_H_ */
