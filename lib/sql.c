/*
 *
 *  ailsalib: Alisatech library
 *  Copyright (C) 2016 Iain M Conochie <iain-AT-thargoid.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *  
 *  sql.c
 *
 *  Contains sql functions for the ailsalib library
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ailsa.h>
#include <ailsasql.h>

struct ailsa_sql_config_s {
	char *dbtype;
	char *file;
	char *db;
	char *user;
	char *pass;
	char *socket;
	unsigned int magic;
	unsigned int port;
	unsigned long int cliflag;
};

int
ailsa_init_sql(AILSASQL_CONFIG **conf, AILSA_LIST *values)
{
// No where near finished
	int retval = 0;
	if (!(values))
		return -1;
	const char *err_str = "in ailsa_init_sql";
	*conf = ailsa_calloc(sizeof(struct ailsa_sql_config_s), err_str);
	return retval;
}


int
ailsa_sql_run_query(AILSASQL_CONFIG *conf, AILSASQL_QUERY *query, AILSA_LIST *data)
{
	int retval = 0;
	if (!conf || !query || !data)
		return -1;

	return retval;
}

