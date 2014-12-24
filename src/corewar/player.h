#ifndef _PLAYER_H_
# define _PLAYER_H_

#include "machine.h"

struct	player
{
	struct header	h;
	cell			prog[CHAMP_MAX_SIZE];
}__attribute__((__packed__));

typedef struct player player;

player	*load_player(const char *addr);
void	delete_player(player **p);

#endif /* !_PLAYER_H_ */
