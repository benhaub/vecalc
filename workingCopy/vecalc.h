/*
 *======================================================================//
 * Author	:	Ben Haubrich					//
 * File		:	vecalc.h					//
 * Synopsis	:	Contains the defnition of a Vector for the	//
 *======================================================================//
 */

#ifndef _VECALC_H_
#define _VECALC_H_

typedef float Elem;

struct Vector {

	int size;
	Elem *elements;
};

#endif /*_VECALC_H_*/
