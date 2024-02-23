/*
 * teste.cpp
 *
 *  Created on: 16 de abr de 2022
 *      Author: alexandre
 */
#include"oc.h"

int main(){
	Octree* pai = NULL;
	Octree* oct = new Octree(0,0,0,8,8,8,pai,0.5,3);
	oct->imprime_octree(oct);
	return 0;
}
