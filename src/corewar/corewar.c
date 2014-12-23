#include "vm.h"
#include <stdio.h>

void	print_state(process *p)
{
	for (int i = 0; i < REG_NUMBER; ++i)
		printf("reg %d : %d\n", i, p->registers[i]);
	printf("pc : %d\n", p->pc);
}

int main()
{
	vm	*v = init_vm(1);
	add_process(v, 0);
	v->mem[0] = ADD;
	v->mem[1] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[2] = 1;
	v->mem[3] = 1;
	v->mem[4] = 1;
	v->procs->proc->registers[1] = 1;
	print_state(v->procs->proc);
	execute_step_vm(v);
	print_state(v->procs->proc);
	delete_vm(&v);
	return 0;
}
