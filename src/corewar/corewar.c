#include "vm.h"
#include <stdio.h>

int main()
{
	vm	*v = init_vm(1);
	v->mem[0] = LD;
	v->mem[1] = encode_parameters(DIR_PARAM, REG_PARAM, 0);
	v->mem[2] = 1;
	v->mem[6] = 2;
	v->mem[7] = ADD;
	v->mem[8] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[9] = 1;
	v->mem[10] = 2;
	v->mem[11] = 1;
	add_process(v, 0);
	dump_process(v->procs->proc);
	while (execute_step_vm(v))
		dump_process(v->procs->proc);
	delete_vm(&v);
	return 0;
}
