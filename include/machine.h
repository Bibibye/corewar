
#ifndef _MACHINE_H_
# define _MACHINE_H_

#include <stdint.h>

typedef int32_t			reg;
typedef int8_t			cell;

#define MAX_ARGS_NUMBER	4
#define MAX_PLAYERS		4
#define	MEM_SIZE		(4 * 1024)
#define CHAMP_MAX_SIZE	(MEM_SIZE / 6)

#define	REG_NUMBER		16
#define CYCLE_TO_DIE	1536

#define PROG_NAME_LENGTH	128
#define COMMENT_LENGTH		2048

struct	header
{
	char		prog_name[PROG_NAME_LENGTH + 1];
	uint32_t	prog_size;
	char		comment[COMMENT_LENGTH + 1];
}__attribute((packed))__;

#endif /* !_MACHINE_H_ */
