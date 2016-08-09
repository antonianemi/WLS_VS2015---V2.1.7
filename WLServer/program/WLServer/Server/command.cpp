/*------------------------------------------------------------------------------*
 * command.c	 													*
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
Command
-------------------------------------------------------------------------------*/
int Command(int sock,char *ip,char *cmd)								//process command from client
{
int error;
if(strlen(cmd)<6)return(1);											//verifica longitub de comando
//if(VerifyChk((unsigned char *)cmd))return (1);							//verifica en checksum	//TODO

	error = 1;													//init var with false
	switch(cmd[0])
	{
	case 'S': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = RegisterScale(sock,ip,cmd);							//register a scale	
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;					
	case 'A': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = RegisterAgent(sock,ip,cmd);							//register an agent
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;					
	case 'V': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = VerifyAgent(sock,cmd);								//verify if there a sale of an agent
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;						
	case 'R': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = ReleaseAgent(sock,ip,cmd);								//release an agent at all scales
		error = RegisterAgent(sock,ip,cmd);							//register an agent
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;					
	case 'G': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = GetTicketAll(sock,ip,cmd);								//get ticket of an agent
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;					
	case 'I': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = AddItem(sock,ip,cmd);									//add item to an agent sale
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;						
	case 'K': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = KillTicket(sock,cmd);									//kill ticket of an agent
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;						
	case 'k': 
		SemaphoreOn(&LockSQL);										//lock sql database
		error = KillItem(sock,cmd);									//kill item to an agent sale
		SemaphoreOff(&LockSQL);										//unlock sql database
		break;
	}

return error;
}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/