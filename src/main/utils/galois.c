#include <galois.h>
#include <stdio.h>

static uint8_t mult_table[1<<8][1<<8];

static uint8_t inv_mult_table[1<<8];

static uint8_t gmul_russian(uint8_t a, uint8_t b, uint16_t g) {
	uint8_t p = 0; /* the product of the multiplication */
	while (a && b) {
            if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
                p ^= a; /* since we're in GF(2^m), addition is an XOR */

            if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
                a = (a << 1) ^ g; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible */
            else
                a <<= 1; /* equivalent to a*2 */
            b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}

static uint8_t gf_degree(uint8_t a) {
    uint8_t res = 0;
    a >>= 1;
    while (a != 0) {
        a >>= 1;
        res += 1;
    }
    return res;
}

static uint8_t gf_invert(uint16_t a, uint16_t mod) {
    uint16_t v = mod;
    uint16_t g1 = 1;
    uint16_t g2 = 0;
    int j = gf_degree(a) - 8;

    uint16_t aux;

    while (a != 1) {
        if (j < 0) {
            aux = v;
            v = a;
            a = aux;

            aux = g1;
            g1 = g2;
            g2 = aux;

            j = -j;
        }

        a ^= v << j;
        g1 ^= g2 << j;

        a %= 256;  // Emulating 8-bit overflow
        g1 %= 256; // Emulating 8-bit overflow

        j = gf_degree(a) - gf_degree(v);
    }

    return (uint8_t) g1;
}

int galois_init(uint16_t g) {

    // init mult table
    for (int i=1; i<(1<<8); i++) {
        for (int j=1; j<(1<<8); j++) {
            mult_table[i-1][j-1] = gmul_russian(i, j, g);
        }
    }

    // init inv mult table
    for (int i=1; i<(1<<8); i++) {
        inv_mult_table[i-1] = gf_invert(i, g);
    }

    return 0;
}

uint8_t gadd(uint8_t a, uint8_t b) {
    // In Galois x+x is a XOR
	return a ^ b;
}

uint8_t gmul(uint8_t a, uint8_t b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return mult_table[a-1][b-1];
}

uint8_t gsub(uint8_t a, uint8_t b) {
    // In Galois x+x = x-x cause x is it's own additive inverse 
	return gadd(a, b); 
}

uint8_t gdiv(uint8_t a, uint8_t b) {
    if (a == 0 || b == 0) { // TODO: b == 0 es ERROR
        return 0;
    }

    uint8_t inv_mul = inv_mult_table[b-1];
    // In Galois the division is a*ḅ^(-1) / ḅ^(-1)*b = 1
	return gmul(a, inv_mul);
}