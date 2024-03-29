/*
 *
 *  libailsa: libailsa library
 *  Copyright (C) 2014-2023  Iain M Conochie <iain-AT-ailsatech.net>
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
 *  Contains definitions for the AILSA_DICT object
 *
 *  Part of the libailsa
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
        size_t size;
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
        data = ailsa_calloc(sizeof(AILSA_DICT), "data in init_kv_s");
        data->key = ailsa_calloc(RBUFF_S, "kv->key in init_kv_s");
        data->value = ailsa_calloc(RBUFF_S, "kv->value in init_kv_s");
        *kv = data;
}

void
clean_kv_s(void *kv)
{
        AILSA_DICT *data = kv;
        if (data->key) {
                memset (data->key, 0, RBUFF_S);
                free(data->key);
        }
        if (data->value) {
                if (data->size)
                        memset(data->value, 0, data->size);
                else
                        memset (data->value, 0, RBUFF_S);
                free(data->value);
        }
        memset (data, 0, sizeof(AILSA_DICT));
        free(data);
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
put_kv_data(AILSA_DICT *kv, void *data, size_t size)
{
        int retval = 0;
        if (kv->value)
                free(kv->value);
        kv->value = data;
        kv->size = size;
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

int
compare_kv(const void *data, const void *cmp)
{
        int retval = 0;
        const AILSA_DICT *orig = data, *new = cmp;
        if ((retval = strcmp(orig->key, new->key)) != 0)
                return retval;
        if (orig->size > 0)
                return memcmp(orig->value, new->value, orig->size);
        else
                return strncmp(orig->value, new->value, RBUFF_S);
}

int
compare_key(const void *data, const void *cmp)
{
        const AILSA_DICT *orig = data, *new = cmp;
        return strcmp(orig->key, new->key);
}

void
create_kv_list(AILSA_LIST **list)
{
        AILSA_LIST *l = ailsa_calloc(sizeof(AILSA_LIST), "list in create_kv_list");
        ailsa_list_init(l, clean_kv_s, compare_key);
        *list = l;
}

void
destroy_kv_list(AILSA_LIST *list)
{
        ailsa_list_destroy(list);
        my_free(list);
}

int
add_to_kv_list(AILSA_LIST *list, const char *key, const char *value)
{
        AILSA_DICT *d;
        void *v;
        int retval = 0;

        init_kv_s(&d);
        if ((retval = put_kv_key(d, key)) != 0)
                return retval;
        if ((ailsa_list_get_member(list, d, &v)) != -1) {
                clean_kv_s(d);
                d = v;
                retval = put_kv_value(d, value);
                return retval;
        }
        if ((retval = put_kv_value(d, value)) != 0)
                return retval;
        retval = ailsa_list_insert_tail(list, d);
        return retval;
}

const char *
get_value_from_kv_list(AILSA_LIST *list, const char *key)
{
        const char *value = NULL;
        AILSA_DICT *d;
        void *v;

        init_kv_s(&d);
        if ((put_kv_key(d, key)) != 0)
                return value;
        if ((ailsa_list_get_member(list, d, &v )) != -1) {
                clean_kv_s(d);
                d = v;
                value = get_kv_value(d);
        } else {
                clean_kv_s(d);
        }
        return value;
}
