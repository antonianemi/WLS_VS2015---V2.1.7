/*
 * extern.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */

#ifndef EXTERN_H_
#define EXTERN_H_


//standar libraries 
#include <stdio.h>													//standar I/O functions
#include <stdlib.h>													//standar functions
#include <string.h>													//string functions
#include <time.h>													//time functions
#include <math.h>													//math functions
#include <vector>													//dynamic arrays fuctions

//sql libs
#include <SQLite/sqlite3.h>											//libreria sql

//user libraries
#include "os.h"													//os definitions & functions
#include "const.h"													//const definitions & functions
#include "struct.h"													//struct definitions & functions
#include "config.h"													//config definitions & functions
#include "server.h"													//server definitions & functions
#include "client.h"													//client definitions & functions
#include "command.h"												//command definitions & functions
#include "process.h"												//database definitions & functions
#include "sql.h"													//sql definitions & functions
#include "misc.h"													//misc definitions & functions
#include "log.h"													//log definitions & functions
#include "timer.h"													//timer definitions & functions
#include "semaphore.h"												//semaphore definitions & functions


//globals vars
extern TCPSERVER Server;												//server socket
extern TCPCLIENT Clients[MAX_TCP_CLIENTS];								//clients sockets
extern SCALE  Scales[MAX_SCALES];										//vector de basculas
//extern AGENT  Agents[MAX_AGENTS];									//vector de vendedores
//extern TICKET Tickets[MAX_TICKETS];									//vector de vectores
extern DETAIL Detail[MAX_DETAIL];										//vector de detalle
extern CONFIG Config;												//configuracion general
extern APP    App;													//info applicacion

//char definitions
extern char Msg[SQL_MAX_DATA_LEN];										//log msg

//long
extern long LockSQL;												//lock sql database 

//global functions
extern void ServerStart(void);
extern int ServerStartStop(int);
extern void ApplicationExit(void);



#endif /* EXTERN_H */