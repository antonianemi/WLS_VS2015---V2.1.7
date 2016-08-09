/*------------------------------------------------------------------------------*
 * sql.c		 													*
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
SQLOpenDBase
-------------------------------------------------------------------------------*/
int SQLOpenDB(char *db,SQL **sql,int mode)								//open db
{
char msg[TYPE_CHAR128];
int error;
error = sqlite3_open_v2(db,sql,mode,NULL);								//open db connection
if(error!=SQL_OK)													//if there is a error
{
	sprintf(msg,"SQL: Failed to open database %s",db);					//create a msg
	Log(msg);														//write a log
     return (SQL_ERROR);												//return error
     }
sprintf(msg,"SQL: Opened <%s>",db) ;
Log(msg);															//write a log
return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
SQLCloseDB
-------------------------------------------------------------------------------*/
int SQLCloseDB(char *db,SQL *sql)										//close db
{
char msg[TYPE_CHAR128];
sprintf(msg,"SQL: Closed <%s>",db);									//create a msg
Log(msg);															//write a log
return sqlite3_close(sql);											//close db connection
}

/*------------------------------------------------------------------------------*
SQLResetStmt
-------------------------------------------------------------------------------*/
int SQLFinalizeStmt(SQLSTMT *stmt)										//reset sql stament query
{
char msg[TYPE_CHAR128];
sprintf(msg,"SQL: Finalize stament");									//create a msg
Log(msg);															//write a log
return sqlite3_finalize(stmt);										//reset sql stament query
}

/*------------------------------------------------------------------------------*
SQLCountRecords 
-------------------------------------------------------------------------------*/
int SQLCountRecords(SQL *sql,char *tb,char *filter)						//count the records at table
{
SQLSTMT *stmt;
char msg[TYPE_CHAR128];
char cmd[SQL_MAX_CMD_LEN];
int  error;
int  count;
count = 0;

if(filter==NULL)sprintf(cmd,"SELECT COUNT(*) FROM %s",tb);					//create cmd
else sprintf(cmd,"SELECT COUNT(*) FROM %s WHERE %s",tb,filter);				//create cmd

sprintf(msg,"SQL: cmd <%s>",cmd);										//create a msg
Log(msg);															//write a log
error = sqlite3_prepare_v2(sql,cmd,strlen(cmd),&stmt,NULL);					//exec cmd
if(error!=SQL_OK)													//if there is a error
{
	sprintf(msg,"SQL:Failed to count records from %s",tb) ;				//create a msg    
	Log(msg);														//write a log
     return (SQL_ERROR);												//return error
}
if(sqlite3_step(stmt)!=SQL_ERROR)count = sqlite3_column_int(stmt, 0);			//read records counter
sprintf(msg,"SQL: %i records at <%s>",count,tb);							//create a log
Log(msg);															//write a msg
SQLFinalizeStmt(stmt);												//finalize sql stmt
return count;														//return number of records
}

/*------------------------------------------------------------------------------*
SQLOpenTable
-------------------------------------------------------------------------------*/
int SQLOpenTable(SQL *sql,SQLSTMT **stmt,char *cmd,char *tb)				//open table
{
char msg[TYPE_CHAR128];
int  error;
sprintf(msg,"SQL: cmd <%s>",cmd);										//create a msg
Log(msg);															//write a log
error = sqlite3_prepare_v2(sql,cmd,strlen(cmd),stmt,NULL);					//exec cmd
if(error!=SQL_OK)													//if there is a error
{
	sprintf(msg,"SQL: Failed to open %s table",tb) ;						//create a msg
	Log(msg);														//load msg
     return (SQL_ERROR);												//return error
}
sprintf(msg,"SQL: %s table opened",tb) ;								//create a msg
Log(msg);															//write a msg
return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
SQLReadRecords function
-------------------------------------------------------------------------------*/
int SQLReadRecord(SQLSTMT *stmt,void *ptr, int *types,int *sqltypes)			//read records from a table
{
char strdata[SQL_MAX_DATA_LEN];
float floatdata;
long offset;
long iddata;
long longdata;
int intdata;
int type;
void *dst,*src;

if(sqlite3_step(stmt)!=SQL_ROW)return (SQL_ERROR);						//if there is not another record
dst = (void *)(long)ptr;												//increment base address
offset = 0;														//init offset
type = 0;															//init var
while(sqltypes[type])												//while there if a field
{
	switch(sqltypes[type])
	{
	case SQL_TYPE_EMPTY:	continue;break;							//ignore this field
	case SQL_TYPE_ID:												//get id data
		iddata = sqlite3_column_int(stmt,type);							
		src = (void*)&iddata;										//points to long
		break;	
	case SQL_TYPE_INT:												//get an integer data
		intdata = sqlite3_column_int(stmt,type);
		src = (void*)&intdata;										//points to int
		break;	
	case SQL_TYPE_LONG:												//get a long data
		longdata = sqlite3_column_int(stmt,type);
		src = (void*)&longdata;										//points to long
		break;	
	case SQL_TYPE_FLOAT:											//get a real data
		floatdata = (float) sqlite3_column_double(stmt,type);
		src = (void*)&floatdata;										//points to float
		break;
	case SQL_TYPE_TEXT:												//get a string data
		strcpy(strdata,(char*)sqlite3_column_text(stmt,type));
		src = (void*)&strdata[0];									//points to string
		break;
	}	
	memcpy(dst,src,types[type]);										//copy block of memory
	offset += types[type];											//increment offset
	dst = (void *)((long)ptr + offset);								//increment base address
	type++;														//increment type
}
return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
SQLInsertRecords function
-------------------------------------------------------------------------------*/
int SQLInsertRecord(SQL *sql,char *cmd,void *ptr, int *types,int *sqltypes)//write a record at table
{
char msg[TYPE_CHAR128];
char msg2[TYPE_CHAR256];
char values[SQL_MAX_DATA_LEN];
char aux[SQL_MAX_DATA_LEN];
float floatdata;
long offset;
long longdata;
int intdata;
int type;
int  error;
char byte;
void *src;

src = (void *)(long)ptr;												//increment base address
offset = 0;														//init offset
type = 0;															//init var
memset(&values[0],0,SQL_MAX_CMD_LEN);									//init var
strcat(cmd," (");													//add parenthesis
while(sqltypes[type])												//while there if a field
{
	memset(&aux[0],0,SQL_MAX_CMD_LEN);									//init var
	switch(types[type])
	{	
	case TYPE_BYTE:
		memcpy(&byte,src,1);										//copy byte
		sprintf(aux,"%c",byte);										//set char
		break;
	case TYPE_INT:													//set a long data
		memcpy(&intdata,src,2);
		sprintf(aux,"%ld",intdata);									//set long		
		break;	
	case TYPE_LONG:												//set a long data
		memcpy(&longdata,src,4);
		sprintf(aux,"%ld",longdata);									//set long
		break;	
	case TYPE_FLOAT:												//set a float data
		memcpy(&floatdata,src,4);
		sprintf(aux,"%7.2",floatdata);								//set float
		break;		
	case TYPE_CHAR16:												//set a string data
	case TYPE_CHAR32:												//set a string data
	case TYPE_CHAR64:												//set a string data
	case TYPE_CHAR128:												//set a string data
	case TYPE_CHAR256:												//set a string data
		sprintf(aux,"'%s'",(char*)src);								//set string
		break;
	}	

	strcat(values,aux);
	offset += types[type];											//increment offset
	src = (void *)((long)ptr + offset);								//increment base address
	type++;														//increment type
	if(sqltypes[type])strcat(values," , ");								//add a separator
}
strcat(cmd,values);													//copy values
strcat(cmd,"); ");													//add semicolon

strcpy(msg2,"SQL: ");												//create msg
strcat(msg2,cmd);													//create msg
Log(msg2);														//write a msg
error = sqlite3_exec(sql,cmd,NULL,NULL,NULL);
if (error!=SQLITE_OK)
{
	sprintf(msg,"SQL: Not was possible to execute that  command");			//create a msg
	Log(msg);														//write a msg
	return (SQL_ERROR);												//return error
}
sprintf(msg,"SQL: 1 record add");										//create a msg
Log(msg);															//write a msg
return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
SQLUpdateRecords function
-------------------------------------------------------------------------------*/
int SQLUpdateRecord(SQL *sql,char *filter,char *cmd,void *ptr, int *types,char **sqlfields,int *sqltypes)//write a record at table
{
char msg[TYPE_CHAR128];
char msg2[TYPE_CHAR256];
char values[SQL_MAX_DATA_LEN];
char aux[SQL_MAX_DATA_LEN];
float floatdata;
long offset;
long longdata;
int intdata;
int type;
int  error;
char byte;
void *src;

src = (void *)(long)ptr;												//increment base address
offset = 0;														//init offset
type = 0;															//init var
memset(&values[0],0,SQL_MAX_CMD_LEN);									//init var
while(sqltypes[type])												//while there if a field
{

	strcat(values,sqlfields[type]);									//add var name
	strcat(values,"=");												//add equal symbol
	memset(&aux[0],0,SQL_MAX_CMD_LEN);									//init var
	switch(types[type])
	{	
	case TYPE_BYTE:
		memcpy(&byte,src,1);										//copy byte
		sprintf(aux,"%c",byte);										//set char
		break;
	case TYPE_INT:													//set a long data
		memcpy(&intdata,src,2);
		sprintf(aux,"%ld",intdata);									//set long		
		break;	
	case TYPE_LONG:												//set a long data
		memcpy(&longdata,src,4);
		sprintf(aux,"%ld",longdata);									//set long
		break;	
	case TYPE_FLOAT:												//set a float data
		memcpy(&floatdata,src,4);
		sprintf(aux,"%7.2",floatdata);								//set float
		break;		
	case TYPE_CHAR16:												//set a string data
	case TYPE_CHAR32:												//set a string data
	case TYPE_CHAR64:												//set a string data
	case TYPE_CHAR128:												//set a string data
	case TYPE_CHAR256:												//set a string data
		sprintf(aux,"'%s'",(char*)src);								//set string
		break;
	}	

	strcat(values,aux);												//add value of var
	offset += types[type];											//increment offset
	src = (void *)((long)ptr + offset);								//increment base address
	type++;														//increment type
	if(sqltypes[type])strcat(values," , ");								//add a separator
}
strcat(cmd,values);													//copy values
strcat(cmd," WHERE ");												//add where stament
strcat(cmd,filter);													//add filter condition
strcat(cmd,"; ");													//add semicolon

strcpy(msg2,"SQL: ");												//create msg
strcat(msg2,cmd);													//create msg
Log(msg2);														//write a msg
error = sqlite3_exec(sql,cmd,NULL,NULL,NULL);
if (error!=SQLITE_OK)
{
	printf("Error: %i",error);
	sprintf(msg,"SQL: Not was possible to execute that  command");			//create a msg
	Log(msg);														//write a msg
	return (SQL_ERROR);												//return error
}
sprintf(msg,"SQL: 1 record update");									//create a msg
Log(msg);															//write a msg

return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
SQLDeleteRecords
-------------------------------------------------------------------------------*/
int SQLDeleteRecord(SQL *sql,char *tb,char *filter)						//delete records at table
{
char msg[TYPE_CHAR128];
char cmd[SQL_MAX_CMD_LEN];
int  error;

if(strlen(filter)>0)sprintf(cmd,"DELETE FROM %s WHERE %s ;",tb,filter);		//add filter
else sprintf(cmd,"DELETE FROM %s ;",tb);								//prepare cmd

sprintf(msg,"SQL: cmd <%s>",cmd);										//create a msg
Log(msg);															//write a msg
error = sqlite3_exec(sql,cmd,NULL,NULL,NULL);							//execute cmd
if (error!=SQL_OK)													//if there is a error
{
	sprintf(msg,"SQL: unable to erase %s table",tb);						//create a msg
	Log(msg);														//write a msg
	return (1);													//return error
}
sprintf(msg,"SQL: %s table erased",tb);									//create a msg
Log(msg);															//write a msg
return (SQL_OK);													//return true
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/