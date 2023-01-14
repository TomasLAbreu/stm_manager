#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <stdint.h>
#include "fixed_point.h"

/******************************************************************************

******************************************************************************/
//typedef struct
//{
//	int M;
//	int N;
//
//	float *x_ant;
//	float *y_ant;
//	float const *y_coefs;
//	float const *x_coefs;
//
//	uint32_t dc;	// dc component
//	uint8_t status;
//} filter_t;

typedef struct
{
	int M;
	int N;

	fixed_point_t *x_ant;
	fixed_point_t *y_ant;
	fixed_point_t const *y_coefs;
	fixed_point_t const *x_coefs;

	fixed_point_t dc;	// dc component
	uint8_t status;
} filter_t;
/******************************************************************************
Function Prototypes
******************************************************************************/

extern char filter_init(filter_t *ft);
extern char filter_kill(filter_t *ft);
//extern float filter_calc(filter_t *ft, uint32_t x);
extern int filter_calc(filter_t *ft, fixed_point_t x);

#endif // !__FILTERS_H__
