#ifndef _OP_H_
# define _OP_H_

#include <stdbool.h>
#include "instructions.h"
#include "vm.h"

bool	execute_op(vm *v, process *p, cell *instruction);

#endif /* !_OP_H_ */
