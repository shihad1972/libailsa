/*
 *  ailsalib: Ailsatech library
 *  Copyright (C) 2012 - 2016  Iain M Conochie <iain-AT-thargoid.co.uk>
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
 *  ailsasql.h: Main ailsalib SQL library header file
 */

#ifndef __AILSASQL_H__
# define __AILSASQL_H__
# include <stddef.h>
# include <ailsalib.h>

enum {		// sql types
	ASQL_INT = 1,
	ASQL_TXT = 2,
	ASQL_SHRT = 3,
	ASQL_BOOL = 4,
	ASQL_BIN = 5
};

typedef struct ailsa_sql_s {
	const char *query;
	size_t args;
	size_t fields;
	unsigned short int *arg_type;
	unsigned short int *field_type;
	unsigned long int updated;
} AILSASQL_QUERY;

typedef struct ailsa_sql_elem_s {
	char *field;
	void *value;
} AILSASQL_ELEM;

typedef struct ailsa_sql_config_s AILSASQL_CONFIG;

int
ailsa_init_sql(AILSASQL_CONFIG **conf, AILSA_LIST *values);

int
ailsa_sql_run_query(AILSASQL_CONFIG *conf, AILSASQL_QUERY *query, AILSA_LIST *data);

# endif // __AILSASQL_H__
