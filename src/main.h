#ifndef _MAIN_H_
#define _MAIN_H_

#define MASK(val, mask, shift) (((val) & (mask)) >> (shift))

#ifdef DEBUG
#define DBG_PRINT(str) printf str;
#else
#define DBG_PRINT(str) {;}
#endif

void Reset();

#endif
