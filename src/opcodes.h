#ifndef _OPCODES_H_
#define _OPCODES_H_

#define OPCODE_CLS(op)            (((op) & 0xFFFF) == 0x00E0)
#define OPCODE_RETURN(op)         (((op) & 0xFFFF) == 0x00EE)
#define OPCODE_JUMP(op)           (((op) & 0xF000) == 0x1000)
#define OPCODE_CALL(op)           (((op) & 0xF000) == 0x2000)
#define OPCODE_SKIP_EQ(op)        (((op) & 0xF000) == 0x3000)
#define OPCODE_SKIP_NEQ(op)       (((op) & 0xF000) == 0x4000)
#define OPCODE_SKIP_EQ_REG(op)    (((op) & 0xF00F) == 0x5000)
#define OPCODE_MOV_VAL(op)        (((op) & 0xF000) == 0x6000)
#define OPCODE_ADD_VAL(op)        (((op) & 0xF000) == 0x7000)
#define OPCODE_MOV_REG(op)        (((op) & 0xF00F) == 0x8000)
#define OPCODE_OR(op)             (((op) & 0xF00F) == 0x8001)
#define OPCODE_AND(op)            (((op) & 0xF00F) == 0x8002)
#define OPCODE_XOR(op)            (((op) & 0xF00F) == 0x8003)
#define OPCODE_ADD_REG(op)        (((op) & 0xF00F) == 0x8004)
#define OPCODE_SUB_REG(op)        (((op) & 0xF00F) == 0x8005)
#define OPCODE_LSR(op)            (((op) & 0xF00F) == 0x8006)
#define OPCODE_SUBN(op)           (((op) & 0xF00F) == 0x8007)
#define OPCODE_LSL(op)            (((op) & 0xF00F) == 0x800E)
#define OPCODE_SKIP_REG_NEQ(op)   (((op) & 0xF00F) == 0x9000)
#define OPCODE_MOV_I(op)          (((op) & 0xF000) == 0xA000)
#define OPCODE_JUMP_REL(op)       (((op) & 0xF000) == 0xB000)
#define OPCODE_RAND(op)           (((op) & 0xF000) == 0xC000)
#define OPCODE_DRAW(op)           (((op) & 0xF000) == 0xD000)
#define OPCODE_SKIP_KP(op)        (((op) & 0xF0FF) == 0xE09E)
#define OPCODE_SKIP_NKP(op)       (((op) & 0xF0FF) == 0xE0A1)
#define OPCODE_GET_DELAY(op)      (((op) & 0xF0FF) == 0xF007)
#define OPCODE_INPUT(op)          (((op) & 0xF0FF) == 0xF00A)
#define OPCODE_SET_DELAY(op)      (((op) & 0xF0FF) == 0xF015)
#define OPCODE_SET_SOUND(op)      (((op) & 0xF0FF) == 0xF018)
#define OPCODE_ADD_I(op)          (((op) & 0xF0FF) == 0xF01E)
#define OPCODE_FONT(op)           (((op) & 0xF0FF) == 0xF029)
#define OPCODE_BCD(op)            (((op) & 0xF0FF) == 0xF033)
#define OPCODE_STR(op)            (((op) & 0xF0FF) == 0xF055)
#define OPCODE_LDR(op)            (((op) & 0xF0FF) == 0xF065)

#endif
