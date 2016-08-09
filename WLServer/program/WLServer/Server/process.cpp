/*------------------------------------------------------------------------------*
 * database.c		 												*
 * 		       	     FABRICANTES DE BÁSCULAS TORREY					*
 * 						SERVIDOR ETHERNET							*
 *------------------------------------------------------------------------------*
 *																*
 *												Gregory Garcia		*
 * 														      	*
 *-----------------------------------------------------------------------------*/

#include "extern.h"
#include "tables.h"													//tables definitions

/*------------------------------------------------------------------------------*
Star Code
-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
SendResponse
-------------------------------------------------------------------------------*/
void SendResponse(int sock, char *str,char *msg)
{
char cmd[MAX_BUFFERSIZE];
int bytes;
int n;
strcpy(cmd,str);cmd[4] = 0;											//trunk cmd
strcat(cmd,msg);													//create msg
n = strlen(cmd);													//longitud
if(n>1508)return;
cmd[n] = (char) CheckSum((unsigned char*)cmd);							//checksum
cmd[n+1] = 0x0d;													//CR
cmd[n+2] = 0x0;													//null
printf("Send<%i> <%s>",strlen(cmd),cmd);
bytes = TCPServerSend(sock,cmd);										//send msg	TODO
}

/*------------------------------------------------------------------------------*
SendConfirm
-------------------------------------------------------------------------------*/
int SendConfirm(int sock, char *str,char *cmd)
{
char msg[SQL_MAX_CMD_LEN];
int bytes;
SendResponse(sock,str,cmd);											//send msg
bytes = TCPServerRecv(sock,msg);										//recv confirm	TODO
if(strstr(cmd,TCP_OK)==0)return(1);									//return error	TODO
else	return(0);													//return ok 
}

/*------------------------------------------------------------------------------*
LoadRecords
-------------------------------------------------------------------------------*/
int LoadRecords(void *ptr,long size,int *types, int *sqltypes,char *db,char *tb,char *filter,char *sort)
{
char cmd[SQL_MAX_CMD_LEN];
char msg[SQL_MAX_CMD_LEN];
SQL *sql;
SQLSTMT *stmt = NULL;
void *pointer;
int i;

sprintf(msg,"SQL: Loading records from %s table",tb);						//create a msg
Log(msg);															//load a msg

if(SQLOpenDB(DBASE,&sql,SQL_READONLY))return(1);							//open db
if(SQLCountRecords(sql,tb,NULL)>0)
{
	if(filter==NULL)sprintf(cmd,"SELECT * FROM %s",tb);					//create cmd
	else sprintf(cmd,"SELECT * FROM %s WHERE (%s)",tb,filter);				//create cmd & filter
	if(sort!=NULL)													//if there are sort filter
	{
		strcat(cmd," ORDER BY ");									//add cmd sort
		strcat(cmd,sort);											//add filter
	}

	if(SQLOpenTable(sql,&stmt,&cmd[0],tb)==SQL_OK)						//open table		
	{
		i = 0;													//init var
		pointer = (void *)((long)ptr);								//init pointer
		while(SQLReadRecord(stmt,pointer,types,sqltypes)==SQL_OK)			//load a record
		{
			i++;													//increment counter
			pointer = (void *)((long)ptr + (size*i));					//increment base address
		}
	}		
	SQLFinalizeStmt(stmt);											//finalize sql stmt	
}
if(SQLCloseDB(DBASE,sql))return(1);									//close db
return (0);
}

/*------------------------------------------------------------------------------*
WriteRecords
-------------------------------------------------------------------------------*/
int WriteRecord(void *ptr,int *types, char **sqlfields,int *sqltypes,char *db,char *tb)
{
char cmd[SQL_MAX_CMD_LEN];
char fields[SQL_MAX_CMD_LEN];
SQL *sql;
int error;
int i;
	
if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);							//open db faild

i = 0;															//init var
memset(&fields[0],0,SQL_MAX_CMD_LEN);									//init var
while(sqltypes[i])													//while there is a field
{	
	strcat(fields,sqlfields[i]);										//copy the field name
	i++;															//increment counter
	if(sqltypes[i])strcat(fields," , ");								//add a separator
}

sprintf(cmd,"INSERT INTO %s (%s) values ",tb,fields);						//create cmd
error = SQLInsertRecord(sql,&cmd[0],ptr,types,sqltypes);					//insert record
if(SQLCloseDB(DBASE,sql))return(1);									//close db
return error;
}

/*------------------------------------------------------------------------------*
UpdateRecords
-------------------------------------------------------------------------------*/
int UpdateRecord(char *filter,void *ptr,int *types, char **sqlfields,int *sqltypes,char *db,char *tb)
{
char cmd[SQL_MAX_CMD_LEN];
SQL *sql;
int error;

if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);							//open db faild
sprintf(cmd,"UPDATE %s SET ",tb);										//create update cmd
error = SQLUpdateRecord(sql,filter,&cmd[0],ptr,types,sqlfields,sqltypes);		//update record
if(SQLCloseDB(DBASE,sql))return(1);									//close db
return error;
}

/*------------------------------------------------------------------------------*
RegisterScale
-------------------------------------------------------------------------------*/
int RegisterScale(int sock,char *ip,char *str)							//register an scale	
{
char filter[SQL_MAX_CMD_LEN];
char serie[TYPE_CHAR16];
int  error = 0;
int c,max;

c = 0;															//init var
RegStr(1,1,str,serie,4);												//get serial number
while((strcmp(Scales[c].Serie,serie)!=0) && c<MAX_SCALES)c++;				//looking for scales
if(c==MAX_SCALES)													//update
{
	max = 0;														//init var
	for(c=0;c<MAX_SCALES;c++)if(Scales[c].Id>max)max = Scales[c].Id;			//looking id
	c = 0;														//init var
	while(Scales[c].Id!=0 && c<MAX_SCALES)c++;							//looking for empty record

	Scales[c].Id = max+1;											//set id
	strcpy(Scales[c].Serie,serie);									//set serie
	Scales[c].Agent = 0;											//set agent
	Scales[c].Status = TRUE;											//set status	
	strcpy(Scales[c].IP,ip);											//set ip
	GetTime(Scales[c].Time);											//set time	
	error = WriteRecord(&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
}
else																//insert
{
	sprintf(filter," Serie='%s' ",serie);								//create cmd
	Scales[c].Status = TRUE;											//set status	
	strcpy(Scales[c].IP,ip);											//set ip
	GetTime(Scales[c].Time);											//set time	
	error = UpdateRecord(filter,&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
}
Config.RefresScales = 1;												//refresh scales
if(error==0)SendResponse(sock,str,TCP_OK);								//response to client
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
RegisterAgent
-------------------------------------------------------------------------------*/
int RegisterAgent(int sock,char *ip,char *str)							//register an scale	
{
char filter[SQL_MAX_CMD_LEN];
long agent;
int error = 0;
int c;

//get data
agent = RegLong(1,1,str,4);											//get line number

//update scale
c = 0;
while((strcmp(Scales[c].IP,ip)!=0) && c<MAX_SCALES)c++;					//looking for agent
if(c==MAX_SCALES)return(1);											//if is there not
sprintf(filter," IP ='%s' ",ip);										//create cmd
Scales[c].Agent = agent;												//init vars
Scales[c].Status = TRUE;												//set status	
GetTime(Scales[c].Time);												//set time	
error = UpdateRecord(filter,&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
Config.RefresScales = 1;												//refresh scales
return (0);
}

/*------------------------------------------------------------------------------*
Verify Agent
-------------------------------------------------------------------------------*/
int VerifyAgent(int sock,char *str)									//verify if there a sale of an agent
{
char msg[SQL_MAX_CMD_LEN];
long count;
long agent;
int c;

agent = RegLong(1,1,str,4);											//get agent number
count = 0;														//init var
c = 0;															//init counter
while(Detail[c].Id!=0 && c<MAX_DETAIL)									//for all record
{
	if(Detail[c].Id==agent)count++;									//increment counter
	c++;															//increment counter
}
sprintf(msg,"%ld\t",count);											//create msg
SendResponse(sock,str,msg);											//response to client
return (0);														//return answerd
}

/*------------------------------------------------------------------------------*
RemoteReleaseAgent
-------------------------------------------------------------------------------*/
int RemoteReleaseAgent(TCPCLIENT client)								//release agent at scale
{
int  error = 0;
client.Socket = TCPOpenClient(client.IP,Config.ClientPort);					//open client port
error = TCPClientSend(client.Socket,client.Msg);							//close client port
error = TCPClientClose(client.Socket);									//send to client port
return error;
}

/*------------------------------------------------------------------------------*
Release Agent
-------------------------------------------------------------------------------*/
int ReleaseAgent(int sock,char *ip,char *str)							//release an agent at all scales
{
TCPCLIENT client;
char filter[SQL_MAX_CMD_LEN];
long agent;
int  error = 0;
int c;
int n;
char chk;

c = 0;															//init var
agent = RegLong(1,1,str,4);											//get serial number
while(Scales[c].Agent!=agent && c<MAX_SCALES)c++;							//looking for agent
if(c<MAX_SCALES && strcmp(Scales[c].IP,ip)!=0)							//if is a different scale with same agent
{
	strcpy(client.IP,Scales[c].IP);									//copy message
	strcpy(client.Msg,str);											//copy message
	n = strlen(client.Msg) - 1;										//len
	client.Msg[n] = 0;												//add null
	chk = CheckSum((unsigned char*)client.Msg);							//calculate checksum
	client.Msg[n] = chk;											//add checksum
	client.Msg[n+1] = 0x0d;											//add return
	client.Msg[n+2] = 0x0;											//add return
	if(RemoteReleaseAgent(client))error = 1;							//resend message to scale
	sprintf(filter," IP ='%s' ",Scales[c].IP);							//create cmd
	Scales[c].Agent = 0;											//init vars
	error = UpdateRecord(filter,&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
}
Config.RefresScales = 1;												//refresh scales
if(error==0)SendResponse(sock,str,TCP_OK);								//response to client
else SendResponse(sock,str,TCP_ERROR);									//response to client
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
GetTicketAll
-------------------------------------------------------------------------------*/
int GetTicketAll(int sock,char *ip,char *str)							//get ticket of an agent
{
char filter[SQL_MAX_CMD_LEN];
char msg[MAX_BUFFERSIZE];
char pref[16];
long	agent;
int	error = 0;
int	c;
int	count;

agent = RegLong(1,1,str,4);											//get agent number

//update scale
c = 0;
while((strcmp(Scales[c].IP,ip)!=0) && c<MAX_SCALES)c++;					//looking for agent
if(c==MAX_SCALES)return(1);											//if is there not
sprintf(filter," IP ='%s' ",ip);										//create cmd
Scales[c].Agent = agent;												//init vars
Scales[c].Status = TRUE;												//set status	
GetTime(Scales[c].Time);												//set time	
error = UpdateRecord(filter,&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record

//get detail
c = 0;															//init vars
count = 0;														//init vars
memset(&msg[0],0,SQL_MAX_DATA_LEN);									//init vars
while(Detail[c].Id && error==0 && c<MAX_DETAIL)							//while there is a item
{
	if(Detail[c].Id==agent)											//is the same agent
	{
		strcat(msg,Detail[c].Content);								//create msg
		strcat(msg,"\t\n");											//create msg				
		count++;
		if(strlen(msg)>MAX_BUFFERSIZE)return 1;							//return error
	}
	c++;															//increment counter
}

sprintf(pref,"G%03i\t\n",count);
printf(pref);
printf(msg);
Config.RefresScales = 1;												//refresh scales
if(error==0)SendResponse(sock,pref,msg);								//response to client		
return (0);														//return answerd
}

/*------------------------------------------------------------------------------*
AddItem
-------------------------------------------------------------------------------*/
int AddItem(int sock,char *ip,char *str)								//add item to an agent sale
{
SQL *sql;
char filter[SQL_MAX_CMD_LEN];
char content[SQL_MAX_CMD_LEN];
long agent;
long line;
int error = 0;
int find;
int c;


//get data
agent = RegLong(1,1,str,4);											//get line number
line = RegLong(2,1,str,4);											//get line number
RegStr(3,1,str,content,4);											//get new ticket header

//update scale
c = 0;
while((strcmp(Scales[c].IP,ip)!=0) && c<MAX_SCALES)c++;					//looking for agent
if(c==MAX_SCALES)return(1);											//if is there not
sprintf(filter," IP ='%s' ",ip);										//create cmd
Scales[c].Agent = agent;												//init vars
Scales[c].Status = TRUE;												//set status	
GetTime(Scales[c].Time);												//set time	
error = UpdateRecord(filter,&Scales[c],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
Config.RefresScales = 1;												//refresh scales

//looking for position
c = 0;															//init counter
find = FALSE;														//init find
while(Detail[c].Id && c<MAX_DETAIL)									//looking for record
{
	if(Detail[c].Id==agent && Detail[c].Line==line)						//there is this line at detail
	{
		find = TRUE;												//set var true
		break;													//break while
	}
	c++;															//increment counter
}

if(find==FALSE)													//add 
{
	//add new item
	c = 0;														//init var
	while(Detail[c].Id!=0 && c<MAX_DETAIL)c++;							//looking for empty record
	if(c==MAX_DETAIL)return(1);										//error	
	Detail[c].Id = agent;											//set agent
	Detail[c].Line = line;											//set line
	strcpy(Detail[c].Content,content);									//set content
	Detail[c].Status = 1;											//set status true
	GetTime(Detail[c].Time);											//set time	
	error = WriteRecord(&Detail[c],(int*)&DETAIL_TYPES[0],(char **)&SQL_DETAIL_FIELDS[0],(int*)&SQL_DETAIL_TYPES[0],DBASE,TABLE_DETAIL);// write record

}
else																//insert line
{
	//sort lines
	c = 0;														//init counter	
	while(Detail[c].Id && c<MAX_DETAIL)								//looking for record	
	{
		if(Detail[c].Id==agent && Detail[c].Line>=line)					//there is this line at detail
			Detail[c].Line += 1;									//increment counter
		c++;
	}

	//add new item
	c = 0;														//init var
	while(Detail[c].Id!=0 && c<MAX_DETAIL)c++;							//looking for empty record
	if(c==MAX_DETAIL)return(1);										//error	
	Detail[c].Id = agent;											//set agent
	Detail[c].Line = line;											//set line
	strcpy(Detail[c].Content,content);									//set content
	Detail[c].Status = 1;											//set status true
	GetTime(Detail[c].Time);											//set time	

	//remove lines at detail
	if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);						//open db
	sprintf(filter," Id_Agent = %ld",agent);							//create filter	
	error = SQLDeleteRecord(sql,TABLE_DETAIL,filter);						//delete records at table
	if(SQLCloseDB(DBASE,sql))return(1);								//close db

	//write lines at detail
	c = 0;														//init counter	
	while(Detail[c].Id && c<MAX_DETAIL)								//looking for record
	{
		if(Detail[c].Id==agent)										//there is this line at detail
			error = WriteRecord(&Detail[c],(int*)&DETAIL_TYPES[0],(char **)&SQL_DETAIL_FIELDS[0],(int*)&SQL_DETAIL_TYPES[0],DBASE,TABLE_DETAIL);// write record
		c++;														//increment counter
	}

}

//reload detail
memset(&Detail,0,sizeof(DETAIL)*MAX_DETAIL);								//inicializa estructura tickets
LoadRecords((void*)&Detail[0],sizeof(DETAIL),(int*)&DETAIL_TYPES,(int*)&SQL_DETAIL_TYPES,DBASE,TABLE_DETAIL,NULL," Id_Agent, Line ");//load list of detail
Config.RefresTickets = 1;											//refresh tickets
if(error==0)SendResponse(sock,str,TCP_OK);								//response to client
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
KillTicket
-------------------------------------------------------------------------------*/
int KillTicket(int sock,char *str)										//kill ticket of an agent
{
SQL *sql;
char filter[SQL_MAX_CMD_LEN];
int error = 0;
int agent;

agent = RegLong(1,1,str,4);											//get agent number
if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);							//open db
sprintf(filter," Id_Agent = %ld",agent);								//create filter
error = SQLDeleteRecord(sql,TABLE_DETAIL,filter);							//delete records at table detail
if(SQLCloseDB(DBASE,sql))return(1);									//close db
if(error==0)SendResponse(sock,str,TCP_OK);								//response to client
memset(&Detail,0,sizeof(DETAIL)*MAX_DETAIL);								//inicializa estructura tickets
LoadRecords((void*)&Detail[0],sizeof(DETAIL),(int*)&DETAIL_TYPES,(int*)&SQL_DETAIL_TYPES,DBASE,TABLE_DETAIL,NULL," Id_Agent, Line ");//load list of detail
Config.RefresTickets = 1;											//refresh tickets
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
KillItem
-------------------------------------------------------------------------------*/
int KillItem(int sock,char *str)										//kill ticket of an agent
{
SQL *sql;
char filter[SQL_MAX_CMD_LEN];
long agent;
int line;
int error = 0;
int find;
int c;

agent = RegLong(1,1,str,4);											//get agent number
line = RegLong(2,1,str,4);											//get ip address
printf("*** Kill Agent: %i, Line:%i\n",agent,line);

//delete item at memory
c = 0;															//init counter
find = FALSE;														//init find
while(Detail[c].Id && c<MAX_DETAIL)									//looking for record
{	
	if(Detail[c].Id==agent && Detail[c].Line==line)						//there is this line at detail
	{
		printf("*** Kill one\n");
		memset(&Detail[c],0,sizeof(DETAIL));							//clear record
		find = TRUE;												//set var true
		break;													//break while
	}
	c++;															//increment counter
}


if(find==TRUE)														//item removed 
{
	//recalculate index
	c = 0;														//init counter
	find = FALSE;													//init find
	line = 1;														//init counter lines
	while(c<MAX_DETAIL)								//looking for record
	{
		if(Detail[c].Id==agent)										//there is this line at detail
		{			
			Detail[c].Line = line;									//set new line number
			line++;												//increment line number
		}
		c++;														//increment counter
	}

	//delete records at table
	if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);						//open db
	sprintf(filter," Id_Agent = %ld ",agent);							//create filter
	error = SQLDeleteRecord(sql,TABLE_DETAIL,filter);						//delete records at table
	if(SQLCloseDB(DBASE,sql))return(1);								//close db

	//write lines at detail
	c = 0;														//init counter	
	while(c<MAX_DETAIL)												//looking for record
	{
		if(Detail[c].Id==agent)										//there is this line at detail
			error = WriteRecord(&Detail[c],(int*)&DETAIL_TYPES[0],(char **)&SQL_DETAIL_FIELDS[0],(int*)&SQL_DETAIL_TYPES[0],DBASE,TABLE_DETAIL);// write record
		c++;														//increment counter
	}

	//load records
	memset(&Detail,0,sizeof(DETAIL)*MAX_DETAIL);							//inicializa estructura tickets
	LoadRecords((void*)&Detail[0],sizeof(DETAIL),(int*)&DETAIL_TYPES,(int*)&SQL_DETAIL_TYPES,DBASE,TABLE_DETAIL,NULL," Id_Agent, Line ");//load list of detail
	Config.RefresTickets = 1;										//refresh tickets
}

if(error==0)SendResponse(sock,str,TCP_OK);								//response to client
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
PurgeTable
-------------------------------------------------------------------------------*/
int PurgeTable(char *table,char *filter)
{
SQL *sql;
int error = 0;
if(SQLOpenDB(DBASE,&sql,SQL_READWRITE))return(1);							//open db
error = SQLDeleteRecord(sql,table,filter);								//delete records at table
if(SQLCloseDB(DBASE,sql))return(1);									//close db
return error;														//return answerd
}

/*------------------------------------------------------------------------------*
InitDatabase
-------------------------------------------------------------------------------*/
void InitDatabase(void)												//init database
{
if(Config.RestartTables)
{
	PurgeTable(TABLE_SCALES,"");										//purge scales table
	PurgeTable(TABLE_DETAIL,"");										//purge detail table
}
else
{
	LoadRecords((void*)&Scales[0],sizeof(SCALE),(int*)&SCALE_TYPES,(int*)&SQL_SCALE_TYPES,DBASE,TABLE_SCALES,NULL," Serie ");//load list of scales
	LoadRecords((void*)&Detail[0],sizeof(DETAIL),(int*)&DETAIL_TYPES,(int*)&SQL_DETAIL_TYPES,DBASE,TABLE_DETAIL,NULL," Id_Agent, Line ");//load list of detail
}

memset(Msg,0,SQL_MAX_DATA_LEN);										//log msg
Config.RefresScales = 1;												//refresh scales
Config.RefresTickets = 1;											//refresh tickets
}

/*------------------------------------------------------------------------------*
TestSQLite
-------------------------------------------------------------------------------*/
void TestSQLite(void)
{
int error;
Scales[0].Id = 1;												//set id
strcpy(Scales[0].Serie,"A1300101");								//set serie
Scales[0].Agent = 0;											//set agent
Scales[0].Status = TRUE;											//set status	
strcpy(Scales[0].IP,"192.168.100.100");								//set ip

printf("Test SLQite\n");
while(1)
{

	GetTime(Scales[0].Time);											//set time	
	error = UpdateRecord(" Id = 1 ",&Scales[0],(int*)&SCALE_TYPES[0],(char **)&SQL_SCALE_FIELDS[0],(int*)&SQL_SCALE_TYPES[0],DBASE,TABLE_SCALES);// write record
	printf("Saved Table\n");
	Sleep(2000);
}

exit(0);
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/