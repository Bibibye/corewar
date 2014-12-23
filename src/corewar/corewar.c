#include "vm.h"
#include <stdio.h>

int main()
{
	vm	*v = init_vm(1);
	v->mem[0] = ADD;
	v->mem[1] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[2] = 1;
	v->mem[3] = 1;
	v->mem[4] = 1;
	add_process(v, 0);
	v->procs->proc->registers[0] = 1;
	dump_process(v->procs->proc);
	while (execute_step_vm(v))
		dump_process(v->procs->proc);
	delete_vm(&v);
	return 0;
}
