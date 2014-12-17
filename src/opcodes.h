#ifndef _OPCODES_H_
#define _OPCODES_H_

#define OPCODE_RETURN(op)       (((op) & 0xFFFF) == 0x00EE)
#define OPCODE_JUMP(op)         (((op) & 0xF000) == 0x1000)
#define OPCODE_CALL(op)         (((op) & 0xF000) == 0x2000)
#define OPCODE_SKIP_EQ(op)      (((op) & 0xF000) == 0x3000)
#define OPCODE_SKIP_NEQ(op)     (((op) & 0xF000) == 0x4000)
#define OPCODE_SKIP_CMP(op)     (((op) & 0xF00F) == 0x5000)
#define OPCODE_MOV_VAL(op)      (((op) & 0xF000) == 0x6000)
#define OPCODE_ADD_VAL(op)      (((op) & 0xF000) == 0x7000)
#define OPCODE_MOV_REG(op)      (((op) & 0xF00F) == 0x8000)
#define OPCODE_AND_REG(op)      (((op) & 0xF00F) == 0x8002)
#define OPCODE_ADD_REG(op)      (((op) & 0xF00F) == 0x8004)
#define OPCODE_SUB_REG(op)      (((op) & 0xF00F) == 0x8005)
#define OPCODE_MOV_I(op)        (((op) & 0xF000) == 0xA000)
#define OPCODE_RAND(op)         (((op) & 0xF000) == 0xC000)
#define OPCODE_DRAW(op)         (((op) & 0xF000) == 0xD000)
#define OPCODE_SKIP_KEY_UP(op)  (((op) & 0xF0FF) == 0xE0A1)
#define OPCODE_GET_DELAY(op)    (((op) & 0xF0FF) == 0xF007)
#define OPCODE_SET_DELAY(op)    (((op) & 0xF0FF) == 0xF015)
#define OPCODE_SET_SOUND(op)    (((op) & 0xF0FF) == 0xF018)
#define OPCODE_FONT(op)         (((op) & 0xF0FF) == 0xF029)
#define OPCODE_BCD(op)          (((op) & 0xF0FF) == 0xF033)
#define OPCODE_LDR(op)          (((op) & 0xF0FF) == 0xF065)

#endif