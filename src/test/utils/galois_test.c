#include <stdlib.h>
#include <check.h>

#include "../../main/utils/galois.c"

static uint16_t g = 0x163;

START_TEST(galois_mult_matrix_test)
{
    uint8_t ans1, ans2;

    galois_init(g);

    for (unsigned int x=1; x<256; x++) {
        for (unsigned int y=x; y<50; y++) {
            ans1 = gmul(x, y); // table 
            ans2 = gmul_russian(x, y, g); //ruso

            ck_assert_uint_eq(ans2, ans1);
        }
    }
}
END_TEST


Suite *
suite(void)
{
    Suite *s = suite_create("galois");
    TCase *tc = tcase_create("galois");

    tcase_add_test(tc, galois_mult_matrix_test); 
    suite_add_tcase(s, tc);

    return s;
}

int main(void)
{
    SRunner *sr = srunner_create(suite());
    int number_failed;

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
