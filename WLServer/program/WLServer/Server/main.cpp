/*------------------------------------------------------------------------------*
 * main.c		 													*
 * 		       	     FABRICANTES DE BÁSCULAS TORREY					*
 * 						SERVIDOR ETHERNET							*
 *------------------------------------------------------------------------------*
 *																*
 *												Gregory Garcia		*
 * 														      	*
 *-----------------------------------------------------------------------------*/
#include "extern.h"													//extern functions & vars
#include "viewer.h"													//extern functions & vars


/*-------------------------------------------------------------------------------
Star Code
-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------
declaracion de variable globales
-------------------------------------------------------------------------------*/
//namespaces definition
using namespace Interface;

//structs definition
TCPSERVER Server;													//server socket
TCPCLIENT Clients[MAX_TCP_CLIENTS];									//clients sockets
SCALE  Scales[MAX_SCALES];											//vector de basculas
DETAIL Detail[MAX_DETAIL];											//vector de detalle
CONFIG Config;														//info configuracion general
APP    App;														//info applicacion

//char definitions
char Msg[SQL_MAX_DATA_LEN];											//log msg

//long
long LockSQL = 0;													//lock sql database 

/*-------------------------------------------------------------------------------
main function
-------------------------------------------------------------------------------*/
void main(int argc,char **argv)
{

	Application::EnableVisualStyles();									//enables windows UI
	Application::SetCompatibleTextRenderingDefault(false);					//set default value for rendering
	Application::Run(gcnew viewer());									//create a new window

}

/*-------------------------------------------------------------------------------
ServerStart
-------------------------------------------------------------------------------*/
void ServerStart(void)
{
	PurgeTable(TABLE_SCALES,"");										//purge scales table
	PurgeTable(TABLE_DETAIL,"");										//purge detail table
	InitTCPServer(&Server,&Clients[0],MAX_TCP_CLIENTS,Config.IP,Config.ServerPort);	//inicializa el servidor de Tcp
}

/*-------------------------------------------------------------------------------
ServerStop
-------------------------------------------------------------------------------*/
void ServerStop(void)
{
	if(Server.Active)TCPCloseServer(&Server);
}

/*-------------------------------------------------------------------------------
ServerStartStop
-------------------------------------------------------------------------------*/
int ServerStartStop(int op)
{
	MessageBoxButtons buttons = MessageBoxButtons::YesNo;
	String^ caption = "WLS Server";     
	String^ message1 = "  ¿Desea Iniciar el servidor?\nDo you want to Start the server?";
	String^ message2 = "  ¿Desea Detener el servidor?\nDo you want to Stop the server?";

	System::Windows::Forms::DialogResult result;
     if(op)result = MessageBox::Show(message1, caption, buttons);
	else result = MessageBox::Show(message2, caption, buttons);

	if (result == ::DialogResult::Yes )
	{	
		if(op)ServerStart();else ServerStop();
		return (1);
	}
	else return (0);
}

/*-------------------------------------------------------------------------------
ApplicationExit
-------------------------------------------------------------------------------*/
void ApplicationExit(void)
{     
	String^ caption = "WLS Server";
	String^ message = "¿Desea Terminar la aplicacion?\nDo you want to finish the program?";     
     MessageBoxButtons buttons = MessageBoxButtons::YesNo;
	if (MessageBox::Show(message, caption, buttons) == ::DialogResult::Yes )
	{
		ServerStop();		
		Log("Sys: Program Terminated");								//write a msg log
		Application::Exit();
	}
}

/*-------------------------------------------------------------------------------
End Code
-------------------------------------------------------------------------------*/

