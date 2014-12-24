#include "vm.h"
#include <stdio.h>

int main()
{
	vm	*v = init_vm(1);
	v->mem[0] = LFORK;
	v->mem[1] = encode_parameters(DIR_PARAM, 0, 0);
	v->mem[2] = 6;
	v->mem[6] = LD;
	v->mem[7] = encode_parameters(DIR_PARAM, REG_PARAM, 0);
	v->mem[8] = 5;
	v->mem[12] = 4;
	v->mem[13] = LD;
	v->mem[14] = encode_parameters(DIR_PARAM, REG_PARAM, 0);
	v->mem[15] = 1;
	v->mem[19] = 2;
	v->mem[20] = ADD;
	v->mem[21] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[22] = 1;
	v->mem[23] = 2;
	v->mem[24] = 1;
	v->mem[25] = LD;
	v->mem[26] = encode_parameters(REG_PARAM, REG_PARAM, 0);
	v->mem[27] = 1;
	v->mem[28] = 3;
	v->mem[29] = SUB;
	v->mem[30] = encode_parameters(REG_PARAM, REG_PARAM, REG_PARAM);
	v->mem[31] = 4;
	v->mem[32] = 3;
	v->mem[33] = 3;
	v->mem[34] = ZJMP;
	v->mem[35] = 0;
	v->mem[36] = 20;
	add_process(v, 0, 0);
	while (execute_step_vm(v))
	{
		char c;
		scanf("%c", &c);
	}
	delete_vm(&v);
	return 0;
}
