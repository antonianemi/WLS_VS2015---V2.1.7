/*
 * os.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef OS_H_
#define OS_H_


//operative systems constants
#define FILE_COMPILE			__FILE__								//compile file
#define DATE_COMPILE			__DATE__								//compile date
#define TIME_COMPILE			__TIME__								//compile time
#define DEBUG					0									//debug mode


//operative system
#define OS_UBUNTU				1									//linux
#define OS_WINDOWS				2									//windows
#define OS_OSX_MAC				3									//osx
#define OS_ANDROID				4									//android


//platform
#define TYPE_OS				OS_WINDOWS							//default operative system


//OS exeptions
#if TYPE_OS == OS_WINDOWS											//if its compilen on windows
#pragma warning(disable: 4793)										//disable warning funcion sscanf
#pragma warning(disable: 4996)										//disable warning funcion strcpy
#define PATH_ROOT				"C:/PROYECTOS/WLS_VS2012/WLServer/"	//root directory
#include <winsock2.h>												//socket libraries
#endif

#if TYPE_OS == OS_UBUNTU												//if its compilen on ubuntu
#define PATH_ROOT				"/home/gregory/WLServer/"				//root directory
#endif

#if TYPE_OS == OS_OSX_MAC											//if its compilen on OSX
#define PATH_ROOT				"/WLServer/"							//root directory
#endif

#if TYPE_OS == OS_ANDROID											//if its compilen on android
#define PATH_ROOT				"/WLServer/"							//root directory
#endif


//subdirectories
#define PATH_LOG				PATH_ROOT "log/"						//logs directory
#define PATH_DATA				PATH_ROOT "data/"						//data directory
#define PATH_LENG				PATH_ROOT "idiom/"						//lenguage directory
#define PATH_TEXTURES			PATH_ROOT "textures/"					//images directory


#endif /* OS_H */
