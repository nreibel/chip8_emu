#ifndef BITS_H_
#define BITS_H_

#ifdef DEBUG
#define DBG_PRINT(str) printf str;
#else
#define DBG_PRINT(str) {;}
#endif

#define NULL_PTR (void*) 0

#define MASK(val, mask, shift) (((val) & (mask)) >> (shift))

#define BIT(val, bit) ((val) & (1 << (bit)))

#define INC(x, m) x = (x + 1) % m;
#define DEC(x, m) x = (x + m - 1) % m;

#endif
