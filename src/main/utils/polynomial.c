#include <polynomial.h>
#include <galois.h>
#include <string.h>

int poly_add(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result) {
    size_t i;
    for (i=0; i<poly1_g || i<poly2_g; i++) {
        if (i<poly1_g && i<poly2_g) {
            result[i] = gadd(poly1[i], poly2[i]);
        }
        else if (i<poly1_g) {
            result[i] = poly1[i];
        }
        else {
            result[i] = poly2[i];
        }
    }
    return 0;
}

int poly_mult(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result) {
    size_t i, j;

    size_t result_grade = (poly1_g-1) + (poly2_g-1) + 1;
    memset((void *) result, 0, result_grade * sizeof(uint8_t)); // inicializacion de result en 0
    
    setvbuf(stdout, NULL, _IONBF, 0); 
    printf("Multiplying!\n");
    for (i=0; i<poly1_g; i++) {
        for (j=0; j<poly2_g; j++) {
            result[i+j] = gadd(result[i+j], gmul(poly1[i], poly2[j]));
            printf("result[%d * %d] = %d\n", i, j, result[i+j]);
        }
    }
    return 0;
}

int poly_mult_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result) {
    for (size_t i=0; i<poly_g; i++) {
        result[i] = gmul(poly[i], scalar);
    }
    return 0;
}

int poly_div_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result) {
    for (size_t i=0; i<poly_g; i++) {
        result[i] = gdiv(poly[i], scalar);
    }
    return 0;
}

int lagrange_interpolate(uint8_t tuples[][2], size_t k, uint8_t * poly) {
    
    uint8_t x_i, x_j;

    int res;

    uint8_t divisor;

    uint8_t poly_aux1[k];
    uint8_t poly_aux2[k];
    uint8_t poly_aux3[k];

    uint8_t poly_mult_acum[k];

    memset((void *) poly_mult_acum, 0, k * sizeof(uint8_t));
    
    poly_aux1[1] = 1;

    for(size_t i =0; i < k; i++){
        x_i = tuples[i][0];
    
        for(size_t j = 0; j < k; j++){
            x_j = tuples[j][0];

            // L(t) = (t-x_j)/(x_i-x_j)
            if (j != i) {
                poly_aux1[0] = -x_j;
                divisor = x_i - x_j;
                res = poly_div_scalar(poly_aux1, 2, divisor, poly_aux2);
                if (res != 0) {
                    return res;
                }

                for(size_t n=0; n<k; n++) {
                    poly_aux3[n] = poly_mult_acum[n];
                }

                res = poly_mult(poly_aux2, 2, poly_aux3, k, poly_mult_acum);
                if (res != 0) {
                    return res;
                }
            }
        }
    }

    return 0;
}