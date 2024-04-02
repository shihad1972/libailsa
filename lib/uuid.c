/*
 *
 *  libailsa: Alisatech library
 *  Copyright (C) 2015 Iain M Conochie <iain-AT-ailsatech.net>
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
 *  uuid.c
 *
 *  Contains helper functions for UUID
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <uuid.h>
#include <ailsa.h>

char *
ailsa_gen_uuid(void)
{
	uuid_t	uuid;
	char 	*out = NULL;

	if (!(out = ailsa_calloc(64, "out in ailsa_get_uuid")))
		return NULL;
	uuid_generate(uuid);
	uuid_unparse_lower(uuid, out);
	return out;
}

