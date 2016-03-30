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
 *  generic.c
 *
 *  Contains generic functions for the ailsalib library
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ailsalib.h>

void
show_ailsalib_version()
{
	printf("libailsalib: Version %s\n", AILSAVERSION);
}

void
ailsa_chomp(char *line)
{
	char *p;

	p = strchr(line, '\n');
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

int
ailsa_init_string(AILSA_STRING **str)
{
// I suppose if this does not alloc, I should really return -1 here and set errno
	size_t init = 16384;
	AILSA_STRING *tmp;
	tmp = ailsa_calloc(sizeof(AILSA_STRING), "str in ailsa_init_string");
	tmp->max = init;
	tmp->size = 0;
	*str = tmp;
	return 0;
}

int
ailsa_resize_string(AILSA_STRING *str)
{
	if (!(str))
		return -1;
	size_t len = str->max;
	if (len > (SIZE_MAX / 2))
		return -1;
	len = len ^ 2;
	if (!(str->string = realloc(str->string, len)))
		return -1;
	str->max = len;
	return 0;
}

int
ailsa_free_string(AILSA_STRING *str)
{
	if (!(str))
		return -1;
	if (str->string) {
		memset(str->string, 0, str->size);
		free(str->string);
	}
	memset(str, 0, sizeof(AILSA_STRING));
	free(str);
	return 0;
}

