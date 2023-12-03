/*
 *
 *  libailsa: Alisatech Data library
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
 *  data.c
 *
 *  Contains the functions for data types various AILSA_ data types
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ailsa.h>

// Linked List functions

void
ailsa_list_init(AILSA_LIST *list, void (*destroy)(void *data), int (*cmp)(const void *key1, const void *key2))
{
	list->total = 0;
	list->destroy = destroy;
	list->cmp = cmp;
	list->head = NULL;
	list->tail = NULL;
}

void
ailsa_list_destroy(AILSA_LIST *list)
{
	void *data;

	while (list->total > 0) {
		if (ailsa_list_remove(list, list->tail, (void **)&data) == 0 &&
		    list->destroy != NULL) {
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(AILSA_LIST));
	return;
}

int
ailsa_list_ins_next(AILSA_LIST *list, AILSA_ELEM *element, void *data)
{
	size_t size = sizeof(AILSA_ELEM);
	AILSA_ELEM *new;

	if ((!(element) && list->total != 0) || list->total == SIZE_MAX)
		return -1;
	new = ailsa_calloc(size, "new in ailsa_list_ins_next");
	new->data = data;
	if (list->total == 0) {
		list->head = new;
		list->tail = new;
		new->prev = NULL;
		new->next = NULL;
	} else {
		new->next = element->next;
		new->prev = element;
		if (element->next == NULL)
			list->tail = new;
		else
			element->next->prev = new;
		element->next = new;
	}
	list->total++;
	return 0;
}

int
ailsa_list_ins_prev(AILSA_LIST *list, AILSA_ELEM *element, void *data)
{
	size_t size = sizeof(AILSA_ELEM);
	AILSA_ELEM *new;

	if ((!(element) && list->total != 0) || list->total == SIZE_MAX)
		return -1;
	new = ailsa_calloc(size, "new in ailsa_list_ins_prev");
	new->data = data;
	if (list->total == 0) {
		list->head = new;
		list->tail = new;
		new->prev = NULL;
		new->next = NULL;
	} else {
		new->next = element;
		new->prev = element->prev;
		if (element->prev == NULL)
			list->head = new;
		else
			element->prev->next = new;
		element->prev = new;
	}
	list->total++;
	return 0;
}

int
ailsa_list_insert_head(AILSA_LIST *list, void *data)
{
	AILSA_ELEM *tmp;
	int retval = 0;
	if (!(list) || !(data))
		return -1;
	if (list->total == SIZE_MAX)
		return -1;
	if (list->total == 0) {
		retval = ailsa_list_ins_next(list, NULL, data);
	} else {
		AILSA_ELEM *new = ailsa_calloc(sizeof(AILSA_ELEM), "new in ailsa_list_insert");
		new->data = data;
		tmp = (AILSA_ELEM *)list->head;
		new->next = tmp;
		new->prev = NULL;
		tmp->prev = new;
		list->head = new;
		list->total++;
	}
	return retval;
}

int
ailsa_list_insert_tail(AILSA_LIST *list, void *data)
{
	AILSA_ELEM *tmp;
	int retval = 0;
	if (!(list) || !(data))
		return -1;
	if (list->total == SIZE_MAX)
		return -1;
	if (list->total == 0) {
		retval = ailsa_list_ins_next(list, NULL, data);
	} else {
		AILSA_ELEM *new = ailsa_calloc(sizeof(AILSA_ELEM), "new in ailsa_list_insert");
		new->data = data;
		tmp = (AILSA_ELEM *)list->tail;
		tmp->next = new;
		new->prev = list->tail;
		new->next = NULL;
		list->tail = new;
		list->total++;
	}
	return retval;
}

int
ailsa_list_remove(AILSA_LIST *list, AILSA_ELEM *element, void **data)
{
	if (!(element) || list->total == 0)
		return -1;
	*data = element->data;
	if (element == list->head) {
		list->head = element->next;
		if (!(list->head))
			list->tail = NULL;
		else
			element->next->prev = NULL;
	} else {
		element->prev->next = element->next;
		if (!(element->next))
			list->tail = element->prev;
		else
			element->next->prev = element->prev;
	}
	my_free(element);
	list->total--;
	return 0;
}

int
ailsa_list_position(AILSA_LIST *list, size_t *pos, const void *data)
{
	AILSA_ELEM *elem = list->head;
	if (!(data))
		return -1;
	while (list->cmp(elem->data, data) != 0) {
		(*pos)++;
		if (!(elem->next))
			return -1;
		elem = elem->next;
	}
	return 0;
}

// Hash tables functions

// Hash a character string key
unsigned int
ailsa_hash(const void *key)
{
	const char	*ptr;
	unsigned int	val, tmp;

	val = 0;
	ptr = key;
	while (*ptr != '\0') {
		val = (val << 4) + (unsigned)(*ptr);
		if ((tmp = (val & 0xf0000000))) {
			val = val ^ (tmp >> 24);
			val = val ^ tmp;
		}
		ptr++;
	}
	return val;
}

/* For dozies that do not understand hash tables
	table:		array of linked lists
	buckets:	total number of linked lists
	size:		current number of buckets that have at least 1 entry
	h:		hashing function (you can use the one above, ailsa_hash)
	match:		used in searching the linked list
	destroy:	removes an individual linked list
 */
int
ailsa_hash_init(AILSA_HASH *htbl, unsigned int buckets,
		unsigned int (*h)(const void *key),
		int (*match)(const void *key1, const void *key2),
		int (*cmp)(const void *key1, const void *key2),
		void (*destroy)(void *data))
{
	unsigned int i;

	htbl->table = ailsa_calloc(buckets * sizeof(AILSA_LIST), "htbl->table in ailsa_hash_init");
	htbl->buckets = buckets;
	htbl->size = 0;
	for (i = 0; i < buckets; i++)
		ailsa_list_init(&htbl->table[i], destroy, cmp);
	htbl->h = h;
	htbl->match = match;
	htbl->destroy = destroy;
	return 0;
}

void
ailsa_hash_destroy(AILSA_HASH *htbl)
{
	unsigned int i;

	for (i = 0; i < htbl->buckets; i++)
		ailsa_list_destroy(&htbl->table[i]);
	my_free(htbl->table);
	memset(htbl, 0, sizeof(AILSA_HASH));
}

int
ailsa_hash_insert(AILSA_HASH *htbl, void *data, const char *key)
{
	unsigned int	bucket;
	int		retval;

	if (ailsa_hash_lookup(htbl, &data, key) == 0)
		return 1;
	bucket = htbl->h(key) % htbl->buckets;
	if ((retval = ailsa_list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
		htbl->size++;
	return retval;
}

int
ailsa_hash_remove(AILSA_HASH *htbl, void **data, const char *key)
{
	AILSA_ELEM 	*em, *prev;
	unsigned int	bucket;

	bucket = htbl->h(key) % htbl->buckets;
	prev = NULL;
	for (em = htbl->table[bucket].head; em != NULL; em = em->next) {
		if (htbl->match(*data, em->data)) {
			if (ailsa_list_remove(&htbl->table[bucket], prev, data) == 0) {
				htbl->size--;
				return 0;
			} else {
				return 1;
			}
		}
		prev = em;
	}
	return -1;
}

int
ailsa_hash_lookup(AILSA_HASH *htbl, void **data, const char *key)
{
	AILSA_ELEM 	*em;
	unsigned int	bucket;

	bucket = htbl->h(key) % htbl->buckets;
	for (em = htbl->table[bucket].head; em != NULL; em = em->next) {
		if (htbl->match(*data, em->data)) {
			*data = em->data;
			return 0;
		}
	}
	return -1;
}

int
ailsa_init_string(AILSA_STRING **str)
{
// I suppose if this does not alloc, I should really return -1 here and set errno
	size_t init = STRING_S;
	AILSA_STRING *tmp;
	tmp = ailsa_calloc(sizeof(AILSA_STRING), "str in ailsa_init_string");
	tmp->string = ailsa_calloc(init, "str->string in ailsa_init_string");
	tmp->max = init;
	tmp->size = 0;
	*str = tmp;
	return 0;
}

int
ailsa_add_to_string(AILSA_STRING *str, const char *string)
{
	int retval = 0;
	size_t len = strlen(string);
	size_t total = len + str->size;
	if (total >= str->max)
		if ((retval = ailsa_resize_string(str)) != 0)
			return retval;
	sprintf(str->string + str->size, "%s", string);
	str->size = total;
	return retval;
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

void
ailsa_free_string(void *data)
{
	AILSA_STRING *str = data;
	if (!(str))
		return;
	if (str->string) {
		memset(str->string, 0, str->size);
		free(str->string);
	}
	memset(str, 0, sizeof(AILSA_STRING));
	free(str);
}

int
ailsa_compare_string(const void *str, const void *cmp)
{
	const AILSA_STRING *string = str;
	const char *compare = cmp;
	return strncmp(string->string, compare, string->size);
}
