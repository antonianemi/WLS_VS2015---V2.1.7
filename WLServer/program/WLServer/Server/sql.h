/*
 * sql.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef SQL_H_
#define SQL_H_

int SQLOpenDB(char*,SQL**,int);										//open db
int SQLCloseDB(char*,SQL*);											//close db
int SQLFinalizeStmt(SQLSTMT*);										//finalize sql stament query
int SQLCountRecords(SQL*,char*,char*);									//count the records at table
int SQLOpenTable(SQL*,SQLSTMT**,char*,char*);							//open a table
int SQLReadRecord(SQLSTMT*,void*,int*,int*);								//read records from a table
int SQLInsertRecord(SQL *sql,char*,void*,int*,int*);						//insert a record at table
int SQLUpdateRecord(SQL *sql,char*,char*,void*,int*,char**,int*);			//update a record at table
int SQLDeleteRecord(SQL*,char*,char*);									//delete records at table

#endif /* SQL_H */