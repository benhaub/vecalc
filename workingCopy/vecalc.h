/*
 *======================================================================//
 * Author	:	Ben Haubrich					//
 * File		:	vecalc.h					//
 * Synopsis	:	Contains the defnition of a Vector and it's	//
 * 		:	elements					//
 *======================================================================//
 */

#ifndef _VECALC_H_
#define _VECALC_H_

#define MAXVECSIZE 65536

typedef float Elem;

struct Vector {

	int size;
	Elem *elements;
};

#endif /*_VECALC_H_*/
