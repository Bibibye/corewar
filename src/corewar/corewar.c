#include "vm.h"

int main()
{
	vm	*v = init_vm(1);
	add_process(v, 0, 0);
	while (execute_step_vm(v));
	delete_vm(&v);
	return 0;
}
