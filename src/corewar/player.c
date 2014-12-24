#include "player.h"
#include <stdlib.h>
#include <stdio.h>

player	*load_player(const char *filename)
{
	player	*tmp;
	FILE	*f;

	f = fopen(filename, "rb");
	if (!f)
		return 0;
	tmp = malloc(sizeof(*tmp));
	fread(tmp, sizeof(*tmp), 1, f);
	fclose(f);
	return tmp;
}

void	delete_player(player **p)
{
	if (!p || !*p)
		return;
	free(*p);
	*p = 0;
}
