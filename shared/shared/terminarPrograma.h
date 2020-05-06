/*
 * terminarPrograma.h
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#ifndef SHARED_TERMINARPROGRAMA_H_
#define SHARED_TERMINARPROGRAMA_H_

#include"commons/log.h"
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include"socket.h"

//Usar para liberar recursos.
void matarPrograma(t_log*, t_config*, int);


#endif /* SHARED_TERMINARPROGRAMA_H_ */