/*
 *
 *  ldap-col: collection of ldap utilities
 *  Copyright (C) 2014-2015  Iain M Conochie <iain-AT-thargoid.co.uk>
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
 *  ailsa_kv_s.c
 *
 *  Shared function defintions for the ailsa ldap library
 *
 *  Part of the ldap collection suite of program
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <ctype.h>
#include <ailsa.h>

typedef struct ailsa_kv_s {
        char *key;
        void *value;
} AILSA_DICT;

/* 
 * Key values pairs are limited to 255 characters
 *
 * If you need more, get a life, and get a better naming scheme :P
 *
 */

void
init_kv_s(AILSA_DICT **kv)
{
        AILSA_DICT *data;
        if (!(data = calloc(sizeof(AILSA_DICT), sizeof(char))))
                error(AILSA_MALLOC, errno, "data in init_kv_s");
	if (!(data->key = calloc(RBUFF_S, sizeof(char))))
		error(AILSA_MALLOC, errno, "kv->key in init_kv_s");
	if (!(data->value = calloc(RBUFF_S, sizeof(char))))
		error(AILSA_MALLOC, errno, "kv->values in init_kv_s");
        *kv = data;
}

void
clean_kv_s(AILSA_DICT **kv)
{
        AILSA_DICT *data = *kv;
        if (data->key) {
                memset (data->key, 0, RBUFF_S);
                free(data->key);
        }
        if (data->value) {
                memset (data->value, 0, RBUFF_S);
                free(data->value);
        }
        memset (data, 0, sizeof(AILSA_DICT));
        free(data);
        *kv = NULL;
}

int
put_kv_key(AILSA_DICT *kv, const char *key)
{
        int retval = 0;
        if (!(kv->key))
                return AILSA_NO_DATA;
        if (snprintf(kv->key, RBUFF_S, "%s", key) >= RBUFF_S) {
                ailsa_show_error(AILSA_TRUNCATE);
                retval = AILSA_TRUNCATE;
        }
        return retval;
}

int
put_kv_value(AILSA_DICT *kv, const char *value)
{
        int retval = 0;
        if (!(kv->value))
                return AILSA_NO_DATA;
        if (snprintf(kv->value, RBUFF_S, "%s", value) >= RBUFF_S) {
                ailsa_show_error(AILSA_TRUNCATE);
                retval = AILSA_TRUNCATE;
        }
        return retval;
}

int
put_kv_data(AILSA_DICT *kv, void *data)
{
        int retval = 0;
        if (kv->value)
                return AILSA_DATA_EXISTS;
        kv->value = data;
        return retval;
}

const char *
get_kv_key(AILSA_DICT *kv)
{
        char *key = '\0';

        key = kv->key;
        return key;
}

const char *
get_kv_value(AILSA_DICT *kv)
{
        char *value = '\0';

        value = kv->value;
        return value;
}

const void *
get_kv_data(AILSA_DICT *kv)
{
        void *data = '\0';

        data = kv->value;
        return data;
}
