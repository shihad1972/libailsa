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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ailsa.h>

const unsigned char
ailsa_b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int
ailsa_b64_nums[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
        36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

int
b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51
};

static int
ailsa_validate_b64_char(char c)
{
/*
 * This is a very basic validation function. It does not handle new lines
 * whitespace, nor does it check that the '=' sign is at the end of the
 * string, nor that there are only 2 '=' signs in the string. It will do
 * for now, however.
 */
        if (c >= '0' && c <= '9')
                return 1;
        if (c >= 'A' && c <= 'Z')
                return 1;
        if (c >= 'a' && c <= 'z')
                return 1;
        if (c == '+' || c == '/' || c == '=')
                return 1;
        return 0;
}

static size_t
ailsa_b64_decode_length(const unsigned char *str)
{
/*
 * Again, this function should probably check there are only a maximum of
 * 2 '=' charaters in the string.
 */
        size_t retval, len, i;
        if (!(str))
                return 0;
        len = strlen((const char *)str);
        if (len % 4) {
                fprintf(stderr, "Incorrect length of input string in ailsa_b64_decode_length\n");
                return 0;
        }
        retval = len / 4 * 3;
        for(i = len; i-- > 0; ) {
                if (str[i] == '=')
                        retval--;
                else
                        break;
        }
        return retval;
}

unsigned char *
ailsa_b64_encode(const unsigned char *str, size_t len)
{
        size_t slen, i, j, v;
        unsigned char *out;

        if (!(str) || (len == 0))
                return NULL;
        slen = (((len / 3) + ((len % 3) ? 1 : 0)) * 4);
        out = ailsa_calloc(slen + 1, "out in ailsa_b64_encode");
        out[slen] = '\0';
        for (i = 0, j = 0; i < len; i+=3, j+=4) {
                v = str[i];
                v = i + 1 < len ? v << 8 | str[i + 1] : v << 8;
                v = i + 2 < len ? v << 8 | str[i + 2] : v << 8;
                out[j] = ailsa_b64_chars[(v >> 18) & 0x3F];
                out[j + 1] = ailsa_b64_chars[(v >> 12) & 0x3F];
                if (i + 1 < len) {
                        out[j + 2] = ailsa_b64_chars[(v >> 6) & 0x3F];
                } else {
                        out[j + 2] = '=';
                }
                if (i + 2 < len) {
                        out[j + 3] = ailsa_b64_chars[(v & 0x3F)];
                } else {
                        out[j + 3] = '=';
                }
        }
        return out;
}

unsigned char *
ailsa_b64_decode(const unsigned char *in)
{
	size_t len, i,j;
	int    v;
        unsigned char *out;

	if (!(in))
		return NULL;
	len = strlen((const char *)in);
	if (len % 4 != 0)
		return NULL;

	for (i=0; i<len; i++) {
		if (!ailsa_validate_b64_char(in[i])) {
			return NULL;
		}
	}
        out = ailsa_calloc(ailsa_b64_decode_length(in) + 1, "out in b64_decode");
	for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[in[i]-43];
		v = (v << 6) | b64invs[in[i+1]-43];
		v = in[i+2]=='=' ? v << 6 : (v << 6) | b64invs[in[i+2]-43];
		v = in[i+3]=='=' ? v << 6 : (v << 6) | b64invs[in[i+3]-43];

		out[j] = (v >> 16) & 0xFF;
		if (in[i+2] != '=')
			out[j+1] = (v >> 8) & 0xFF;
		if (in[i+3] != '=')
			out[j+2] = v & 0xFF;
	}

	return out;
}
