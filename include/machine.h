
#ifndef _MACHINE_H_
# define _MACHINE_H_

#include <stdint.h>

#define MAX_ARGS_NUMBER	4
#define MAX_PLAYERS		4
#define	MEM_SIZE		(4 * 1024)
#define IDX_MOD			(MEM_SIZE / 8)
#define CHAMP_MAX_SIZE	(MEM_SIZE / 6)

#define	REG_NUMBER		16
#define CYCLE_TO_DIE	1536
#define CYCLE_DELTA		50
#define NBR_LIVE		21
#define MAX_CHECKS		10

#define T_REG			1
#define T_DIR			2
#define T_IND			4
#define T_LAB			8

#define PROG_NAME_LENGTH	128
#define COMMENT_LENGTH		2048

struct	header
{
	char		prog_name[PROG_NAME_LENGTH + 1];
	uint32_t	prog_size;
	char		comment[COMMENT_LENGTH + 1];
};

#endif /* !_MACHINE_H_ */
