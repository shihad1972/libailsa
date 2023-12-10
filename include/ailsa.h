/*
 *  ailsalib: Ailsatech library
 *  Copyright (C) 2012 - 2016  Iain M Conochie <iain-AT-ailsatech.net>
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
 *  ailsalib.h: Main ailsalib library header file
 */

#ifndef __AILSA_H__
# define __AILSA_H__
# include <stdlib.h>
# include <time.h>
/** Useful macro to safely avoid double-free memory corruption
 ** Shamelessly stolen from the nagios source. Thanks :) */
# ifndef my_free
#  define my_free(ptr) do { if(ptr) { free(ptr); ptr = NULL; } } while(0)
# endif // my_free

# ifndef true
#  define true 1
# endif // true
# ifndef false
#  define false 0
# endif // false

# ifndef NI_MAXHOST
#  define NI_MAXHOST 1025
# endif // NI_MAXHOST so I do not have to use __GNU_SOURCE. grrrr 

# define AILSAVERSION "0.1"

// Data Definitions

enum {			/* regex search codes */
	UUID_REGEX = 0,
	NAME_REGEX,
	ID_REGEX,
	CUSTOMER_REGEX,
	COID_REGEX,
	MAC_REGEX,
	IP_REGEX,
	DOMAIN_REGEX,
	PATH_REGEX,
	LOGVOL_REGEX,
	FS_REGEX,
	ADDRESS_REGEX,
	DEV_REGEX,
	CAPACITY_REGEX,
	OS_VER_REGEX,
	POSTCODE_REGEX,
	URL_REGEX,
	PHONE_REGEX,
	EMAIL_REGEX,
	TXTRR_REGEX,
	CN_REGEX,
	DC_REGEX
};

enum {		// Some string length constants
	CH_S = 2,
	COMM_S = 8,
	RANGE_S = 16,
	MAC_S = 32,
	HOST_S = 64,
	NAME_S = 128,
	RBUFF_S = 256,
	TBUFF_S = 512,
	BUFF_S = 1204,
	FILE_S = 4096,
	STRING_S = 16384,
	BUILD_S = 65536
};

enum {                  // Error codes
        AILSA_NO_ACTION = 1,
	AILSA_MALLOC = 99,
        AILSA_NO_DATA = 200,
        AILSA_NO_CONNECT = 201,
	AILSA_TRUNCATE = 202,
	AILSA_DATA_EXISTS = 203
};

// Linked List data types

typedef struct ailsa_element_s {
	struct	ailsa_element_s *prev;
	struct	ailsa_element_s *next;
	void	*data;
} AILSA_ELEM;

typedef struct ailsa_list_s {
	size_t 	total;
	int 	(*cmp)(const void *key1, const void *key2);
	void 	(*destroy)(void *data);
	AILSA_ELEM 	*head;
	AILSA_ELEM 	*tail;
} AILSA_LIST;

// Hash table types

typedef struct ailsa_hash_s {
	unsigned int	buckets;
	unsigned int	(*h)(const void *key);
	int		(*match)(const void *key1, const void *key2);
	void		(*destroy)(void *data);
	unsigned int	size;
	AILSA_LIST	*table;
} AILSA_HASH;

typedef struct ailsa_str_s {
	char *string;
	size_t max;
	size_t size;
} AILSA_STRING;

// Opaque types

typedef struct ailsa_kv_s AILSA_DICT;

// AILSA_ data functions;

// Linked List
void
ailsa_list_init(AILSA_LIST *list, void (*destroy)(void *data), int (*cmp)(const void *key1, const void *key2));

void
ailsa_list_destroy(AILSA_LIST *list);

int
ailsa_list_ins_next(AILSA_LIST *list, AILSA_ELEM *element, void *data);

int
ailsa_list_ins_prev(AILSA_LIST *list, AILSA_ELEM *element, void *data);

int
ailsa_list_insert_head(AILSA_LIST *list, void *data);

int
ailsa_list_insert_tail(AILSA_LIST *list, void *data);

int
ailsa_list_remove(AILSA_LIST *list, AILSA_ELEM *element, void **data);

int
ailsa_list_position(AILSA_LIST *list, size_t *pos, const void *data);

int
ailsa_list_get_member(AILSA_LIST *list, const void *data, void **member);

// Hash Table

unsigned int
ailsa_hash(const void *key);

int
ailsa_hash_init(AILSA_HASH *htbl, unsigned int buckets,
		unsigned int (*h)(const void *key),
		int (*match)(const void *key1, const void *key2),
		int (*cmp)(const void *key1, const void *key2),
		void (*destroy)(void *data));

void
ailsa_hash_destroy(AILSA_HASH *htbl);

int
ailsa_hash_insert(AILSA_HASH *htbl, void *data, const char *key);

int
ailsa_hash_remove(AILSA_HASH *htbl, void **data, const char *key);

int
ailsa_hash_lookup(AILSA_HASH *htbl, void **data, const char *key);

/*
 * AILSA_STRING is to allow easy resizing of a string buffer.
 * Initialise with ailsa_init_string, and destroy with 
 * ailsa_free_string.
 */

int
ailsa_init_string(AILSA_STRING **str);

int
ailsa_add_to_string(AILSA_STRING *str, const char *string);

int
ailsa_resize_string(AILSA_STRING *str);

void
ailsa_free_string(void *data);

int
ailsa_compare_string(const void *str, const void *cmp);

// Path and various string functions

int
ailsa_add_trailing_dot(char *string);

int
ailsa_add_trailing_slash(char *string);

void
ailsa_chomp(char *line);

void
ailsa_munch(char *line);

// Memory functions

void *
ailsa_calloc(size_t len, const char *msg);

// Logging functions

void
ailsa_start_syslog(const char *prog);

void
ailsa_syslog(int priority, const char *msg, ...);

void
ailsa_show_error(int retval);

// Networking Functions

int
ailsa_tcp_bind(const char *node, const char *service);

int
ailsa_tcp_connect(const char *node, const char *service);
 
int
ailsa_tcp_accept(int sock);

int
ailsa_get_fqdn(char *host, char *fqdn, char *ip);

int
ailsa_handle_send_error(int error);

int
ailsa_handle_recv_error(int error);

int
ailsa_send_response(int client, char *buf);

int
ailsa_do_close(int client, char *buf);

// File / Directory IO helper functions

int
check_directory_access(const char *dir, int create);

int
ailsa_write_file(const char *name, void *data, size_t len);

int
ailsa_read_file(const char *name, void *data, size_t len);

int
ailsa_append_file(const char *name, void *data, size_t len);

// Input validation
 
int
ailsa_validate_input(char *input, int test);

// KV Functions and types

void
init_kv_s(AILSA_DICT **kv);

void
clean_kv_s(void *kv);

int
put_kv_value(AILSA_DICT *kv, const char *value);

int
put_kv_key(AILSA_DICT *kv, const char *name);

// Be careful with the one. Only pass in a piece of data the can be
// free()ed in one go. Any pointers in the data will not get released.
int
put_kv_data(AILSA_DICT *kv, void *data, size_t size);

const char *
get_kv_key(AILSA_DICT *kv);

const char *
get_kv_value(AILSA_DICT *kv);

const void *
get_kv_data(AILSA_DICT *kv);

int
compare_kv(const void *data, const void *cmp);

int
compare_key(const void *data, const void *cmp);

// End KV Functions

#endif // __AILSA_H__
