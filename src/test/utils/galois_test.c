#include <stdlib.h>
#include <check.h>

#include "../../main/utils/galois.c"

uint8_t gmul_russian(uint8_t a, uint8_t b) {
	uint8_t p = 0; /* the product of the multiplication */
	while (a && b) {
            if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
                p ^= a; /* since we're in GF(2^m), addition is an XOR */

            if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
                a = (a << 1) ^ 0x163; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible */
            else
                a <<= 1; /* equivalent to a*2 */
            b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}

START_TEST(galois_mult_matrix_test)
{
    uint8_t ans1, ans2;

    for (unsigned int x=1; x<256; x++) {
        for (unsigned int y=1; y<256; y++) {
            ans1 = gmul(x, y); // table 
            ans2 = gmul_russian(x, y); //ruso

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
