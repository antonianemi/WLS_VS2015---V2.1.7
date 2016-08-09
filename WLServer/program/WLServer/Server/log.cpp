/*------------------------------------------------------------------------------*
 * log.c		 													*
 * 		       	     FABRICANTES DE BÁSCULAS TORREY					*
 * 						SERVIDOR ETHERNET							*
 *------------------------------------------------------------------------------*
 *																*
 *												Gregory Garcia		*
 * 														      	*
 *-----------------------------------------------------------------------------*/

#include "extern.h"
#include "viewer.h"


/*------------------------------------------------------------------------------*
Star Code
-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
ClearLog
-------------------------------------------------------------------------------*/
void ClearLog(void)													//clear log file
{
FILE *file;
if((file=fopen(FILE_LOG,"w"))==NULL)									//there is not a log file
{
	printf("Log: Failed to clear log file\n");
}
else 
{
	fclose(file);													//close log file
	printf("Log: Log file clear\n");
}
}

/*------------------------------------------------------------------------------*
Log
-------------------------------------------------------------------------------*/
void Log(char *str)													//log
{
FILE *file;
char msg[MAX_BUFFERSIZE];
int c;

memset(&msg[0],0,SQL_MAX_CMD_LEN);										//clear msg var
GetTime(msg);														//init msg with time
strcat(msg," ");													//add separator
strcat(msg,str);													//add msg
c=0;while(msg[c] && c<256){if(msg[c]=='\n' || msg[c]=='\r')msg[c] = ' ';c++;}	//remove \n & \r
strcat(msg,"\n");													//add new line
if((file=fopen(FILE_LOG,"a"))==NULL)									//there is not a log file
{
	printf("Log: Failed to write at file\n");
}
else 
{
	fwrite(msg,strlen(msg),1,file);									//write msg at log file
	strcpy(Msg,msg);												//copy log msg
	fclose(file);													//close log file		
}
//printf("%s\n",str);													//show msg log
}

/*------------------------------------------------------------------------------*
InitLog
-------------------------------------------------------------------------------*/
void InitLog(void)													//init log
{
FILE *file;
if((file=fopen(FILE_LOG,"r"))==NULL)									//there is not a log file
{
	printf("Log: File <%s>not found\n",FILE_LOG);	
	if((file = fopen(FILE_LOG,"w"))!=NULL)								//create log file
	{		
		fclose(file);												//close log file
		Log("Log: Log file created");									//log msg
	}
	else printf("Log: Failed to create <%s> log file\n",FILE_LOG);
}
else fclose(file);													//close log file
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/
