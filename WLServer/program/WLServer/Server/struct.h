/*
 * struct.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef STRUCT_H_
#define STRUCT_H_

//scales struct
typedef struct SCALE{
	long Id;														//id
	char Serie[TYPE_CHAR16];											//numero de series de la bascula	
	long Status;													//status 0 = inactivo, 1 = activo
	char Time[TYPE_CHAR32];											//ultimo registro		
	long Agent;													//id agent
	char IP[TYPE_CHAR16];											//ip de báscula		
}SCALE_STRUCT;

//agents struct
typedef struct AGENT{
	long Id;														//id
	char Name[TYPE_CHAR64];											//nombre del vendedor
	long Status;													//status 0 = inactivo, 1 = activo
	char Time[TYPE_CHAR32];											//ultimo registro		
}AGENT_STRUCT;

//ticket struct
typedef struct TICKET{	
	long Id;														//id
	char Content[TYPE_CHAR128];										//nombre del vendedor
	long Status;													//status 0 = inactivo, 1 = activo
	char Time[TYPE_CHAR32];											//ultimo registro		
}TICKET_STRUCT;

//detail struct
typedef struct DETAIL{	
	long Id;														//id	
	char Content[TYPE_CHAR128];										//nombre del vendedor
	long Status;													//status 0 = inactivo, 1 = activo
	char Time[TYPE_CHAR32];											//ultimo registro		
	long Line;													//line
}DETAIL_STRUCT;

//config struct
typedef struct CONFIG{	
	char IP[TYPE_CHAR16];											//ip server
	int  ServerPort;												//port
	int  ClientPort;												//port
	int  Lenguage;													//0 = ingles, 1 = español	
	int  TimeoutScale;												//time out scale
	int  TimeoutAgent;												//time out vendedor
	int	RefresScales;												//refresh scales
	int	RefresTickets;												//refresh Tickets
	int  RestartTables;												//start with empty tables
}CONFIG_STRUCT;

//app struct
typedef struct APP{	
	char Root[TYPE_CHAR128];											//ruta de la applicacion
	char Name[TYPE_CHAR64];											//nombre de la aplicacion	
	long Width;													//ancho de display
	long Height;													//altura de display
}APP_STRUCT;

//client struct
typedef struct TCPCLIENT{	
	HANDLE Thread;													//handle thread
	SOCKET Socket;													//socket
	char   IP[TYPE_CHAR16];											//ip
	char   Msg[TYPE_CHAR64];											//msg
	int    Active;													//status 0 = inactivo, 1 = activo
}TCPCLIENT_STRUCT;

//server struct
typedef struct TCPSERVER{		
	HANDLE    Thread;												//handle thread
	SOCKET    Socket;												//socket	
	TCPCLIENT *Clients;												//clients
	char		IP[TYPE_CHAR16];										//ip
	long		Port;												//port to connect
	int		Active;												//status 0 = inactivo, 1 = activo
	int		MaxClients;											//number maxim of clients
}TCPSERVER_STRUCT;


#endif /* STRUCT_H */
