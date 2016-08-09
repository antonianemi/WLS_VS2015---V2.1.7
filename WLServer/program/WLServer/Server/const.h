/*
 * const.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef CONST_H_
#define CONST_H_

#define FILE_LOG				PATH_LOG "log.txt"						//log file
#define FILE_CONFIG				PATH_DATA "config.txt"					//configuration file
#define FILE_MENU				PATH_DATA "menu.txt"					//menu file
#define FILE_SPANISH			PATH_LENG "spanish.txt"					//spanish file
#define FILE_ENGLISH			PATH_LEND "english.txt"					//english file

#define DISPLAY_WIDTH			640									//width screen
#define DISPLAY_HEIGHT			480									//height screen

#define MAX_SCALES				100									//maximo numero de basculas
#define MAX_AGENTS				100									//maximo numero de vendedores
#define MAX_TICKETS				100									//maximo numero de tickets
#define MAX_TRANSACTIONS			100									//maximo numero de tickets
#define MAX_DETAIL				MAX_TICKETS*MAX_TRANSACTIONS				//maximo numero de lineas

#define MAX_TCP_CLIENTS			MAX_SCALES							//maximo numero de clientes tcp
#define MAX_SOCKETS				MAX_TCP_CLIENTS + 1						//maximo numero de sockets tcp
#define MAX_EVENTS				MAX_TCP_CLIENTS + 1						//maximo numero de eventos
#define SOCKET_ERRNO			WSAGetLastError()						//funcion de error
#define MAX_BUFFERSIZE			5000									//tamaño de buffer tcp socket
#define BUFFERSIZE				1500									//tamaño de buffer tcp socket
#define TCP_TRYS_RECEIVED		3									//maximo numero de ciclos sin respuesta del clients
#define TCP_OK					"Ok"									//ok request
#define TCP_ERROR				"Error"								//error request
#define TCP_END				"End"								//end request

#define SEMAPHORE_TIMEOUT		20									//2 seconds

#define DBASE					PATH_DATA "database.db"					//dtabase file
#define TABLE_AGENTS			"agents"								//agents table
#define TABLE_SCALES			"scales"								//scales table
#define TABLE_TICKETS			"ticket"								//tickets table
#define TABLE_DETAIL			"detail"								//detail table
#define SQL					sqlite3								//type sqlite
#define SQLSTMT				sqlite3_stmt							//type sqlite stament
#define SQL_READONLY			SQLITE_OPEN_READONLY					//read only mode
#define SQL_READWRITE			SQLITE_OPEN_READWRITE					//read & write mode
#define SQL_OK					SQLITE_OK								//ok answerd from a cmd
#define SQL_ERROR				SQLITE_ERROR							//error answerd from a cmd
#define SQL_DONE				SQLITE_DONE							//done answerd from a cmd
#define SQL_ROW				SQLITE_ROW							//row answerd from a cmd
#define SQL_MAX_CMD_LEN			256									//maxim sql command length
#define SQL_MAX_DATA_LEN			1512									//maxim sql data length

#define SQL_TYPE_NULL			0									//sql null data
#define SQL_TYPE_ID				1									//sql id data
#define SQL_TYPE_INT			2									//sql int data
#define SQL_TYPE_LONG			3									//sql long data
#define SQL_TYPE_FLOAT			4									//sql float data
#define SQL_TYPE_TEXT			5									//sql string data
#define SQL_TYPE_EMPTY			6									//sql empty data

#define TYPE_NULL				0									//sql data length 1
#define TYPE_BYTE				1									//sql data length 1
#define TYPE_INT				2									//sql data length 2
#define TYPE_LONG				4									//sql data length 4
#define TYPE_FLOAT				5									//sql data length 4
#define TYPE_CHAR16				16									//sql data length 16
#define TYPE_CHAR32				32									//sql data length 32
#define TYPE_CHAR64				64									//sql data length 64
#define TYPE_CHAR128			128									//sql data length 128
#define TYPE_CHAR256			256									//sql data length 256

#endif /* CONST_H */