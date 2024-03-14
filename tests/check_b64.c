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
 *  check_b64.c
 *
 *  Test suite definition for base 64 routines
 *
 *  Part of the libailsa
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <check.h>
#include "../include/ailsa.h"

START_TEST(check_b64)
{
        unsigned char *b64, *b64d;
        const unsigned char *str = (const unsigned char *)"This will be the text to encode";
        size_t len = strlen((const char *)str);

        b64 = ailsa_b64_encode(str, len);
        b64d = ailsa_b64_decode(b64);
        ck_assert_ptr_nonnull(b64);
        ck_assert_ptr_nonnull(b64d);
        if(b64d)
                ck_assert_mem_eq(str, b64d, len);
        my_free(b64);
        if (b64d)
                my_free(b64d);

}
END_TEST

Suite *
b64_suite(void)
{
        Suite *s;
        TCase *ailsa;
        s = suite_create("b64");
        ailsa = tcase_create("b64");
        tcase_add_test(ailsa, check_b64);
        suite_add_tcase(s, ailsa);
        return s;
}

int
main(void)
{
        int failed = 0;
        Suite *s;
        SRunner *sr;

        s = b64_suite();
        sr = srunner_create(s);
        srunner_run_all(sr, CK_ENV);
        failed = srunner_ntests_failed(sr);
        srunner_free(sr);
        return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
