#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#include "../../main/utils/polynomial.c"
#include "../../main/utils/galois.c"

START_TEST(test_polynomial_addition)
{
    uint8_t poly_res[10] = {0};
 
    uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
    uint8_t poly2[] = {0, 0, 0, 1}; // x^3

    uint8_t expected_poly[] = {0, 0, 1, 1, 1, 0, 1}; // x^9+x^7+x^5

    int res;

    res = poly_add(poly1, 7, poly2, 4, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = 7;

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}

START_TEST(test_polynomial_multiplication)
{
    uint8_t poly_res[10] = {0};
 
    uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
    uint8_t poly2[] = {0, 0, 0, 1}; // x^3

    uint8_t expected_poly[] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1}; // x^9+x^7+x^5

    int res;

    res = poly_mult(poly1, 7, poly2, 4, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = (7-1)+(4-1)+1;

    setvbuf(stdout, NULL, _IONBF, 0); 

    for(size_t i = 0; i < poly_res_size; i++){
        printf("%d, ", poly_res[i]);
    }

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}
END_TEST

Suite *
suite(void)
{
    Suite *s = suite_create("polynomial");
    TCase *tc = tcase_create("polynomial");

    tcase_add_test(tc, test_polynomial_multiplication);
    // tcase_add_test(tc, test_bmp_handler_xwvu_to_pixels);
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
