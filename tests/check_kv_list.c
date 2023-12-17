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
 *  check_kv_list.c
 *
 *  Test suite definition for AILSA_DICT object
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

START_TEST(check_kv_list)
{
        const char *path = "./mock.txt";
        const char *server = "ticktock.example.org";
        const char *base = "dc=example,dc=org";
        const char *s;
        char l[TBUFF_S], k[RBUFF_S], v[RBUFF_S], *p;
        int i, max = 128;
        AILSA_LIST *list;
        FILE *f;

        create_kv_list(&list);
        if (!(f = fopen(path, "r")))
                goto cleanup;
        for (i = 0; max > 0; i++) {
                if (!(fgets(l, TBUFF_S, f)))
                        break;
                if (sscanf(l, " %[#\n\r]", k))
                        continue;
                if (sscanf(l, " %[a-zA-Z0-9_] %[^#\n\r]", k, v) < 2)
                        continue;
                for (p = k; *p; p++) if (isalpha(*p)) *p = tolower(*p);
                if ((add_to_kv_list(list, k, v)) != 0)
                        continue;
        }
        if (!(s = get_value_from_kv_list(list, "server"))) {
                ck_assert_ptr_nonnull(s);
                goto cleanup;
        }
        ck_assert_str_eq(s, server);
        if (!(s = get_value_from_kv_list(list, "base"))) {
                ck_assert_ptr_nonnull(s);
                goto cleanup;
        }
        ck_assert_str_eq(s, base);

        cleanup:
                ck_assert_ptr_nonnull(f);
                if (f)
                        fclose(f);
                ailsa_list_destroy(list);
                my_free(list);
}
END_TEST

Suite * kv_suite(void)
{
        Suite *s;
        TCase *ailsa;
        s = suite_create("kv");
        ailsa = tcase_create("kv_list");
        tcase_add_test(ailsa, check_kv_list);
        suite_add_tcase(s, ailsa);

        return s;
}

int
main(void)
{
        int failed = 0;
        Suite *s;
        SRunner *sr;
 
        s = kv_suite();
        sr = srunner_create(s);

        srunner_run_all(sr, CK_ENV);
        failed = srunner_ntests_failed(sr);
        srunner_free(sr);
        return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
