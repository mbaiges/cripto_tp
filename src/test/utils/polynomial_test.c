#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#include "../../main/utils/polynomial.c"
#include "../../main/utils/galois.c"

static uint16_t POLY_GEN = 0x163;

// START_TEST(test_polynomial_addition)
// {
//     galois_init(POLY_GEN);

//     uint8_t poly_res[10] = {0};
 
//     uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
//     uint8_t poly2[] = {0, 0, 0, 1}; // x^3

//     uint8_t expected_poly[] = {0, 0, 1, 1, 1, 0, 1};

//     int res;

//     res = poly_add(poly1, 7, poly2, 4, poly_res);
    
//     ck_assert_uint_eq(0, res);

//     size_t poly_res_size = 7;

//     for(size_t i = 0; i < poly_res_size; i++){
//         ck_assert_uint_eq(expected_poly[i], poly_res[i]);
//     }
// }
// END_TEST

// START_TEST(test_polynomial_multiplication_by_scalar)
// {
//     galois_init(POLY_GEN);

//     uint8_t poly1[] = {0, 0, 4, 0, 200, 0, 5};
//     uint8_t scalar = 2;

//     uint8_t poly_res[7] = {0};

//     uint8_t expected_poly[] = {0, 0, 8, 0, 243, 0, 10};

//     int res;

//     res = poly_mult_scalar(poly1, 7, scalar, poly_res);
    
//     ck_assert_uint_eq(0, res);

//     size_t poly_res_size = 7;

//     for(size_t i = 0; i < poly_res_size; i++){
//         ck_assert_uint_eq(expected_poly[i], poly_res[i]);
//     }
// }
// END_TEST

// START_TEST(test_polynomial_division_by_scalar)
// {
//     galois_init(POLY_GEN);

//     uint8_t poly1[] = {0, 0, 8, 0, 243, 0, 10};
//     uint8_t scalar = 2;

//     uint8_t poly_res[7] = {0};

//     uint8_t expected_poly[] = {0, 0, 4, 0, 200, 0, 5};

//     int res;

//     res = poly_div_scalar(poly1, 7, scalar, poly_res);
    
//     ck_assert_uint_eq(0, res);

//     size_t poly_res_size = 7;

//     for(size_t i = 0; i < poly_res_size; i++){
//         ck_assert_uint_eq(expected_poly[i], poly_res[i]);
//     }
// }
// END_TEST

// START_TEST(test_polynomial_multiplication)
// {
//     galois_init(POLY_GEN);

//     uint8_t poly_res[10] = {0};
 
//     uint8_t poly1[] = {0, 0, 1, 0, 1, 0, 1}; // x^6+x^4+x^2
//     uint8_t poly2[] = {0, 0, 0, 1}; // x^3

//     uint8_t expected_poly[] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1}; // x^9+x^7+x^5

//     int res;

//     res = poly_mult(poly1, 7, poly2, 4, poly_res);
    
//     ck_assert_uint_eq(0, res);

//     size_t poly_res_size = (7-1)+(4-1)+1;

//     for(size_t i = 0; i < poly_res_size; i++){
//         ck_assert_uint_eq(expected_poly[i], poly_res[i]);
//     }
// }
// END_TEST

START_TEST(test_polynomial_eval)
{
    galois_init(POLY_GEN);

    uint8_t poly_res = 0;

    uint8_t poly[] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t x = 5;

    uint8_t expected_y = 203;

    int res;

    res = poly_eval(poly, sizeof(poly)/sizeof(poly[0]), x, &poly_res);
    
    ck_assert_uint_eq(0, res);
    ck_assert_uint_eq(expected_y, poly_res);

    uint8_t poly2_res = 0;

    uint8_t poly2[] = {8, 3, 0, 1, 220, 255, 188, 101};
    uint8_t x2 = 254;

    uint8_t expected_y2 = 102;

    int res2;

    res2 = poly_eval(poly2, sizeof(poly2)/sizeof(poly2[0]), x2, &poly2_res);
    
    ck_assert_uint_eq(0, res2);
    ck_assert_uint_eq(expected_y2, poly2_res);
}
END_TEST

START_TEST(test_polynomial_interpolation)
{
    galois_init(POLY_GEN);

    uint8_t poly[] = {2, 4};
    uint8_t x[] = {255, 254};
    uint8_t y[] = {91, 95};

    int res;

    size_t poly_res_size = sizeof(x)/sizeof(x[0]);
    uint8_t poly_res[poly_res_size];

    res = poly_interpolate(x, y, poly_res_size, poly_res);
    
    ck_assert_uint_eq(0, res);

    for(size_t i = 0; i < poly_res_size; i++){
        ck_assert_uint_eq(poly[i], poly_res[i]);
    }

    uint8_t poly2[] = {8, 3, 0, 1, 220, 255, 188, 101};
    uint8_t x2[] = {255, 254, 200, 40, 100, 126, 127, 99};
    uint8_t y2[] = {207, 102,  63, 148, 103,  86,  36,  77};

    int res2;

    size_t poly2_res_size = sizeof(x2)/sizeof(x2[0]);
    uint8_t poly2_res[poly2_res_size];

    res2 = poly_interpolate(x2, y2, poly2_res_size, poly2_res);
    
    ck_assert_uint_eq(0, res2);

    for(size_t i = 0; i < poly2_res_size; i++){
        ck_assert_uint_eq(poly2[i], poly2_res[i]);
    }

    uint8_t poly3[] = {1, 255, 10};
    uint8_t x3[] = {1, 2, 3};
    uint8_t y3[] = {244, 180, 65};

    int res3;

    size_t poly3_res_size = sizeof(x3)/sizeof(x3[0]);
    uint8_t poly3_res[poly3_res_size];

    res3 = poly_interpolate(x3, y3, poly3_res_size, poly3_res);
    
    ck_assert_uint_eq(0, res3);

    for(size_t i = 0; i < poly3_res_size; i++){
        ck_assert_uint_eq(poly3[i], poly3_res[i]);
    }
}
END_TEST

Suite *
suite(void)
{
    Suite *s = suite_create("polynomial");
    TCase *tc = tcase_create("polynomial");

    // tcase_add_test(tc, test_polynomial_addition);
    // tcase_add_test(tc, test_polynomial_multiplication_by_scalar);
    // tcase_add_test(tc, test_polynomial_division_by_scalar);
    // tcase_add_test(tc, test_polynomial_multiplication);
    tcase_add_test(tc, test_polynomial_eval);
    tcase_add_test(tc, test_polynomial_interpolation);
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
