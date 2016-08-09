/*------------------------------------------------------------------------------*
 * misc.c		 													*
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
GetLocalIP
-------------------------------------------------------------------------------*/
int GetLocalIP(char *str)
{
unsigned char b1, b2, b3, b4;
char szBuffer[1024];
WSADATA wsaData;

WORD wVersionRequested = MAKEWORD(2, 0);
if(::WSAStartup(wVersionRequested, &wsaData) != 0) return false;    
if(gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
{
	WSACleanup();
     return false;
}
struct hostent *host = gethostbyname(szBuffer);
if(host == NULL)
{
	WSACleanup();
     return false;
}

//Obtain the computer's IP
b1 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
b2 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
b3 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
b4 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;
sprintf(str,"%i.%i.%i.%i",b1,b2,b3,b4);
WSACleanup();
return true;
}

/*------------------------------------------------------------------------------*
check if file exist
-------------------------------------------------------------------------------*/
int FileExist(char *a)												//check if file exist
{
FILE *hFile;
if((hFile=fopen(a,"r")) == NULL)return (0);								//if the file not exist return 0
fclose(hFile);														//close the file
return (1);														//return 1
}

/*------------------------------------------------------------------------------*
reads a file record
-------------------------------------------------------------------------------*/
void FileRead(FILE *f,char *a)										//reads a file record
{
char *str;
do{
	str = fgets(a, 255, f);											//read a line of the file
}while ((a[0] == '/' || a[0] == '\n'));									//ignore this line
}

/*------------------------------------------------------------------------------*
reads a file record
-------------------------------------------------------------------------------*/
void FileWrite(FILE *f,char *a)										//writes a file record
{
fputs(a,f);														//write at a file
}

/*------------------------------------------------------------------------------*
sscanf_tab
-------------------------------------------------------------------------------*/
int sscanf_tab(char *src,char *format, ... )								//escanea entrada de archivo para extraer datos
{   
va_list ap;   
float *f;   
char  *a;
int  *i;
char buf[255] = {'\0'};
char *fp, *sp = src;     
int conv = 0, index;   
va_start ( ap, format );   
for ( fp = format; *fp != '\0'; fp++ ) 
{     	
	for ( index = 0; *sp != '\0' && *sp != 0x09 && *sp != 10; index++) buf[index] = *sp++;     	
	buf[index] = 0;
	while ( *sp == 0x09 || *sp == 10) sp++;     
	while ( *fp != '%' ) fp++;     
	if ( *fp == '%' ) 
	{       
		switch ( *++fp ) 
		{       
			case 'd':
			case 'i':         				
				i = va_arg ( ap, int * );         
				*i = atoi ( buf );         
			break;       
			case 'f':         
				f = va_arg ( ap, float * );
				*f = (float)atof ( buf );         
			break;       
			case 's':         
				a = va_arg ( ap, char * );				
				strncpy ( a, buf, strlen ( buf ) + 1 );				
			break;       
		}       
		conv++;     
	}	
}   
va_end ( ap );   
return conv; 
} 

/*------------------------------------------------------------------------------
Checksum
------------------------------------------------------------------------------*/
unsigned char CheckSum(unsigned char *ss)
{
unsigned long s,r;
unsigned int c,n,k;
unsigned char chk;

s = 0;r = 0;n = 0;c = 2;												//inicializa variables
n = strlen((char*)ss);												//no considera checksum y enter en ultimas posiciones
for(k=0;k<n;k++)													//Solo la cadena de caracteres valida
{
	r = *(ss+k) * c;												//el valor de la posición por el contador de posición
	s = s + r;													//se suma los valores
	c++;															//se incrementa la posición
	if(c==9)c = 2;													//9 es el máximo contador de posición
}
r = (s * 10) % 11;													//mod 11
r += 48;
chk = (unsigned char)r ;												//checksum como unsigned char

return (chk); 														//regresa checksum
}

/*------------------------------------------------------------------------------
Verificar el checksum
------------------------------------------------------------------------------*/
int  VerifyChk(unsigned char *str)
{
unsigned char chk_1;
unsigned char chk_2;

chk_1 = *(str+strlen((char*)str)-2);									//extrae el chk
*(str+strlen((char*)str)-2) = 0;										//trunca la cadena
chk_2 = CheckSum(str);												//calcula el chk
*(str+strlen((char*)str)) = chk_1;										//repone el chk

if(chk_1==chk_2)return 0;											//chk ok
{
	printf("Chk1: %c\n",chk_1);
	printf("Chk2: %c\n",chk_2);
	return 1;														//chk error
}
}

/*-----------------------------------------------------------------------------*
RegCpy
-------------------------------------------------------------------------------*/
void RegCpy(int lf,char *src,char *dst,int ini)
{
int n,c,i;
if(lf==0)return;
lf--;
n = strlen(src);
c = ini;

while(lf && c<n){if(src[c]==0xa)lf--;c++;}								//busca el registro
i = 0;
while(src[c]!=0xa && c<n)
{
	dst[i] = src[c];												//copia la fuente a destino
	i++;
	c++;
}
dst[i] = 0;
}

/*-----------------------------------------------------------------------------*
RegStr
------------------------------------------------------------------------------*/
void RegStr(int tab,int lf,char *src,char *dst,int ini)
{
int n,c,i;
if(tab==0 || lf==0)return;
tab--;
lf--;
n = strlen(src);
c = ini;

while(lf && c<n){if(src[c]==0xa)lf--;c++;}								//busca el registro
while(tab && c<n){if(src[c]==0x9)tab--;c++;}								//busca el campo

i = 0;
while(src[c]!=0x9 && c<n)
{
	dst[i] = src[c];												//copia la fuente a destino
	i++;
	c++;
}
dst[i] = 0;

}

/*-----------------------------------------------------------------------------*
RegFloat
------------------------------------------------------------------------------*/
float RegFloat(int tab,int lf,char *s,int ini)
{
char dato[16];
float resp = 0;
int n,c,i;

if(tab==0 || lf==0)return (0);
tab--;
lf--;
strcpy(dato,"");
n = strlen(s);
c = ini;

while(lf && c<n){if(s[c]==0xa)lf--;c++;}								//busca el registro
while(tab && c<n){if(s[c]==0x9)tab--;c++;}								//busca el campo

i = 0;
while(s[c]!=0x9 && c<n)
{
	dato[i] = s[c];
	i++;
	c++;
}
dato[i] = 0;
resp = (float) atof(dato);
return resp;
}

/*-----------------------------------------------------------------------------*
RegLong
------------------------------------------------------------------------------*/
long RegLong(int tab,int lf,char *s,int ini)
{
char dato[16] = { 0 };
long resp = 0;
int n,c,i;

if(tab==0 || lf==0)return (0);
tab--;
lf--;
strcpy(dato,"");
n = strlen(s);
c = ini;

while(lf && c<n){if(s[c]==0xa)lf--;c++;}								//busca el registro
while(tab && c<n){if(s[c]==0x9)tab--;c++;}								//busca el campo

i = 0;
while(s[c]!=0x9 && c<n)
{
	dato[i] = s[c];
	i++;
	c++;
}
dato[i] = 0;
resp = atol(dato);
return resp;
}

/*-----------------------------------------------------------------------------*
RegInt
------------------------------------------------------------------------------*/
int  RegInt(int tab,int lf,char *s,int ini)
{
char dato[16];
int resp = 0;
int n,c,i;

if(tab==0 || lf==0)return (0);
tab--;
lf--;
strcpy(dato,"");
n = strlen(s);
c = ini;

while(lf && c<n){if(s[c]==0xa)lf--;c++;}								//busca el registro
while(tab && c<n){if(s[c]==0x9)tab--;c++;}								//busca el campo

i = 0;
while(s[c]!=0x9 && c<n)
{
	dato[i] = s[c];
	i++;
	c++;
}
dato[i] = 0;
resp = atoi(dato);
return resp;
}

/*-----------------------------------------------------------------------------*
PrintScales function
------------------------------------------------------------------------------*/
void PrintScales(void)
{
	int i;
	i = 0;
	printf("\nScales\n");
	while(Scales[i].Id)
	{
		printf(" %02ld <%s> <%s> %ld %ld <%s>\n",Scales[i].Id,Scales[i].Serie,Scales[i].IP,Scales[i].Agent,Scales[i].Status,Scales[i].Time);
		i++;
	}
}

/*-----------------------------------------------------------------------------*
PrintAgents function
------------------------------------------------------------------------------*/
void PrintAgents(void)
{/*
	int i;
	i = 0;
	printf("\nAgents\n");
	while(Agents[i].Id)
	{
		printf(" %02ld <%s> %ld <%s>\n",Agents[i].Id,Agents[i].Name,Agents[i].Status,Agents[i].Time);
		i++;
	}*/
}

/*-----------------------------------------------------------------------------*
PrintTickets function
------------------------------------------------------------------------------*/
void PrintTickets(void)
{/*
	int i;
	i = 0;
	printf("\nTickets\n");
	while(Tickets[i].Id)
	{
		printf(" %02ld <%s> %ld <%s>\n",Tickets[i].Id,Tickets[i].Content,Tickets[i].Status,Tickets[i].Time);
		i++;
	}*/
}

/*-----------------------------------------------------------------------------*
PrintDetail function
------------------------------------------------------------------------------*/
void PrintDetail(void)
{
	int i;
	i = 0;
	printf("\nDetail\n");
	while(Detail[i].Id)
	{
		printf(" %02ld %02ld <%s> %ld <%s>\n",Detail[i].Id,Detail[i].Line,Detail[i].Content,Detail[i].Status,Detail[i].Time);
		i++;
	}
}

/*-----------------------------------------------------------------------------*
InitVars function
------------------------------------------------------------------------------*/
void InitVars(void)
{
memset(&Clients,0,sizeof(TCPCLIENT)*MAX_TCP_CLIENTS);						//inicializa estructura app
memset(&Scales,0,sizeof(SCALE)*MAX_SCALES);								//inicializa estructura basculas
memset(&Detail,0,sizeof(DETAIL)*MAX_DETAIL);								//inicializa estructura tickets
memset(&Config,0,sizeof(CONFIG));										//inicializa estructura config
memset(&App,0,sizeof(APP));											//inicializa estructura app

memset(Msg,0,SQL_MAX_DATA_LEN);										//log msg

LockSQL = 0;														//lock sql database 

strcpy(App.Name,"WLS Server");										//app's name
strcpy(App.Root,PATH_ROOT);											//app's rooth
App.Width = DISPLAY_WIDTH;	//(glutGet(GLUT_SCREEN_WIDTH)/4)*3;			//window width
App.Height = DISPLAY_HEIGHT;	//(glutGet(GLUT_SCREEN_HEIGHT)/4)*3;			//window height

}

/*------------------------------------------------------------------------------*
End Code
-------------------------------------------------------------------------------*/