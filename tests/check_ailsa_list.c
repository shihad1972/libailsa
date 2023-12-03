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
 *  check_ailsa_list.c
 *
 *  Test suite definition for AILSA_LIST object
 *
 *  Part of the libailsa
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "../include/ailsa.h"

START_TEST(check_list_init)
{
        int retval = 0;
        AILSA_LIST *list = ailsa_calloc(sizeof(AILSA_LIST), "AILSA_LIST in check_list_init");
        AILSA_STRING *head, *tail;
        ailsa_list_init(list, ailsa_free_string, ailsa_compare_string);
        if ((retval = ailsa_init_string(&head)) != 0)
                fprintf(stderr, "AILSA_STRING head init failed\n");
        if ((retval = ailsa_init_string(&tail)) != 0)
                fprintf(stderr, "AILSA_STRING tail init failed\n");
        if ((retval = ailsa_list_insert_head(list, head)) != 0)
                fprintf(stderr, "Insert into HEAD failed\n");
        if ((retval = ailsa_list_insert_tail(list, tail)) != 0)
                fprintf(stderr, "Insert into TAIL failed\n");
        ck_assert_ptr_nonnull(list->destroy);
        ck_assert_ptr_nonnull(list->head);
        ck_assert_ptr_nonnull(list->tail);
        ck_assert_ptr_eq(list->head, ((AILSA_ELEM *)list->tail)->prev);
        ck_assert_ptr_eq(list->tail, ((AILSA_ELEM *)list->head)->next);
        ailsa_list_destroy(list);
        free(list);
}
END_TEST

START_TEST(check_list_insert)
{
        int retval = 0;
        AILSA_LIST *list = ailsa_calloc(sizeof(AILSA_LIST), "AILSA_LIST in check_list_insert");
        AILSA_STRING *head, *tail, *next, *prev;
        AILSA_ELEM *data;
        ailsa_list_init(list, ailsa_free_string, ailsa_compare_string);
        if ((retval = ailsa_init_string(&head)) != 0)
                fprintf(stderr, "AILSA_STRING head init failed\n");
        if ((retval = ailsa_init_string(&tail)) != 0)
                fprintf(stderr, "AILSA_STRING tail init failed\n");
        if ((retval = ailsa_init_string(&next)) != 0)
                fprintf(stderr, "AILSA_STRING next init failed\n");
        if ((retval = ailsa_init_string(&prev)) != 0)
                fprintf(stderr, "AILSA_STRING prev init failed\n");
        if ((retval = ailsa_list_insert_head(list, head)) != 0)
                fprintf(stderr, "Insert into HEAD failed\n");
        if ((retval = ailsa_list_insert_tail(list, tail)) != 0)
                fprintf(stderr, "Insert into TAIL failed\n");
        data = list->tail;
        if ((retval = ailsa_list_ins_next(list, data, next))!= 0)
                fprintf(stderr, "Insert into NEXT failed\n");
        ck_assert_ptr_ne(data, list->tail);
        ck_assert_ptr_eq(data, list->tail->prev);
        data = list->head;
        if ((retval = ailsa_list_ins_prev(list, data, prev)) != 0)
                fprintf(stderr, "Insert into PREV failed\n");
        ck_assert_ptr_ne(data, list->head);
        ck_assert_ptr_eq(data, list->head->next);
        ailsa_list_destroy(list);
        free(list);
}
END_TEST

START_TEST(check_string_compare)
{
        int retval = 0;
        size_t pos = 0;
        const char *search = "We will search for this string";
        AILSA_LIST *list = ailsa_calloc(sizeof(AILSA_LIST), "AILSA_LIST in check_list_compare");
        AILSA_STRING *head, *tail, *middle, *pop;
        ailsa_list_init(list, ailsa_free_string, ailsa_compare_string);
        if ((retval = ailsa_init_string(&head)) != 0)
                fprintf(stderr, "AILSA_STRING head init failed\n");
        if ((retval = ailsa_add_to_string(head, "head")) != 0)
                fprintf(stderr, "Cannot add to head string\n");
        if ((retval = ailsa_init_string(&tail)) != 0)
                fprintf(stderr, "AILSA_STRING tail init failed\n");
        if ((retval = ailsa_add_to_string(tail, "tail")) != 0)
                fprintf(stderr, "Cannot add to tail string\n");
        if ((retval = ailsa_init_string(&middle)) != 0)
                fprintf(stderr, "AILSA_STRING middle init failed\n");
        if ((retval = ailsa_add_to_string(middle, "middle")) != 0)
                fprintf(stderr, "Cannot add to middle string\n");
        if ((retval = ailsa_init_string(&pop)) != 0)
                fprintf(stderr, "AILSA_STRING pop init failed\n");
        if ((retval = ailsa_add_to_string(pop, search)) != 0)
                fprintf(stderr, "Cannot add to pop string\n");
        if ((retval = ailsa_list_insert_head(list, head)) != 0)
                fprintf(stderr, "Cannot insert into head of list\n");
        if ((retval = ailsa_list_insert_tail(list, tail)) != 0)
                fprintf(stderr, "Cannot insert into tail of list\n");
        if ((retval = ailsa_list_ins_next(list, list->head, middle)) != 0)
                fprintf(stderr, "Cannot insert middle after head\n");
        if ((retval = ailsa_list_ins_prev(list, list->tail, pop)) != 0)
                fprintf(stderr, "Cannot insert pop before tail\n");
        ck_assert_uint_eq(list->total, 4);
        if ((retval = ailsa_list_position(list, &pos, search)) != 0)
                fprintf(stderr, "Failed to find search string\n");
        ck_assert_uint_eq(pos, 2);
        ailsa_list_destroy(list);
        free(list);
}
END_TEST

Suite * list_suite(void)
{
        Suite *s;
        TCase *ailsa;
        s = suite_create("list");
        ailsa = tcase_create("list_core");
        tcase_add_test(ailsa, check_list_init);
        tcase_add_test(ailsa, check_list_insert);
        tcase_add_test(ailsa, check_string_compare);
        suite_add_tcase(s, ailsa);
        return s;
}

int
main(void)
{
        int failed = 0;
        Suite *s;
        SRunner *sr;
        s = list_suite();
        sr = srunner_create(s);

        srunner_run_all(sr, CK_ENV);
        failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
