/*
 * tables.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Gregory G.
 */


#ifndef TABLE_H_
#define TABLE_H_

//#include "const.h"

/*
-- Describe SCALES
CREATE TABLE scales (
    "Id" INTEGER,
    "Serie" TEXT,
    "Status" INTEGER,
    "Time" TEXT
)
*/
const int SCALE_TYPES[7]=	{TYPE_LONG,	TYPE_CHAR16,	TYPE_LONG,	TYPE_CHAR32,	TYPE_LONG,	TYPE_CHAR16,	0};
const int SQL_SCALE_TYPES[7]=	{SQL_TYPE_ID,	SQL_TYPE_TEXT,	SQL_TYPE_INT,	SQL_TYPE_TEXT,	SQL_TYPE_INT,	SQL_TYPE_TEXT,	SQL_TYPE_NULL};
const char *SQL_SCALE_FIELDS[7]=
{
    "Id",		//INTEGER,
    "Serie",	//TEXT,    
    "Status",	//INTEGER,
    "Time",	//TEXT
    "Agent",	//INTEGER
    "IP",		//TEXT,
    "\0"
};


/*
-- Describe AGENTS
CREATE TABLE agents (
    "Id" INTEGER,
    "Name" TEXT,
    "Status" INTEGER,
    "Time" TEXT
)
*/
const int AGENT_TYPES[5]={TYPE_LONG,TYPE_CHAR64,TYPE_LONG,TYPE_CHAR32,0};
const int SQL_AGENT_TYPES[5]={SQL_TYPE_ID,SQL_TYPE_TEXT,SQL_TYPE_INT,SQL_TYPE_TEXT,SQL_TYPE_NULL};
const char *SQL_AGENT_FIELDS[5]=
{
    "Id",		//INTEGER,
    "Name",	//TEXT,
    "Status",	//INTEGER,
    "Time",	//TEXT
    "\0"
};



/*
-- Describe TICKET
CREATE TABLE "ticket" (
    "Id_Agent" INTEGER,
    "Content" TEXT,
    "Status" INTEGER,
    "Time" TEXT
)
*/
const int TICKET_TYPES[5]={TYPE_LONG,TYPE_CHAR128,TYPE_LONG,TYPE_CHAR32,0};
const int SQL_TICKET_TYPES[5]={SQL_TYPE_ID,SQL_TYPE_TEXT,SQL_TYPE_INT,SQL_TYPE_TEXT,SQL_TYPE_NULL};
const char *SQL_TICKET_FIELDS[5]=
{
    "Id_Agent",		//INTEGER,
    "Content",	//TEXT,
    "Status",	//INTEGER,
    "Time",	//TEXT
    "\0"
};



/*
-- Describe DETAIL
CREATE TABLE "detail" (
    "Id" INTEGER,
    "Line" INTEGER,
    "Content" TEXT,
    "Status" INTEGER,
    "Time" TEXT
)
*/
const int DETAIL_TYPES[6]=	{TYPE_LONG,	TYPE_CHAR128,	TYPE_LONG,	TYPE_CHAR32,	TYPE_LONG,	0};
const int SQL_DETAIL_TYPES[6]={SQL_TYPE_ID,	SQL_TYPE_TEXT,	SQL_TYPE_INT,	SQL_TYPE_TEXT,	SQL_TYPE_INT,	SQL_TYPE_NULL};
const char *SQL_DETAIL_FIELDS[6]=
{
    "Id_Agent",//INTEGER,    
    "Content",	//TEXT,
    "Status",	//INTEGER,
    "Time",	//TEXT
    "Line",	//INTEGER,
    "\0"
};


#endif /* TABLES_H */
