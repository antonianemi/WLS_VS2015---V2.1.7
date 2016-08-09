/*------------------------------------------------------------------------------*
 * server.c	 													*
 * 		       	     FABRICANTES DE BÁSCULAS TORREY					*
 * 						SERVIDOR ETHERNET							*
 *------------------------------------------------------------------------------*
 *																*
 *												Gregory Garcia		*
 * 														      	*
 *-----------------------------------------------------------------------------*/

#include "extern.h"

/*------------------------------------------------------------------------------*
Star Code
-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
TCPOpenServerSocket
-------------------------------------------------------------------------------*/
int TCPOpenServer(TCPSERVER *serv,TCPCLIENT *clients,int n,char *ip, long port)//Open server socket
{
WSADATA wsaData;													//wsadata struct
struct sockaddr_in addr;												//address struct
memset(serv,0,sizeof(TCPSERVER));										//clear server obj
strcpy(serv->IP,ip);												//set ip
serv->MaxClients = n;												//maxim number of clients
serv->Clients = clients;												//pointer to array of clients
serv->Port = port;													//set port
addr.sin_family = AF_INET ;											//set kind of connexion
addr.sin_addr.s_addr = inet_addr(serv->IP);								//set local ip address
addr.sin_port = htons ((u_short)serv->Port);								//set port number
if(WSAStartup(MAKEWORD(2,0), &wsaData)!=NULL) return (1);					//Initialize Winsock 
if((serv->Socket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)return(2);//Create socket 
if(bind(serv->Socket,(struct sockaddr *)&addr,sizeof(addr))==SOCKET_ERROR)return(3);//bind socket to ip address
if(listen(serv->Socket,serv->MaxClients)==SOCKET_ERROR)return(4);			//set listen mode socket 
serv->Active = TRUE;												//set active
return (0);														//return true
}

/*------------------------------------------------------------------------------*
TCPCloseServer
-------------------------------------------------------------------------------*/
int TCPCloseServer(TCPSERVER *serv)									//close server
{
serv->Active = FALSE;												//set inactive
return closesocket(serv->Socket);										//close socket
}

/*------------------------------------------------------------------------------*
TCPCloseServerClient
-------------------------------------------------------------------------------*/
int TCPCloseServerClient(TCPCLIENT *client)								//close tcp client
{
client->Active = FALSE;												//set inactive
Log("TCP: Client closed");											//write a log
return closesocket(client->Socket);									//close socket
}

/*------------------------------------------------------------------------------*
TCPNextClientSockect
-------------------------------------------------------------------------------*/
int TCPNextClient(TCPCLIENT *clients)
{
int c;
c = 0;															//init counter
while(clients[c].Active && c<MAX_TCP_CLIENTS)c++;							//looking for available socket
if(c==MAX_TCP_CLIENTS)return 0;										//return null;
return(c+1);														//return id client
}

/*------------------------------------------------------------------------------*
TCPServerRecv
-------------------------------------------------------------------------------*/
int TCPServerRecv(SOCKET sock,char *str)
{
char msg[TYPE_CHAR128];
int bytes;
bytes = recv(sock,str,BUFFERSIZE,0);									//receive from client
if(bytes)
{
	sprintf(msg,"TCP: Socket (%i), Received<%i>:<%s>",sock,strlen(str),str);	//create a msg
	printf("Socket(%i):%s",sock,str);
	Log(msg);														//write a log
}
return bytes;
}

/*------------------------------------------------------------------------------*
TCPServerSend
-------------------------------------------------------------------------------*/
int TCPServerSend(SOCKET sock,char *str)
{
char msg[MAX_BUFFERSIZE];
if(send(sock,str,strlen(str),0)!=strlen(str))							//send
{
	sprintf(msg,"TCP: Socket (%i), Send<%i>:<%s>",sock,strlen(str),str);		//create a msg
	Log(msg);														//write a log
	return (1);													//return error
}
sprintf(msg,"TCP: Socket (%i), Send<%i>:<%s>",sock,strlen(str),str);			//create a msg
printf("Socket(%i):%s\n",sock,msg);
Log(msg);															//write a log
return (0);														//return true
}

/*------------------------------------------------------------------------------*
TCPServerClient
-------------------------------------------------------------------------------*/
void TCPServerClient(TCPCLIENT *client)
{
char msg[SQL_MAX_DATA_LEN];
char buffer[BUFFERSIZE];
char *answerd;
int  received;
int	empty;

if(client->Socket>0)
{	
	printf("Socket(%i):Open\n",client->Socket);
	sprintf(msg,"TCP: New thread instanded from '%s'",client->IP);			//create a msg
	Log(msg);														//write a log
	sprintf(msg,"TCP: Waiting for command...");							//create a msg
	Log(msg);														//write a log
	
	memset(&buffer,0,BUFFERSIZE);										//clear buffer
	empty = 0;													//init var
	answerd = NULL;												//init array	
	while(client->Active)											//while client socket is active
	{	
		if((received=TCPServerRecv(client->Socket,&buffer[0]))>0)			//if there is a recieved command
		{
			if(buffer[received-1]==0x0d)								//if the last byte is CR
			{			
				if(Command(client->Socket,client->IP,buffer))			//process command
				{
					TCPServerSend(client->Socket,TCP_ERROR);			//send error
				}
			}
			memset(&buffer,0,BUFFERSIZE);								//clear buffer
			empty = 0;											//init var
		}
		else
		{
			empty++;												//increment counter
			if(empty==TCP_TRYS_RECEIVED)TCPCloseServerClient(client);		//close socket
		}
	}
	printf("Socket(%i):Closed\n\n\n",client->Socket);
	sprintf(msg,"TCP: Thread Closed from %s",client->IP);					//create a msg
	Log(msg);														//write a log
}
CloseHandle(client->Thread);											//close thread
memset(client,0,sizeof(TCPCLIENT));									//free tcp client
}

/*------------------------------------------------------------------------------*
TCPListenServer
-------------------------------------------------------------------------------*/
void TCPServerListen(TCPSERVER *serv)									//listen for client request
{
char msg[TYPE_CHAR128];
struct sockaddr_in addrAccept;										//ip address client
int lenAccept;														//len
int error = 0;														//error
int id;

lenAccept = sizeof( addrAccept );										//init len
sprintf(msg,"TCP: Server Initiated at '%s:%ld'",serv->IP,serv->Port);			//create a msg
Log(msg);															//write a log
while(serv->Active)
{
	id = TCPNextClient(serv->Clients);									//return id of tcpclient socket array
	if(id == NULL)													//id can not be 0
	{
		sprintf(msg,"TCP: None sockets available");						//create a msg
		Log(msg);													//write a log
		Sleep(100);
		continue;	
	}	
	else															//id client
	{
		sprintf(msg,"TCP: Next conection at: %i",id);					//create a msg
		Log(msg);													//write a log
	}
	serv->Clients[id-1].Socket=accept(serv->Socket,(struct sockaddr*)&addrAccept,&lenAccept);//we create another socket to accept the connexion with the client socket 
	if(serv->Clients[id-1].Socket!=INVALID_SOCKET)
	{
		serv->Clients[id-1].Active = TRUE;								//set active
		strcpy(serv->Clients[id-1].IP,inet_ntoa(addrAccept.sin_addr));		//copy ip
		serv->Clients[id-1].Thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&TCPServerClient,&serv->Clients[id-1],0,0);//create thread for client
	}
}
CloseHandle(serv->Thread);											//close thread
memset(serv,0,sizeof(TCPSERVER));										//clear server obj
sprintf(msg,"TCP: Server Closed");										//create a msg
Log(msg);															//write a log
}

/*------------------------------------------------------------------------------*
InitTCPServer
-------------------------------------------------------------------------------*/
void InitTCPServer(TCPSERVER *serv,TCPCLIENT *clients,int n,char *ip, long port)//inicializa el servidor de ethernet
{
char msg[TYPE_CHAR128];
if(TCPOpenServer(serv,clients,n,ip,port)!=NULL)							//open socket
{
	sprintf(msg,"TCP: Unavailable to set server");						//create a msg
	Log(msg);														//write a log
	exit(1);														//finish program
}
serv->Thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&TCPServerListen,serv,0,0);//coloca socket en escucha
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/
