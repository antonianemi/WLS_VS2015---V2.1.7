/*
 * database.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef DATABASE_H_
#define DATABASE_H_


int RegisterScale(int,char*,char*);									//register a scale	
int RegisterAgent(int,char*,char*);									//register an Agent
int VerifyAgent(int,char*);											//verify if there a sale of an agent
int ReleaseAgent(int,char*,char*);										//release an agent at all scales
int GetTicketAll(int,char*,char*);										//get ticket of an agent
int AddTicket(int,char*,char*);										//add item to an agent sale
int AddItem(int,char*,char*);											//add item to an agent sale
int KillTicket(int,char*);											//kill ticket of an agent
int KillItem(int,char*);												//kill item to an agent sale
int WriteRecord(void*,int*,char**,int*,char*,char*);						//write record
int UpdateRecord(char*,void*,int*,char**,int*,char*,char*);					//update record
int LoadRecords(void*,long,int*,int*,char*,char*);						//load records
int PurgeTable(char *,char*);											//purge table
void InitDatabase(void);												//init database


#endif /* DATABASE_H */
