/*
 * client.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef CLIENT_H_
#define CLIENT_H_


int TCPClientRecv(SOCKET,char*);										//recv data from client
int TCPClientSend(SOCKET,char*);										//send data to client 
int TCPClientClose(SOCKET);											//close client socket
int TCPOpenClient(char*,int);											//open client socket


#endif /* CLIENT_H_ */
