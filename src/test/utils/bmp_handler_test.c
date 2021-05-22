#include <stdlib.h>
#include <check.h>

#include "../../main/utils/bmp_handler.c"

START_TEST(test_bmp_handler_pixels_to_xwvu)
{
    size_t width = 6;
    size_t height = 8;

    uint8_t pixels[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47};

    xwvu expected[] = {
        {42,43,36,37}, 
        {44,45,38,39}, 
        {46,47,40,41}, 
        {30,31,24,25}, 
        {32,33,26,27}, 
        {34,35,28,29},
        {18,19,12,13}, 
        {20,21,14,15}, 
        {22,23,16,17}, 
        {6,7,0,1}, 
        {8,9,2,3}, 
        {10,11,4,5}
    };

    int res;
    
    size_t xwvu_size;
    xwvu * xwvu_arr;
    
    res = pixels_to_xwvu(pixels, sizeof(pixels)/sizeof(pixels[0]), &xwvu_arr, &xwvu_size, width, height);

    ck_assert_uint_eq(0, res);

    for (int i=0; i < (int) xwvu_size; i++) {
        ck_assert_uint_eq(xwvu_arr[i].x, expected[i].x);
        ck_assert_uint_eq(xwvu_arr[i].w, expected[i].w);
        ck_assert_uint_eq(xwvu_arr[i].v, expected[i].v);
        ck_assert_uint_eq(xwvu_arr[i].u, expected[i].u);
    }

    free_xwvu_array(xwvu_arr);
}
END_TEST

START_TEST(test_bmp_handler_xwvu_to_pixels)
{
    size_t width = 6;
    size_t height = 8;

    uint8_t expected[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47};

    xwvu xwvu[] = {
        {42,43,36,37}, 
        {44,45,38,39}, 
        {46,47,40,41}, 
        {30,31,24,25}, 
        {32,33,26,27}, 
        {34,35,28,29},
        {18,19,12,13}, 
        {20,21,14,15}, 
        {22,23,16,17}, 
        {6,7,0,1}, 
        {8,9,2,3}, 
        {10,11,4,5}
    };

    int res;
    
    size_t pixels_size;
    uint8_t * pixels_arr;
    
    res = xwvu_to_pixels(xwvu, sizeof(xwvu)/sizeof(xwvu[0]), &pixels_arr, &pixels_size, width, height);

    ck_assert_uint_eq(0, res);

    // printf("[");
    // for (int i=0; i < (int) pixels_size; i++) {
    //     printf("%d", pixels_arr[i]);
    //     if (i != pixels_size-1) {
    //         printf(", ");
    //     }
    // }
    // printf("]\n");

    for (int i=0; i < (int) pixels_size; i++) {
        ck_assert_uint_eq(expected[i], pixels_arr[i]);
    }

    free_pixels_array(pixels_arr);
}
END_TEST

Suite *
suite(void)
{
    Suite *s = suite_create("bmp_handler");
    TCase *tc = tcase_create("bmp_handler");

    tcase_add_test(tc, test_bmp_handler_pixels_to_xwvu);
    tcase_add_test(tc, test_bmp_handler_xwvu_to_pixels);
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
