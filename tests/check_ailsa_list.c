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
        ailsa_list_init(list, ailsa_free_string);
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
        ailsa_list_init(list, ailsa_free_string);
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
        ck_assert_ptr_ne(data, (AILSA_ELEM *)list->tail);
        ck_assert_ptr_eq(data, ((AILSA_ELEM *)list->tail)->prev);
        data = list->head;
        if ((retval = ailsa_list_ins_prev(list, data, prev)) != 0)
                fprintf(stderr, "Insert into PREV failed\n");
        ck_assert_ptr_ne(data, (AILSA_ELEM *)list->head);
        ck_assert_ptr_eq(data, ((AILSA_ELEM *)list->head)->next);
        ailsa_list_destroy(list);
        free(list);
}

Suite * list_suite(void)
{
        Suite *s;
        TCase *ailsa;
        s = suite_create("list");
        ailsa = tcase_create("list_core");
        tcase_add_test(ailsa, check_list_init);
        tcase_add_test(ailsa, check_list_insert);
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
