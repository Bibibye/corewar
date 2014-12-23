#include "vm.h"
#include <stdio.h>

int main()
{
	vm	*v = init_vm(1);
	v->mem[0] = LD;
	v->mem[1] = encode_parameters(DIR_PARAM, REG_PARAM, 0);
	v->mem[2] = 5;
	v->mem[6] = 4;
	v->mem[7] = LD;
	v->mem[8] = encode_parameters(DIR_PARAM, REG_PARAM, 0);
	v->mem[9] = 1;
	v->mem[13] = 2;
	v->mem[14] = ADD;
	v->mem[15] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[16] = 1;
	v->mem[17] = 2;
	v->mem[18] = 1;
	v->mem[19] = LD;
	v->mem[20] = encode_parameters(REG_PARAM, REG_PARAM, 0);
	v->mem[21] = 1;
	v->mem[22] = 3;
	v->mem[23] = SUB;
	v->mem[24] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[25] = 4;
	v->mem[26] = 3;
	v->mem[27] = 3;
	v->mem[28] = ZJMP;
	v->mem[29] = 0;
	v->mem[30] = 7;
	add_process(v, 0);
	dump_process(v->procs->proc);
	while (execute_step_vm(v))
	{
		dump_process(v->procs->proc);
		char c;
		scanf("%c", &c);
	}
	delete_vm(&v);
	return 0;
}
