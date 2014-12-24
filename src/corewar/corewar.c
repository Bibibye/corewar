#include "vm.h"
#include "player.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	vm	*v = init_vm(1);
	if (argc < 2)
		return 0;
	player	*tmp = load_player(argv[1]);
	memcpy(v->mem, tmp->prog, sizeof(tmp->prog));
	printf("name : %s\ncomment : %s\n", tmp->h.prog_name, tmp->h.comment);
	delete_player(&tmp);
	add_process(v, 0, 0);
	while (execute_step_vm(v));
	delete_vm(&v);
	return 0;
}
