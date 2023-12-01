#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "../include/ailsa.h"

typedef struct ailsa_dn_s {
        char rdn[HOST_S];
        char dn[RBUFF_S];
} ailsa_dn_s;

START_TEST(check_kv_init)
{
        AILSA_DICT *kv = NULL;

        init_kv_s(&kv);
        ck_assert_ptr_nonnull(kv);
        clean_kv_s(kv);
}
END_TEST

START_TEST(check_kv_object)
{
        AILSA_DICT *kv = NULL;
        const char *key = "format";
        const char *value = "stable";
        init_kv_s(&kv);
        ck_assert_ptr_nonnull(kv);
        put_kv_key(kv, key);
        put_kv_value(kv, value);
        ck_assert_ptr_nonnull(get_kv_key(kv));
        ck_assert_ptr_nonnull(get_kv_value(kv));
        ck_assert_str_eq(get_kv_key(kv), key);
        ck_assert_str_eq(get_kv_value(kv), value);
        clean_kv_s(kv);
}
END_TEST

START_TEST(check_kv_list)
{
        int retval = 0;
        size_t pos = 0;
        ailsa_dn_s *dn = ailsa_calloc(sizeof(ailsa_dn_s), "ailsa_dn_s dn in check_kv_list");
        ailsa_dn_s *ddn = ailsa_calloc(sizeof(ailsa_dn_s), "ailsa_dn_s cmp in check_kv_list");
        sprintf(dn->rdn, "ailsa");
        sprintf(dn->dn, "shihad.org");
        AILSA_DICT *head, *tail, *cmp;
        init_kv_s(&head);
        init_kv_s(&tail);
        init_kv_s(&cmp);
        put_kv_key(head, "dn");
        put_kv_key(cmp, "dn");
        if ((retval = put_kv_data(head, dn, sizeof(ailsa_dn_s))) != 0)
                fprintf(stderr, "Unable to insert data into head\n");
        memcpy(ddn, dn, sizeof(ailsa_dn_s));
        if ((retval = put_kv_data(cmp, ddn, sizeof(ailsa_dn_s))) != 0)
                fprintf(stderr, "Unable to insert data into cmp\n");
        put_kv_key(tail, "cn");
        put_kv_value(tail, "iain");
        AILSA_LIST *list = ailsa_calloc(sizeof(AILSA_LIST), "AILSA_LIST in check_kv_list");
        ailsa_list_init(list, clean_kv_s, compare_kv);
        if ((retval = ailsa_list_insert_head(list, head)) != 0)
                fprintf(stderr, "Cannot insert into list head\n");
        if ((retval = ailsa_list_insert_tail(list, tail)) != 0)
                fprintf(stderr, "Cannot insert into list tail\n");
        if ((retval = ailsa_list_position(list, &pos, cmp)) != 0)
                fprintf(stderr, "Cannot find position of cmp in list");
        ck_assert_uint_eq(pos, 0);
        ailsa_list_destroy(list);
        clean_kv_s(cmp);
        my_free(list);
}
END_TEST

Suite * kv_suite(void)
{
        Suite *s;
        TCase *ailsa;
        s = suite_create("kv");
        ailsa = tcase_create("kv_core");
        tcase_add_test(ailsa, check_kv_init);
        tcase_add_test(ailsa, check_kv_object);
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
