#include <stdlib.h>
#include <check.h>

#include "../../main/utils/polynomial.c"
#include "../../main/utils/galois.c"

static uint16_t g = 0x163;

START_TEST(test_polynomial_addition)
{
    galois_init(g);

    uint8_t poly_res[10] = {0};
 
    uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
    uint8_t poly2[] = {0, 0, 0, 1}; // x^3

    uint8_t expected_poly[] = {0, 0, 1, 1, 1, 0, 1};

    int res;

    res = poly_add(poly1, 7, poly2, 4, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = 7;

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}
END_TEST

START_TEST(test_polynomial_multiplication_by_scalar)
{
    galois_init(g);

    uint8_t poly1[] = {0, 0, 4, 0, 200, 0, 5};
    uint8_t scalar = 2;

    uint8_t poly_res[7] = {0};

    uint8_t expected_poly[] = {0, 0, 8, 0, 243, 0, 10};

    int res;

    res = poly_mult_scalar(poly1, 7, scalar, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = 7;

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}
END_TEST

START_TEST(test_polynomial_division_by_scalar)
{
    galois_init(g);

    uint8_t poly1[] = {0, 0, 8, 0, 243, 0, 10};
    uint8_t scalar = 2;

    uint8_t poly_res[7] = {0};

    uint8_t expected_poly[] = {0, 0, 4, 0, 200, 0, 5};

    int res;

    res = poly_div_scalar(poly1, 7, scalar, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = 7;

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}
END_TEST

START_TEST(test_polynomial_multiplication)
{
    galois_init(g);

    uint8_t poly_res[10] = {0};
 
    uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
    uint8_t poly2[] = {0, 0, 0, 1}; // x^3

    uint8_t expected_poly[] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1}; // x^9+x^7+x^5

    int res;

    res = poly_mult(poly1, 7, poly2, 4, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = (7-1)+(4-1)+1;

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(expected_poly[i], poly_res[i]);
    }
}
END_TEST

START_TEST(test_polynomial_lagrange_interpolation)
{
    galois_init(g);

    uint8_t poly_res[3] = {0};

    uint8_t tuples[][2] = {{1,0}, {2,14}, {3,10}};

    uint8_t expected_poly[] = {1, 2, 3};

    int res;

    res = lagrange_interpolate(tuples, 3, poly_res);
    
    ck_assert_uint_eq(0, res);

    size_t poly_res_size = 3;

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

    tcase_add_test(tc, test_polynomial_addition);
    tcase_add_test(tc, test_polynomial_multiplication_by_scalar);
    tcase_add_test(tc, test_polynomial_division_by_scalar);
    tcase_add_test(tc, test_polynomial_multiplication);
    tcase_add_test(tc, test_polynomial_lagrange_interpolation);
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
