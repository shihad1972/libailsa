/*
 *
 *  libailsa: Alisatech library
 *  Copyright (C) 2016 Iain M Conochie <iain-AT-ailsatech.net>
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
 *  generic.c
 *
 *  Contains generic functions for the libailsa library
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <ailsa.h>

void
show_libailsa_version()
{
	printf("libailsa: Version %s\n", AILSAVERSION);
}

void
ailsa_chomp(char *line)
{
	char *p;

	p = strrchr(line, '\n');
	if (p)
		*p = '\0';
}

void
ailsa_munch(char *line)
{
	char *p;

	p = strstr(line, "\r\n");
	if (p)
		*p = '\0';
}

void *
ailsa_calloc(size_t len, const char *msg)
{
	void *p;

	if (!(p = calloc(len, sizeof(char)))) {
		perror(msg);
		exit(errno);
	}
	return p;
}

int
ailsa_add_trailing_slash(char *member)
{
	size_t len;
	int retval;
	
	retval = 0;
	len = strlen(member);
	if ((member[len - 1] != '/') && len < (RBUFF_S - 2)) {
		member[len] = '/';
		member[len + 1] = '\0';
	} else if (member[len - 1] == '/') {
		retval = 0;
	} else {
		retval = -1;
	}
	
	return retval;
}

int
ailsa_add_trailing_dot(char *member)
{
// Maximum input string size is 254 bytes with no trailing /
	size_t len;
	int retval;
	
	retval = 0;
	if ((len = strlen(member)) > (RBUFF_S - 2))
		return -1;
	if (member[len - 1] != '.') {
		member[len] = '.';
		member[len +1] = '\0';
	} else if (member[len - 1] == '.') {
		retval = 0;
	} else {
		retval = -1;
	}
	return retval;
}
