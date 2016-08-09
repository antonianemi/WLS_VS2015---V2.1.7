/*
 * server.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef SERVER_H_
#define SERVER_H_

int TCPServerRecv(SOCKET,char*);										//recv datafor tcp
int TCPServerSend(SOCKET,char*);										//send data for tcp
void InitTCPServer(TCPSERVER*,TCPCLIENT*,int,char *,long);					//socket un perto en escucha
int  TCPCloseServer(TCPSERVER*);										//cierra un servidor tcp


#endif /* SERVER_H_ */