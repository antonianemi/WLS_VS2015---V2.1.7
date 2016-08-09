/*------------------------------------------------------------------------------*
 * config.c		 												*
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
LoadConfig
-------------------------------------------------------------------------------*/
void LoadConfig(void)												//load config
{
char sData[SQL_MAX_CMD_LEN];
FILE *file;
if((file=fopen(FILE_CONFIG,"r"))!=NULL)									//there is not a log file
{
	FileRead(file,sData);											//reads a record
	sscanf_tab(sData,(char *) "%d %d %d %d %d %d",						//scans record		
		&Config.ServerPort,											//server port
		&Config.ClientPort,											//client port
		&Config.Lenguage,											//lenguage
		&Config.TimeoutScale,										//timeout scale
		&Config.TimeoutAgent,										//timeout agent
		&Config.RestartTables										//start with empty tables
		);
	fclose(file);													//close the file
	Log("Cfg: Load Config file");										//load a msg log
}
else Log("Cfg: Unable to load Config file");								//save a msg log
}

/*------------------------------------------------------------------------------*
SaveConfig
-------------------------------------------------------------------------------*/
void SaveConfig(void)												//save config
{
char sData[SQL_MAX_CMD_LEN];
FILE *file;
if((file=fopen(FILE_CONFIG,"w"))!=NULL)									//there is not a log file
{
	sprintf(sData,"%ld\t%ld\t%i\t%i\t%i\n",								//formated data		
		Config.ServerPort,											//server port
		Config.ClientPort,											//client port
		Config.Lenguage,											//lenguage
		Config.TimeoutScale,										//timeout scale
		Config.TimeoutAgent,										//timeout agent
		Config.RestartTables										//start with empty tables
		  );		
	FileWrite(file,sData);											//write at file	
	fclose(file);													//close the file
	Log("Cfg: Save Config file");										//save a msg log	
}
else Log("Cfg: Unable to save Config file");								//save a msg log
}

/*------------------------------------------------------------------------------*
InitConfig
-------------------------------------------------------------------------------*/
void InitConfig(void)												//init config
{

if(FileExist(FILE_CONFIG)==NULL)										//there is not a log file
{
	Log("Cfg: Create Config file");									//save a msg log

	//valores por default
	Config.ServerPort = 50035;										//listen server port
	Config.ClientPort = 50037;										//listen client por
	Config.Lenguage = 1;											//lenguage
	Config.TimeoutAgent = 15;										//agent timeout
	Config.TimeoutScale = 15;										//scale timeout
	Config.RestartTables = 0;										//start with empty tables
	SaveConfig();													//save config
}
else LoadConfig();													//load config file

if(GetLocalIP(Config.IP))Log("Cfg: Local IP obtained");					//get local ip
else Log("Cfg: Local IP failed");										//error

}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/

