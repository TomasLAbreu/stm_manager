#ifndef __FIXED_POINT_H__
#define __FIXED_POINT_H__

typedef uint16_t fixed_point_t;

#define FIXED_POINT_LEN		16
#define FRACTION_BITS 		15

//#define IS_NEGATIVE(_v_) (((_v_) >> (FIXED_POINT_LEN-1)) & 0x01)

#endif // !__FIXED_POINT_H__
