/*
 * bitarray.h
 *
 *  Created on: 28 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_BITARRAY_H_
#define SRC_BITARRAY_H_
#include "gamecard.h"

sem_t* semBitmap;

// BITARRAY
void printearBitArray();
void guardarBitArrayEnArchivo(char* , char* , int );
void leerBitArrayDeArchivo(char* , char** , int );
void vaciarBitArray(t_bitarray* , int );
void liberarUnBloque(int);
t_list* obtenerPrimerosLibresDeBitmap(int);
t_bitarray* crearBitArray(char* , int );

#endif /* SRC_BITARRAY_H_ */
