#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#define LFORK	0x01
#define STI	0x02
#define FORK	0x03
#define LLD	0x04
#define LD	0x05
#define ADD	0x06
#define ZJMP	0x07
#define SUB	0x08
#define LDI	0x09
#define OR	0x0A
#define ST	0x0B
#define AFF	0x0C
#define LIVE	0x0D
#define XOR	0x0E
#define LLDI	0x0F
#define AND	0x10

#define REG_PARAM 0b01
#define DIR_PARAM 0b10
#define IND_PARAM 0b11

#define encode_parameters(p1, p2, p3) ((p1) << 2 | (p2) << 4 | (p3) << 6)
#define decode_parameters(code, p) (((code) >> (p)*2) & 0b00000011)

#define OP_SIZE  1
#define REG_SIZE 1
#define DIR_SIZE 4
#define IND_SIZE DIR_SIZE

#endif /* _INSTRUCTIONS_H_ */
