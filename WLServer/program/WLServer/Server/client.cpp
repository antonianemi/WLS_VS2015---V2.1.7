/*------------------------------------------------------------------------------*
 * client.c	 													*
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
TCPClientClose
-------------------------------------------------------------------------------*/
int TCPClientClose(SOCKET sock)
{
Log("TCP: Client closed");											//write a log
return closesocket(sock);											//close socket
}

/*------------------------------------------------------------------------------*
TCPClientRecv
-------------------------------------------------------------------------------*/
int TCPClientRecv(SOCKET sock,char *str)
{
char msg[TYPE_CHAR128];
int bytes;
bytes = recv(sock,str,BUFFERSIZE,0);									//receive from client
sprintf(msg,"TCP: Client Socket (%i), Received<%i>:<%s>",sock,strlen(str),str);	//create a msg
Log(msg);															//write a log
return bytes;
}

/*------------------------------------------------------------------------------*
TCPClientSend
-------------------------------------------------------------------------------*/
int TCPClientSend(SOCKET sock,char *str)
{
char msg[TYPE_CHAR128];
if(send(sock,str,strlen(str),0)!=strlen(str))							//send
{
	sprintf(msg,"TCP: Client Socket (%i), Send<%i>:<%s>",sock,strlen(str),str);//create a msg
	Log(msg);														//write a log
	return (1);													//return error
}
sprintf(msg,"TCP: Client Socket (%i), Send<%i>:<%s>",sock,strlen(str),str);	//create a msg
Log(msg);															//write a log
return (0);														//return true
}

/*------------------------------------------------------------------------------*
TCPOpenClient
-------------------------------------------------------------------------------*/
int TCPOpenClient(char* ip,int port)
{
SOCKET sock;
WSADATA wsaData;													//wsadata struct
struct sockaddr_in addr;												//address struct
addr.sin_family = AF_INET ;											//set kind of connexion
addr.sin_addr.s_addr = inet_addr(ip);									//set local ip address
addr.sin_port = htons ((u_short)port);									//set port number
if(WSAStartup(MAKEWORD(2,0), &wsaData)!=NULL) return (0);					//Initialize Winsock 
if((sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)return(0);	//Create socket 
if(connect(sock, (struct sockaddr *) &addr,sizeof(addr))!=NULL)return(0);		//Connect to clienta
return (sock);														//return true
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/
