/*
 * misc.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef MISC_H_
#define MISC_H_

int GetLocalIP(char*);												//get local ip address
int FileExist(char*);												//check if file exist
void FileRead(FILE*,char*);											//reads a file record
void FileWrite(FILE*,char*);											//writes a file record
int sscanf_tab(char *src,char *format, ... );							//escanea entrada de archivo para extraer datos
unsigned char CheckSum(unsigned char*);									//calculate chksum
int   VerifyChk(unsigned char *);										//verify chksum
void  RegCpy(int,char*,char*,int);										//copy 1 record
void  RegStr(int,int,char*,char*,int);									//get a string
float RegFloat(int,int,char*,int);										//get a float
long  RegLong(int,int,char*,int);										//get a long
int   RegInt(int,int,char*,int);										//get a int
void  PrintScales(void);												//print scales array
void  PrintAgents(void);												//print agents array
void  PrintTickets(void);											//print tickets array
void  PrintDetail(void);												//print detail array
void	 InitVars(void);												//init global vars

#endif /* MISC_H */